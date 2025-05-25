/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:30:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 19:25:25 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"
#include "game.h"

static void	physics_update_projectiles(t_game *game);
static void	physics_update_enemies(t_game *game);
static void	physics_handle_area_damage(t_game *game, t_vector2 pos, 
				float radius, int damage);
static bool	physics_raycast(t_game *game, t_vector2 start, t_vector2 end, 
				t_vector2 *hit_point);

void	physics_update(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	physics_update_projectiles(game);
	physics_update_enemies(game);
}

bool	physics_circle_collision(t_vector2 pos1, float r1, t_vector2 pos2,
		float r2)
{
	float	distance_squared;
	float	radius_sum;

	distance_squared = math_distance_squared(pos1, pos2);
	radius_sum = r1 + r2;
	return (distance_squared <= (radius_sum * radius_sum));
}

bool	physics_point_in_circle(t_vector2 point, t_vector2 center, float radius)
{
	return (math_distance_squared(point, center) <= (radius * radius));
}

bool	physics_point_in_rect(t_vector2 point, t_rect rect)
{
	return (point.x >= rect.x && point.x < rect.x + rect.w
		&& point.y >= rect.y && point.y < rect.y + rect.h);
}

float	physics_distance(t_vector2 a, t_vector2 b)
{
	return (math_distance(a, b));
}

t_vector2	physics_normalize(t_vector2 vec)
{
	return (math_normalize(vec));
}

t_vector2	physics_rotate(t_vector2 vec, float angle)
{
	return (math_rotate(vec, angle));
}

bool	physics_line_circle_collision(t_vector2 line_start, t_vector2 line_end,
		t_vector2 circle_center, float radius)
{
	t_vector2	line_vec;
	t_vector2	to_circle;
	float		line_length_squared;
	float		projection;
	t_vector2	closest_point;
	float		distance_squared;

	line_vec = vec2_subtract(line_end, line_start);
	to_circle = vec2_subtract(circle_center, line_start);
	line_length_squared = math_magnitude_squared(line_vec);
	if (line_length_squared == 0.0f)
		return (physics_point_in_circle(circle_center, line_start, radius));
	projection = (to_circle.x * line_vec.x + to_circle.y * line_vec.y) 
		/ line_length_squared;
	projection = math_clamp(projection, 0.0f, 1.0f);
	closest_point.x = line_start.x + projection * line_vec.x;
	closest_point.y = line_start.y + projection * line_vec.y;
	distance_squared = math_distance_squared(circle_center, closest_point);
	return (distance_squared <= (radius * radius));
}

t_vector2	physics_predict_position(t_vector2 current_pos, t_vector2 velocity,
		float time)
{
	t_vector2	predicted_pos;

	predicted_pos.x = current_pos.x + velocity.x * time;
	predicted_pos.y = current_pos.y + velocity.y * time;
	return (predicted_pos);
}

t_vector2	physics_calculate_intercept(t_vector2 shooter_pos, 
		t_vector2 target_pos, t_vector2 target_vel, float projectile_speed)
{
	t_vector2	relative_pos;
	float		a;
	float		b;
	float		c;
	float		discriminant;
	float		time1;
	float		time2;
	float		intercept_time;
	t_vector2	intercept_pos;

	relative_pos = vec2_subtract(target_pos, shooter_pos);
	a = math_magnitude_squared(target_vel) - (projectile_speed * projectile_speed);
	b = 2.0f * (relative_pos.x * target_vel.x + relative_pos.y * target_vel.y);
	c = math_magnitude_squared(relative_pos);
	discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f || a == 0.0f)
		return (target_pos);
	time1 = (-b - sqrtf(discriminant)) / (2.0f * a);
	time2 = (-b + sqrtf(discriminant)) / (2.0f * a);
	intercept_time = (time1 > 0.0f) ? time1 : time2;
	if (intercept_time < 0.0f)
		return (target_pos);
	intercept_pos.x = target_pos.x + target_vel.x * intercept_time;
	intercept_pos.y = target_pos.y + target_vel.y * intercept_time;
	return (intercept_pos);
}

void	physics_apply_knockback(t_enemy *enemy, t_vector2 source_pos, 
		float force)
{
	t_vector2	direction;
	t_vector2	knockback;

	if (!validate_pointer(enemy) || !enemy->base.active)
		return ;
	direction = vec2_subtract(enemy->base.pos, source_pos);
	direction = math_normalize(direction);
	knockback = vec2_multiply(direction, force);
	enemy->base.pos = vec2_add(enemy->base.pos, knockback);
}

bool	physics_is_path_clear(t_game *game, t_vector2 start, t_vector2 end)
{
	t_vector2	hit_point;

	return (!physics_raycast(game, start, end, &hit_point));
}

static void	physics_update_projectiles(t_game *game)
{
	t_projectile	*projectile;
	t_enemy			*enemy;
	int				i;
	int				j;
	float			distance;

	i = 0;
	while (i < MAX_PROJECTILES)
	{
		projectile = &game->projectiles[i];
		if (projectile->base.active)
		{
			j = 0;
			while (j < MAX_ENEMIES)
			{
				enemy = &game->enemies[j];
				if (enemy->base.active)
				{
					distance = math_distance(projectile->base.pos, 
						enemy->base.pos);
					if (distance <= (projectile->base.radius + enemy->base.radius))
					{
						effects_create_blood(game, enemy->base.pos);
						effects_create_sparks(game, projectile->base.pos, 3);
						enemy_take_damage(enemy, projectile->damage, game);
						if (projectile->explosive)
						{
							physics_handle_area_damage(game, 
								projectile->base.pos, 
								projectile->explosion_radius, 
								projectile->damage / 2);
						}
						if (!projectile->piercing)
						{
							projectile->base.active = false;
							break ;
						}
					}
				}
				j++;
			}
		}
		i++;
	}
}

static void	physics_update_enemies(t_game *game)
{
	t_enemy	*enemy1;
	t_enemy	*enemy2;
	int		i;
	int		j;
	float	distance;
	float	overlap;
	t_vector2	separation;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		enemy1 = &game->enemies[i];
		if (enemy1->base.active)
		{
			j = i + 1;
			while (j < MAX_ENEMIES)
			{
				enemy2 = &game->enemies[j];
				if (enemy2->base.active)
				{
					distance = math_distance(enemy1->base.pos, enemy2->base.pos);
					if (distance < (enemy1->base.radius + enemy2->base.radius))
					{
						overlap = (enemy1->base.radius + enemy2->base.radius) 
							- distance;
						separation = vec2_subtract(enemy2->base.pos, 
							enemy1->base.pos);
						separation = math_normalize(separation);
						separation = vec2_multiply(separation, overlap * 0.5f);
						enemy1->base.pos = vec2_subtract(enemy1->base.pos, 
							separation);
						enemy2->base.pos = vec2_add(enemy2->base.pos, 
							separation);
					}
				}
				j++;
			}
		}
		i++;
	}
}

static void	physics_handle_area_damage(t_game *game, t_vector2 pos, 
		float radius, int damage)
{
	t_enemy	*enemy;
	int		i;
	float	distance;

	effects_create_explosion(game, pos, 4);
	effects_create_sparks(game, pos, 15);
	i = 0;
	while (i < MAX_ENEMIES)
	{
		enemy = &game->enemies[i];
		if (enemy->base.active)
		{
			distance = math_distance(pos, enemy->base.pos);
			if (distance <= radius)
			{
				effects_create_blood(game, enemy->base.pos);
				physics_apply_knockback(enemy, pos, 10.0f);
				enemy_take_damage(enemy, damage, game);
			}
		}
		i++;
	}
}

static bool	physics_raycast(t_game *game, t_vector2 start, t_vector2 end, 
		t_vector2 *hit_point)
{
	t_tower	*tower;
	int		i;

	if (!validate_pointer(game) || !validate_pointer(hit_point))
		return (false);
	i = 0;
	while (i < MAX_TOWERS)
	{
		tower = &game->towers[i];
		if (tower->base.active)
		{
			if (physics_line_circle_collision(start, end, tower->base.pos, 
				tower->base.radius))
			{
				*hit_point = tower->base.pos;
				return (true);
			}
		}
		i++;
	}
	return (false);
}

void	physics_create_shockwave(t_game *game, t_vector2 center, float radius,
		float force)
{
	t_enemy	*enemy;
	int		i;
	float	distance;
	float	force_factor;

	if (!validate_pointer(game))
		return ;
	effects_create_explosion(game, center, 6);
	effects_create_sparks(game, center, 20);
	i = 0;
	while (i < MAX_ENEMIES)
	{
		enemy = &game->enemies[i];
		if (enemy->base.active)
		{
			distance = math_distance(center, enemy->base.pos);
			if (distance <= radius)
			{
				force_factor = 1.0f - (distance / radius);
				physics_apply_knockback(enemy, center, force * force_factor);
				effects_create_blood(game, enemy->base.pos);
			}
		}
		i++;
	}
}

float	physics_calculate_damage_falloff(float distance, float max_range,
		int base_damage)
{
	float	falloff_factor;

	if (distance >= max_range)
		return (0.0f);
	falloff_factor = 1.0f - (distance / max_range);
	return ((float)base_damage * falloff_factor);
}
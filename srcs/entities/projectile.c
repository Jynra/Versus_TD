/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 19:19:25 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"
#include "utils.h"
#include "game.h"

static void	projectile_update_movement(t_projectile *projectile, t_game *game);
static void	projectile_check_collisions(t_projectile *projectile, t_game *game);
static void	projectile_apply_damage(t_projectile *projectile, t_enemy *enemy, 
				t_game *game);
static void	projectile_create_trail_effect(t_projectile *projectile, t_game *game);

void	projectile_init(t_projectile *projectile)
{
	if (!validate_pointer(projectile))
		return ;
	entity_init(&projectile->base);
	projectile->damage = 0;
	projectile->target_id = -1;
	projectile->lifetime = PROJECTILE_LIFETIME;
	projectile->piercing = false;
	projectile->explosive = false;
	projectile->explosion_radius = 0.0f;
}

void	projectile_create(t_projectile *proj, t_vector2 pos, t_vector2 target, 
		int damage)
{
	t_vector2	direction;

	if (!validate_pointer(proj))
		return ;
	projectile_init(proj);
	proj->base.active = true;
	proj->base.type = ENTITY_PROJECTILE;
	proj->base.pos = pos;
	proj->base.radius = PROJECTILE_SIZE;
	proj->damage = damage;
	proj->lifetime = PROJECTILE_LIFETIME;
	direction = vec2_subtract(target, pos);
	direction = math_normalize(direction);
	proj->base.vel = vec2_multiply(direction, PROJECTILE_SPEED);
	proj->base.rotation = math_angle_between(vec2_zero(), direction);
	debug_log("Projectile created: pos=(%.1f,%.1f), target=(%.1f,%.1f), damage=%d", 
		pos.x, pos.y, target.x, target.y, damage);
}

bool	spawn_projectile(t_game *game, t_vector2 pos, t_vector2 target, int damage)
{
	int	projectile_id;

	if (!validate_pointer(game))
		return (false);
	projectile_id = entities_find_free_projectile(game);
	if (projectile_id == -1)
	{
		debug_warning("No free projectile slot available");
		return (false);
	}
	projectile_create(&game->projectiles[projectile_id], pos, target, damage);
	
	/* Create muzzle smoke effect */
	effects_create_smoke(game, pos);
	
	debug_log("Projectile spawned: id=%d, damage=%d", projectile_id, damage);
	return (true);
}

void	projectile_update(t_projectile *projectile, t_game *game)
{
	if (!validate_pointer(projectile) || !validate_pointer(game))
		return ;
	if (!projectile->base.active)
		return ;
	
	/* Create trail effect occasionally */
	if (random_chance(0.3f))
		projectile_create_trail_effect(projectile, game);
	
	projectile->lifetime -= game->delta_time;
	if (projectile->lifetime <= 0.0f)
	{
		/* Create small explosion when projectile expires */
		effects_create_explosion(game, projectile->base.pos, 1);
		projectile_destroy(projectile);
		return ;
	}
	projectile_update_movement(projectile, game);
	projectile_check_collisions(projectile, game);
}

static void	projectile_update_movement(t_projectile *projectile, t_game *game)
{
	t_vector2	movement;

	movement = vec2_multiply(projectile->base.vel, game->delta_time);
	projectile->base.pos = vec2_add(projectile->base.pos, movement);
	if (projectile->base.pos.x < -50 || projectile->base.pos.x > WINDOW_WIDTH + 50
		|| projectile->base.pos.y < -50 || projectile->base.pos.y > WINDOW_HEIGHT + 50)
	{
		projectile_destroy(projectile);
	}
}

static void	projectile_check_collisions(t_projectile *projectile, t_game *game)
{
	t_enemy	*enemy;
	float	distance;
	int		i;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		enemy = &game->enemies[i];
		if (enemy->base.active)
		{
			distance = math_distance(projectile->base.pos, enemy->base.pos);
			if (distance <= (projectile->base.radius + enemy->base.radius))
			{
				projectile_apply_damage(projectile, enemy, game);
				if (!projectile->piercing)
				{
					/* Create impact effects */
					effects_create_explosion(game, projectile->base.pos, 2);
					effects_create_sparks(game, projectile->base.pos, 5);
					
					projectile_destroy(projectile);
					return ;
				}
			}
		}
		i++;
	}
}

static void	projectile_apply_damage(t_projectile *projectile, t_enemy *enemy, 
		t_game *game)
{
	debug_log("Projectile hit enemy: damage=%d", projectile->damage);
	
	/* Create blood effect on hit */
	effects_create_blood(game, enemy->base.pos);
	
	enemy_take_damage(enemy, projectile->damage, game);
	if (projectile->explosive)
	{
		projectile_explode(projectile, game);
	}
}

void	projectile_explode(t_projectile *projectile, t_game *game)
{
	t_enemy	*enemy;
	float	distance;
	int		explosion_damage;
	int		i;

	if (!validate_pointer(projectile) || !validate_pointer(game))
		return ;
	explosion_damage = projectile->damage / 2;
	
	/* Create massive explosion effect */
	effects_create_explosion(game, projectile->base.pos, 5);
	effects_create_sparks(game, projectile->base.pos, 15);
	effects_create_smoke(game, projectile->base.pos);
	
	debug_log("Projectile exploding: radius=%.1f, damage=%d", 
		projectile->explosion_radius, explosion_damage);
	i = 0;
	while (i < MAX_ENEMIES)
	{
		enemy = &game->enemies[i];
		if (enemy->base.active)
		{
			distance = math_distance(projectile->base.pos, enemy->base.pos);
			if (distance <= projectile->explosion_radius)
			{
				/* Create blood effect for each affected enemy */
				effects_create_blood(game, enemy->base.pos);
				enemy_take_damage(enemy, explosion_damage, game);
			}
		}
		i++;
	}
}

bool	projectile_hit_target(t_projectile *projectile, t_enemy *enemy)
{
	float	distance;

	if (!validate_pointer(projectile) || !validate_pointer(enemy))
		return (false);
	if (!projectile->base.active || !enemy->base.active)
		return (false);
	distance = math_distance(projectile->base.pos, enemy->base.pos);
	return (distance <= (projectile->base.radius + enemy->base.radius));
}

void	projectile_render(t_projectile *projectile, t_game *game)
{
	t_color	projectile_color;
	t_color	glow_color;

	if (!validate_pointer(projectile) || !validate_pointer(game))
		return ;
	if (!projectile->base.active)
		return ;
	
	/* Enhanced projectile rendering with glow effect */
	projectile_color = color_create(255, 255, 150, 255);
	glow_color = color_create(255, 200, 100, 100);
	
	/* Render glow */
	render_circle(game, projectile->base.pos, projectile->base.radius + 3, 
		glow_color);
	/* Render core */
	render_circle(game, projectile->base.pos, projectile->base.radius, 
		projectile_color);
}

void	projectile_destroy(t_projectile *projectile)
{
	if (!validate_pointer(projectile))
		return ;
	debug_log("Destroying projectile at (%.1f, %.1f)", 
		projectile->base.pos.x, projectile->base.pos.y);
	projectile->base.active = false;
}

static void	projectile_create_trail_effect(t_projectile *projectile, t_game *game)
{
	t_vector2	trail_pos;
	t_vector2	trail_vel;

	/* Create trail behind projectile */
	trail_pos.x = projectile->base.pos.x - projectile->base.vel.x * 0.01f;
	trail_pos.y = projectile->base.pos.y - projectile->base.vel.y * 0.01f;
	
	trail_vel = vec2_multiply(projectile->base.vel, -0.1f);
	trail_vel.x += random_float(-10.0f, 10.0f);
	trail_vel.y += random_float(-10.0f, 10.0f);
	
	/* Create a simple spark for trail */
	effects_create_sparks(game, trail_pos, 1);
}
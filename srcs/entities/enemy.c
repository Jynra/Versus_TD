/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 17:28:20 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"
#include "utils.h"
#include "game.h"

static void	enemy_update_movement(t_enemy *enemy, t_game *game);
static void	enemy_update_effects(t_enemy *enemy, t_game *game);
static void	enemy_render_health_bar(t_enemy *enemy, t_game *game);
static void	enemy_render_body(t_enemy *enemy, t_game *game);

void	enemy_init(t_enemy *enemy)
{
	if (!validate_pointer(enemy))
		return ;
	entity_init(&enemy->base);
	enemy->speed = 0.0f;
	enemy->base_speed = 0.0f;
	enemy->path_index = 0;
	enemy->reward = 0;
	enemy->score_value = 0;
	enemy->slowed = false;
	enemy->slow_timer = 0.0f;
	enemy->flying = false;
	enemy->spawn_time = 0.0f;
}

void	enemy_create(t_enemy *enemy, int type)
{
	if (!validate_pointer(enemy))
		return ;
	enemy_init(enemy);
	enemy->base.type = type;
	enemy->base.active = true;
	enemy->base.pos = vec2_create(-GRID_SIZE, GAME_AREA_HEIGHT / 2);
	enemy->base.radius = 12.0f;
	enemy->path_index = 0;
	enemy->spawn_time = time_get_ticks() / 1000.0f;
	enemy_set_stats(enemy, type);
	debug_info("Enemy created: type=%d, hp=%d, speed=%.1f", 
		type, enemy->base.health, enemy->speed);
}

void	enemy_set_stats(t_enemy *enemy, int type)
{
	if (!validate_pointer(enemy))
		return ;
	if (type == ENEMY_BASIC)
	{
		enemy->base.health = ENEMY_BASIC_HEALTH;
		enemy->base.max_health = ENEMY_BASIC_HEALTH;
		enemy->speed = ENEMY_BASIC_SPEED;
		enemy->reward = ENEMY_BASIC_REWARD;
		enemy->score_value = ENEMY_BASIC_SCORE;
		enemy->flying = false;
	}
	else if (type == ENEMY_FAST)
	{
		enemy->base.health = ENEMY_FAST_HEALTH;
		enemy->base.max_health = ENEMY_FAST_HEALTH;
		enemy->speed = ENEMY_FAST_SPEED;
		enemy->reward = ENEMY_FAST_REWARD;
		enemy->score_value = ENEMY_FAST_SCORE;
		enemy->flying = false;
	}
	else if (type == ENEMY_TANK)
	{
		enemy->base.health = ENEMY_TANK_HEALTH;
		enemy->base.max_health = ENEMY_TANK_HEALTH;
		enemy->speed = ENEMY_TANK_SPEED;
		enemy->reward = ENEMY_TANK_REWARD;
		enemy->score_value = ENEMY_TANK_SCORE;
		enemy->flying = false;
	}
	else if (type == ENEMY_FLYING)
	{
		enemy->base.health = ENEMY_FLYING_HEALTH;
		enemy->base.max_health = ENEMY_FLYING_HEALTH;
		enemy->speed = ENEMY_FLYING_SPEED;
		enemy->reward = ENEMY_FLYING_REWARD;
		enemy->score_value = ENEMY_FLYING_SCORE;
		enemy->flying = true;
	}
	enemy->base_speed = enemy->speed;
}

void	enemy_update(t_enemy *enemy, t_game *game)
{
	if (!validate_pointer(enemy) || !validate_pointer(game))
		return ;
	if (!enemy->base.active)
		return ;
	enemy_update_effects(enemy, game);
	enemy_update_movement(enemy, game);
	if (enemy_reached_end(enemy, game))
	{
		game_lose_life(game);
		enemy_destroy(enemy, game);
		debug_info("Enemy reached end - life lost");
	}
}

static void	enemy_update_movement(t_enemy *enemy, t_game *game)
{
	t_vector2	target;
	t_vector2	direction;
	t_vector2	movement;
	float		distance;
	float		current_speed;

	target = enemy_get_next_waypoint(enemy, game);
	direction = vec2_subtract(target, enemy->base.pos);
	distance = math_magnitude(direction);
	if (distance < 5.0f)
	{
		enemy->path_index++;
		if (enemy->path_index >= game->path_length)
			return ;
		target = enemy_get_next_waypoint(enemy, game);
		direction = vec2_subtract(target, enemy->base.pos);
	}
	direction = math_normalize(direction);
	current_speed = enemy->speed;
	if (enemy->slowed)
		current_speed *= TOWER_FREEZE_SLOW_FACTOR;
	movement = vec2_multiply(direction, current_speed * game->delta_time);
	enemy->base.pos = vec2_add(enemy->base.pos, movement);
	enemy->base.rotation = math_angle_between(vec2_zero(), direction);
}

static void	enemy_update_effects(t_enemy *enemy, t_game *game)
{
	if (enemy->slowed)
	{
		enemy->slow_timer -= game->delta_time;
		if (enemy->slow_timer <= 0.0f)
		{
			enemy->slowed = false;
			enemy->speed = enemy->base_speed;
		}
	}
}

t_vector2	enemy_get_next_waypoint(t_enemy *enemy, t_game *game)
{
	if (!validate_pointer(enemy) || !validate_pointer(game))
		return (vec2_zero());
	if (enemy->path_index >= game->path_length)
		return (game->path[game->path_length - 1].pos);
	return (game->path[enemy->path_index].pos);
}

bool	enemy_reached_end(t_enemy *enemy, t_game *game)
{
	if (!validate_pointer(enemy) || !validate_pointer(game))
		return (false);
	return (enemy->path_index >= game->path_length);
}

void	enemy_take_damage(t_enemy *enemy, int damage, t_game *game)
{
	if (!validate_pointer(enemy) || !validate_pointer(game))
		return ;
	if (!enemy->base.active || damage <= 0)
		return ;
	enemy->base.health -= damage;
	debug_log("Enemy took %d damage, health: %d/%d", 
		damage, enemy->base.health, enemy->base.max_health);
	if (enemy->base.health <= 0)
	{
		game_add_money(game, enemy->reward);
		game_add_score(game, enemy->score_value);
		enemy_destroy(enemy, game);
		debug_info("Enemy destroyed - reward: %d, score: %d", 
			enemy->reward, enemy->score_value);
	}
}

void	enemy_apply_slow(t_enemy *enemy, float duration, float factor)
{
	if (!validate_pointer(enemy))
		return ;
	if (!enemy->base.active)
		return ;
	enemy->slowed = true;
	enemy->slow_timer = duration;
	enemy->speed = enemy->base_speed * factor;
	debug_log("Enemy slowed: duration=%.1f, factor=%.2f", duration, factor);
}

void	enemy_destroy(t_enemy *enemy, t_game *game)
{
	if (!validate_pointer(enemy))
		return ;
	debug_log("Destroying enemy at (%.1f, %.1f)", 
		enemy->base.pos.x, enemy->base.pos.y);
	enemy->base.active = false;
	if (game)
		game->current_wave.enemies_alive--;
}

void	enemy_render(t_enemy *enemy, t_game *game)
{
	if (!validate_pointer(enemy) || !validate_pointer(game))
		return ;
	if (!enemy->base.active)
		return ;
	enemy_render_body(enemy, game);
	enemy_render_health_bar(enemy, game);
}

static void	enemy_render_body(t_enemy *enemy, t_game *game)
{
	t_color	body_color;
	t_color	outline_color;

	if (enemy->base.type == ENEMY_BASIC)
		body_color = color_create(255, 100, 100, 255);
	else if (enemy->base.type == ENEMY_FAST)
		body_color = color_create(100, 255, 100, 255);
	else if (enemy->base.type == ENEMY_TANK)
		body_color = color_create(100, 100, 255, 255);
	else if (enemy->base.type == ENEMY_FLYING)
		body_color = color_create(255, 255, 100, 255);
	else
		body_color = color_gray();
	outline_color = color_black();
	if (enemy->flying)
	{
		render_circle(game, enemy->base.pos, enemy->base.radius + 2, 
			outline_color);
		render_circle(game, enemy->base.pos, enemy->base.radius, body_color);
	}
	else
	{
		render_circle(game, enemy->base.pos, enemy->base.radius + 1, 
			outline_color);
		render_circle(game, enemy->base.pos, enemy->base.radius, body_color);
	}
	if (enemy->slowed)
	{
		render_circle(game, enemy->base.pos, enemy->base.radius + 4, 
			color_create(0, 255, 255, 128));
	}
}

static void	enemy_render_health_bar(t_enemy *enemy, t_game *game)
{
	t_rect		bg_rect;
	t_rect		health_rect;
	t_vector2	bar_pos;
	float		health_ratio;
	int			bar_width;
	int			bar_height;

	if (enemy->base.health >= enemy->base.max_health)
		return ;
	bar_width = (int)(enemy->base.radius * 2);
	bar_height = 4;
	bar_pos.x = enemy->base.pos.x - bar_width / 2;
	bar_pos.y = enemy->base.pos.y - enemy->base.radius - 8;
	bg_rect = rect_create((int)bar_pos.x, (int)bar_pos.y, bar_width, bar_height);
	health_ratio = (float)enemy->base.health / (float)enemy->base.max_health;
	health_rect = rect_create((int)bar_pos.x, (int)bar_pos.y, 
		(int)(bar_width * health_ratio), bar_height);
	render_rect(game, bg_rect, color_create(255, 0, 0, 255));
	render_rect(game, health_rect, color_create(0, 255, 0, 255));
}
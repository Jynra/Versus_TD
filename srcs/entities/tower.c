/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tower.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 20:02:22 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"
#include "utils.h"
#include "game.h"

static void	tower_update_targeting(t_tower *tower, t_game *game);
static void	tower_update_firing(t_tower *tower, t_game *game);
static void	tower_render_base(t_tower *tower, t_game *game);
static void	tower_render_cannon(t_tower *tower, t_game *game);
static void	tower_create_firing_effects(t_tower *tower, t_game *game);

void	tower_init(t_tower *tower)
{
	if (!validate_pointer(tower))
		return ;
	entity_init(&tower->base);
	tower->range = 0.0f;
	tower->damage = 0;
	tower->fire_rate = 0.0f;
	tower->fire_timer = 0.0f;
	tower->level = 1;
	tower->kills = 0;
	tower->cost = 0;
	tower->total_damage_dealt = 0;
	tower->selected = false;
	tower->upgrade_cost = 0;
}

void	tower_create(t_tower *tower, t_vector2 pos, int type)
{
	if (!validate_pointer(tower))
		return ;
	tower_init(tower);
	tower->base.type = type;
	tower->base.active = true;
	tower->base.pos = pos;
	tower->base.radius = 16.0f;
	tower->base.health = 100;
	tower->base.max_health = 100;
	tower->fire_timer = 0.0f;
	tower_set_stats(tower, type);
	debug_info("Tower created: type=%d, pos=(%.1f,%.1f), range=%.1f", 
		type, pos.x, pos.y, tower->range);
}

void	tower_set_stats(t_tower *tower, int type)
{
	if (!validate_pointer(tower))
		return ;
	if (type == TOWER_BASIC)
	{
		tower->damage = TOWER_BASIC_DAMAGE;
		tower->range = TOWER_BASIC_RANGE;
		tower->fire_rate = TOWER_BASIC_FIRE_RATE;
		tower->cost = TOWER_BASIC_COST;
		tower->upgrade_cost = TOWER_BASIC_COST * 2;
	}
	else if (type == TOWER_SNIPER)
	{
		tower->damage = TOWER_SNIPER_DAMAGE;
		tower->range = TOWER_SNIPER_RANGE;
		tower->fire_rate = TOWER_SNIPER_FIRE_RATE;
		tower->cost = TOWER_SNIPER_COST;
		tower->upgrade_cost = TOWER_SNIPER_COST * 2;
	}
	else if (type == TOWER_CANNON)
	{
		tower->damage = TOWER_CANNON_DAMAGE;
		tower->range = TOWER_CANNON_RANGE;
		tower->fire_rate = TOWER_CANNON_FIRE_RATE;
		tower->cost = TOWER_CANNON_COST;
		tower->upgrade_cost = TOWER_CANNON_COST * 2;
	}
	else if (type == TOWER_FREEZE)
	{
		tower->damage = TOWER_FREEZE_DAMAGE;
		tower->range = TOWER_FREEZE_RANGE;
		tower->fire_rate = TOWER_FREEZE_FIRE_RATE;
		tower->cost = TOWER_FREEZE_COST;
		tower->upgrade_cost = TOWER_FREEZE_COST * 2;
	}
}

void	tower_update(t_tower *tower, t_game *game)
{
	if (!validate_pointer(tower) || !validate_pointer(game))
		return ;
	if (!tower->base.active)
		return ;
	tower_update_targeting(tower, game);
	tower_update_firing(tower, game);
}

static void	tower_update_targeting(t_tower *tower, t_game *game)
{
	t_enemy	*target;

	target = tower_find_target(tower, game);
	if (target)
	{
		tower->base.rotation = math_angle_between(tower->base.pos, 
			target->base.pos);
	}
}

static void	tower_update_firing(t_tower *tower, t_game *game)
{
	t_enemy	*target;

	if (tower->fire_timer > 0.0f)
		tower->fire_timer -= game->delta_time;
	if (tower->fire_timer <= 0.0f)
	{
		target = tower_find_target(tower, game);
		if (target)
		{
			tower_fire(tower, game);
			tower->fire_timer = 1.0f / tower->fire_rate;
		}
	}
}

void	tower_fire(t_tower *tower, t_game *game)
{
	t_enemy		*target;
	t_vector2	projectile_pos;
	t_vector2	cannon_offset;

	if (!validate_pointer(tower) || !validate_pointer(game))
		return ;
	target = tower_find_target(tower, game);
	if (!target)
		return ;
	cannon_offset.x = cosf(tower->base.rotation) * tower->base.radius;
	cannon_offset.y = sinf(tower->base.rotation) * tower->base.radius;
	projectile_pos = vec2_add(tower->base.pos, cannon_offset);
	tower_create_firing_effects(tower, game);
	if (tower->base.type == TOWER_FREEZE)
	{
		enemy_apply_slow(target, TOWER_FREEZE_DURATION, 
			TOWER_FREEZE_SLOW_FACTOR);
		enemy_take_damage(target, tower->damage, game);
		tower->total_damage_dealt += tower->damage;
		debug_log("Freeze tower fired: target slowed and damaged");
	}
	else
	{
		if (spawn_projectile(game, projectile_pos, target->base.pos, 
			tower->damage))
		{
			debug_log("Tower fired projectile: damage=%d", tower->damage);
		}
	}
}

static void	tower_create_firing_effects(t_tower *tower, t_game *game)
{
	t_vector2	muzzle_pos;
	t_vector2	cannon_offset;

	cannon_offset.x = cosf(tower->base.rotation) * (tower->base.radius + 8.0f);
	cannon_offset.y = sinf(tower->base.rotation) * (tower->base.radius + 8.0f);
	muzzle_pos = vec2_add(tower->base.pos, cannon_offset);
	if (tower->base.type == TOWER_BASIC)
	{
		effects_create_sparks(game, muzzle_pos, 3);
		effects_create_smoke(game, muzzle_pos);
	}
	else if (tower->base.type == TOWER_SNIPER)
	{
		effects_create_sparks(game, muzzle_pos, 8);
		effects_create_explosion(game, muzzle_pos, 1);
		effects_create_smoke(game, muzzle_pos);
	}
	else if (tower->base.type == TOWER_CANNON)
	{
		effects_create_explosion(game, muzzle_pos, 3);
		effects_create_sparks(game, muzzle_pos, 12);
		effects_create_smoke(game, muzzle_pos);
		effects_create_smoke(game, muzzle_pos);
	}
}

t_enemy	*tower_find_target(t_tower *tower, t_game *game)
{
	t_enemy	*closest_enemy;
	t_enemy	*current_enemy;
	float	closest_distance;
	float	current_distance;
	int		i;

	if (!validate_pointer(tower) || !validate_pointer(game))
		return (NULL);
	closest_enemy = NULL;
	closest_distance = tower->range + 1.0f;
	i = 0;
	while (i < MAX_ENEMIES)
	{
		current_enemy = &game->enemies[i];
		if (current_enemy->base.active)
		{
			current_distance = math_distance(tower->base.pos, 
				current_enemy->base.pos);
			if (current_distance <= tower->range 
				&& current_distance < closest_distance)
			{
				closest_enemy = current_enemy;
				closest_distance = current_distance;
			}
		}
		i++;
	}
	return (closest_enemy);
}

bool	tower_in_range(t_tower *tower, t_enemy *enemy)
{
	float	distance;

	if (!validate_pointer(tower) || !validate_pointer(enemy))
		return (false);
	if (!tower->base.active || !enemy->base.active)
		return (false);
	distance = math_distance(tower->base.pos, enemy->base.pos);
	return (distance <= tower->range);
}

void	tower_render(t_tower *tower, t_game *game)
{
	if (!validate_pointer(tower) || !validate_pointer(game))
		return ;
	if (!tower->base.active)
		return ;
	tower_render_base(tower, game);
	tower_render_cannon(tower, game);
	if (tower->selected || game->show_ranges)
	{
		render_circle(game, tower->base.pos, tower->range, 
			color_create(255, 255, 255, 64));
	}
}

static void	tower_render_base(t_tower *tower, t_game *game)
{
	t_color	base_color;
	t_color	outline_color;
	t_color	glow_color;

	if (tower->base.type == TOWER_BASIC)
		base_color = color_create(100, 100, 100, 255);
	else if (tower->base.type == TOWER_SNIPER)
		base_color = color_create(150, 100, 50, 255);
	else if (tower->base.type == TOWER_CANNON)
		base_color = color_create(80, 80, 80, 255);
	else if (tower->base.type == TOWER_FREEZE)
		base_color = color_create(100, 150, 255, 255);
	else
		base_color = color_gray();
	outline_color = color_black();
	if (tower->selected)
	{
		outline_color = color_create(255, 255, 0, 255);
		glow_color = color_create(255, 255, 0, 100);
		render_circle(game, tower->base.pos, tower->base.radius + 4, glow_color);
	}
	render_circle(game, tower->base.pos, tower->base.radius + 2, outline_color);
	render_circle(game, tower->base.pos, tower->base.radius, base_color);
	if (tower->level > 1)
	{
		t_color level_color = color_create(255, 215, 0, 255);
		int i = 0;
		while (i < tower->level - 1)
		{
			render_circle(game, vec2_create(tower->base.pos.x - 8 + i * 4, 
				tower->base.pos.y - tower->base.radius - 6), 2, level_color);
			i++;
		}
	}
}

static void	tower_render_cannon(t_tower *tower, t_game *game)
{
	t_vector2	cannon_start;
	t_vector2	cannon_end;
	t_vector2	cannon_offset;
	t_color		cannon_color;
	float		cannon_length;

	if (tower->base.type == TOWER_SNIPER)
	{
		cannon_length = tower->base.radius + 8.0f;
		cannon_color = color_create(100, 50, 25, 255);
	}
	else if (tower->base.type == TOWER_CANNON)
	{
		cannon_length = tower->base.radius + 6.0f;
		cannon_color = color_create(60, 60, 60, 255);
	}
	else if (tower->base.type == TOWER_FREEZE)
	{
		cannon_length = tower->base.radius + 5.0f;
		cannon_color = color_create(150, 200, 255, 255);
	}
	else
	{
		cannon_length = tower->base.radius + 4.0f;
		cannon_color = color_create(80, 80, 80, 255);
	}
	cannon_offset.x = cosf(tower->base.rotation) * tower->base.radius * 0.5f;
	cannon_offset.y = sinf(tower->base.rotation) * tower->base.radius * 0.5f;
	cannon_start = vec2_add(tower->base.pos, cannon_offset);
	cannon_offset.x = cosf(tower->base.rotation) * cannon_length;
	cannon_offset.y = sinf(tower->base.rotation) * cannon_length;
	cannon_end = vec2_add(tower->base.pos, cannon_offset);
	render_line(game, cannon_start, cannon_end, cannon_color);
}

bool	tower_can_upgrade(t_tower *tower, t_game *game)
{
	if (!validate_pointer(tower) || !validate_pointer(game))
		return (false);
	if (!tower->base.active || tower->level >= TOWER_MAX_LEVEL)
		return (false);
	return (game->money >= tower->upgrade_cost);
}

void	tower_upgrade(t_tower *tower)
{
	if (!validate_pointer(tower))
		return ;
	if (tower->level >= TOWER_MAX_LEVEL)
		return ;
	tower->level++;
	tower->damage = (int)(tower->damage * 1.5f);
	tower->range *= 1.2f;
	tower->fire_rate *= 1.1f;
	tower->upgrade_cost = (int)(tower->upgrade_cost * 
		TOWER_UPGRADE_COST_MULTIPLIER);
	debug_info("Tower upgraded to level %d: damage=%d, range=%.1f", 
		tower->level, tower->damage, tower->range);
}

int	tower_get_upgrade_cost(t_tower *tower)
{
	if (!validate_pointer(tower))
		return (0);
	return (tower->upgrade_cost);
}

void	tower_destroy(t_tower *tower)
{
	if (!validate_pointer(tower))
		return ;
	debug_log("Destroying tower at (%.1f, %.1f)", 
		tower->base.pos.x, tower->base.pos.y);
	tower->base.active = false;
	tower->selected = false;
}
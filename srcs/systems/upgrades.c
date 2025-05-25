/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upgrades.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:30:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 21:58:28 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"
#include "game.h"

static void	upgrade_apply_basic_stats(t_tower *tower);
static void	upgrade_apply_sniper_stats(t_tower *tower);
static void	upgrade_apply_cannon_stats(t_tower *tower);
static void	upgrade_apply_freeze_stats(t_tower *tower);
static void	upgrade_create_visual_effect(t_game *game, t_tower *tower);

void	upgrades_init(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
		{
			game->towers[i].level = 1;
			game->towers[i].upgrade_cost = game->towers[i].cost * 2;
		}
		i++;
	}
	debug_info("Upgrade system initialized");
}

bool	upgrades_tower(t_game *game, int tower_id)
{
	t_tower	*tower;

	if (!validate_pointer(game))
		return (false);
	if (tower_id < 0 || tower_id >= MAX_TOWERS)
		return (false);
	tower = &game->towers[tower_id];
	if (!tower->base.active)
		return (false);
	if (tower->level >= TOWER_MAX_LEVEL)
	{
		debug_warning("Tower already at max level");
		return (false);
	}
	if (!upgrades_can_afford(game, tower))
	{
		debug_warning("Not enough money to upgrade tower");
		return (false);
	}
	if (!game_spend_money(game, tower->upgrade_cost))
		return (false);
	tower->level++;
	upgrades_apply_stats(tower);
	upgrade_create_visual_effect(game, tower);
	debug_info("Tower upgraded to level %d", tower->level);
	return (true);
}

int	upgrades_get_cost(t_tower *tower)
{
	if (!validate_pointer(tower))
		return (0);
	if (tower->level >= TOWER_MAX_LEVEL)
		return (0);
	return (tower->upgrade_cost);
}

void	upgrades_apply_stats(t_tower *tower)
{
	if (!validate_pointer(tower))
		return ;
	if (tower->base.type == TOWER_BASIC)
		upgrade_apply_basic_stats(tower);
	else if (tower->base.type == TOWER_SNIPER)
		upgrade_apply_sniper_stats(tower);
	else if (tower->base.type == TOWER_CANNON)
		upgrade_apply_cannon_stats(tower);
	else if (tower->base.type == TOWER_FREEZE)
		upgrade_apply_freeze_stats(tower);
	tower->upgrade_cost = (int)(tower->upgrade_cost * 
		TOWER_UPGRADE_COST_MULTIPLIER);
}

bool	upgrades_can_afford(t_game *game, t_tower *tower)
{
	if (!validate_pointer(game) || !validate_pointer(tower))
		return (false);
	if (tower->level >= TOWER_MAX_LEVEL)
		return (false);
	return (game->money >= tower->upgrade_cost);
}

static void	upgrade_apply_basic_stats(t_tower *tower)
{
	float	damage_multiplier;
	float	range_multiplier;
	float	rate_multiplier;

	if (tower->level == 2)
	{
		damage_multiplier = 1.5f;
		range_multiplier = 1.2f;
		rate_multiplier = 1.3f;
	}
	else if (tower->level == 3)
	{
		damage_multiplier = 2.0f;
		range_multiplier = 1.4f;
		rate_multiplier = 1.6f;
	}
	else
		return ;
	tower->damage = (int)(TOWER_BASIC_DAMAGE * damage_multiplier);
	tower->range = TOWER_BASIC_RANGE * range_multiplier;
	tower->fire_rate = TOWER_BASIC_FIRE_RATE * rate_multiplier;
	debug_log("Basic tower upgraded: damage=%d, range=%.1f, rate=%.2f", 
		tower->damage, tower->range, tower->fire_rate);
}

static void	upgrade_apply_sniper_stats(t_tower *tower)
{
	float	damage_multiplier;
	float	range_multiplier;
	float	rate_multiplier;

	if (tower->level == 2)
	{
		damage_multiplier = 1.8f;
		range_multiplier = 1.3f;
		rate_multiplier = 1.2f;
	}
	else if (tower->level == 3)
	{
		damage_multiplier = 2.5f;
		range_multiplier = 1.5f;
		rate_multiplier = 1.4f;
	}
	else
		return ;
	tower->damage = (int)(TOWER_SNIPER_DAMAGE * damage_multiplier);
	tower->range = TOWER_SNIPER_RANGE * range_multiplier;
	tower->fire_rate = TOWER_SNIPER_FIRE_RATE * rate_multiplier;
	debug_log("Sniper tower upgraded: damage=%d, range=%.1f, rate=%.2f", 
		tower->damage, tower->range, tower->fire_rate);
}

static void	upgrade_apply_cannon_stats(t_tower *tower)
{
	float	damage_multiplier;
	float	range_multiplier;
	float	rate_multiplier;

	if (tower->level == 2)
	{
		damage_multiplier = 1.6f;
		range_multiplier = 1.1f;
		rate_multiplier = 1.4f;
		tower->explosive = true;
		tower->explosion_radius = 25.0f;
	}
	else if (tower->level == 3)
	{
		damage_multiplier = 2.2f;
		range_multiplier = 1.2f;
		rate_multiplier = 1.7f;
		tower->explosive = true;
		tower->explosion_radius = 35.0f;
	}
	else
		return ;
	tower->damage = (int)(TOWER_CANNON_DAMAGE * damage_multiplier);
	tower->range = TOWER_CANNON_RANGE * range_multiplier;
	tower->fire_rate = TOWER_CANNON_FIRE_RATE * rate_multiplier;
	debug_log("Cannon tower upgraded: damage=%d, range=%.1f, explosive=%.1f", 
		tower->damage, tower->range, tower->explosion_radius);
}

static void	upgrade_apply_freeze_stats(t_tower *tower)
{
	float	damage_multiplier;
	float	range_multiplier;
	float	rate_multiplier;

	if (tower->level == 2)
	{
		damage_multiplier = 1.4f;
		range_multiplier = 1.3f;
		rate_multiplier = 1.3f;
	}
	else if (tower->level == 3)
	{
		damage_multiplier = 1.8f;
		range_multiplier = 1.5f;
		rate_multiplier = 1.5f;
	}
	else
		return ;
	tower->damage = (int)(TOWER_FREEZE_DAMAGE * damage_multiplier);
	tower->range = TOWER_FREEZE_RANGE * range_multiplier;
	tower->fire_rate = TOWER_FREEZE_FIRE_RATE * rate_multiplier;
	debug_log("Freeze tower upgraded: damage=%d, range=%.1f, rate=%.2f", 
		tower->damage, tower->range, tower->fire_rate);
}

static void	upgrade_create_visual_effect(t_game *game, t_tower *tower)
{
	if (!validate_pointer(game) || !validate_pointer(tower))
		return ;
	effects_create_explosion(game, tower->base.pos, 3);
	effects_create_sparks(game, tower->base.pos, 15);
	if (tower->base.type == TOWER_FREEZE)
		effects_create_freeze_aura(game, tower->base.pos, tower->range * 0.8f);
	else
		effects_create_smoke(game, tower->base.pos);
}

bool	upgrades_handle_tower_click(t_game *game, t_tower *tower)
{
	if (!validate_pointer(game) || !validate_pointer(tower))
		return (false);
	if (!tower->base.active)
		return (false);
	if (tower->level >= TOWER_MAX_LEVEL)
	{
		debug_info("Tower is already at maximum level");
		return (false);
	}
	if (!upgrades_can_afford(game, tower))
	{
		debug_info("Not enough money to upgrade tower (cost: $%d)", 
			tower->upgrade_cost);
		return (false);
	}
	return (upgrades_tower(game, tower - game->towers));
}

void	upgrades_get_tower_info(t_tower *tower, char *info_text, size_t max_len)
{
	if (!validate_pointer(tower) || !validate_pointer(info_text))
		return ;
	if (tower->level >= TOWER_MAX_LEVEL)
	{
		snprintf(info_text, max_len, "MAX LEVEL\nDamage: %d\nRange: %.0f", 
			tower->damage, tower->range);
	}
	else
	{
		snprintf(info_text, max_len, 
			"Level %d\nDamage: %d\nRange: %.0f\nUpgrade: $%d", 
			tower->level, tower->damage, tower->range, tower->upgrade_cost);
	}
}

float	upgrades_get_damage_multiplier(int level)
{
	if (level <= 1)
		return (1.0f);
	else if (level == 2)
		return (1.5f);
	else if (level >= 3)
		return (2.0f);
	return (1.0f);
}

float	upgrades_get_range_multiplier(int level)
{
	if (level <= 1)
		return (1.0f);
	else if (level == 2)
		return (1.2f);
	else if (level >= 3)
		return (1.4f);
	return (1.0f);
}

float	upgrades_get_rate_multiplier(int level)
{
	if (level <= 1)
		return (1.0f);
	else if (level == 2)
		return (1.3f);
	else if (level >= 3)
		return (1.6f);
	return (1.0f);
}

bool	upgrades_is_special_ability_unlocked(t_tower *tower)
{
	if (!validate_pointer(tower))
		return (false);
	if (tower->base.type == TOWER_CANNON && tower->level >= 2)
		return (true);
	if (tower->base.type == TOWER_SNIPER && tower->level >= 3)
		return (true);
	return (false);
}
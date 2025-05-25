/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:30:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 19:30:59 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"
#include "game.h"

static void	waves_create_boss_wave(t_wave *wave);
static void	waves_create_rush_wave(t_wave *wave);
static void	waves_create_flying_wave(t_wave *wave);
static void	waves_create_mixed_wave(t_wave *wave);
static bool	waves_is_boss_wave(int wave_number);
static void	waves_give_completion_bonus(t_game *game, t_wave *wave);

void	waves_init(t_wave *wave)
{
	int	i;

	if (!validate_pointer(wave))
		return ;
	wave->number = 1;
	wave->enemies_total = WAVE_BASE_ENEMIES;
	wave->enemies_spawned = 0;
	wave->enemies_alive = 0;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE;
	wave->spawn_timer = 0.0f;
	wave->prep_timer = WAVE_PREP_TIME;
	wave->active = false;
	wave->completed = false;
	wave->preparing = true;
	wave->current_spawn_type = 0;
	i = 0;
	while (i < 4)
	{
		wave->enemy_types[i] = ENEMY_BASIC;
		wave->enemy_counts[i] = 0;
		i++;
	}
	waves_calculate_spawn_data(wave);
	debug_info("Advanced wave system initialized");
}

void	waves_update(t_wave *wave, t_game *game)
{
	if (!validate_pointer(wave) || !validate_pointer(game))
		return ;
	if (wave->preparing)
	{
		wave->prep_timer -= game->delta_time;
		if (wave->prep_timer <= 0.0f)
		{
			wave->preparing = false;
			wave->active = true;
			debug_info("Wave %d started! Type: %s", wave->number,
				waves_is_boss_wave(wave->number) ? "BOSS" : "Normal");
		}
		return ;
	}
	if (!wave->active || wave->completed)
		return ;
	if (wave->enemies_spawned >= wave->enemies_total)
	{
		if (waves_all_enemies_dead(game))
		{
			wave->completed = true;
			wave->active = false;
			waves_give_completion_bonus(game, wave);
			waves_start_next(wave, game);
		}
		return ;
	}
	wave->spawn_timer -= game->delta_time;
	if (wave->spawn_timer <= 0.0f)
	{
		waves_spawn_enemy(wave, game);
		wave->spawn_timer = wave->spawn_delay;
	}
}

void	waves_start_next(t_wave *wave, t_game *game)
{
	if (!validate_pointer(wave) || !validate_pointer(game))
		return ;
	wave->number++;
	wave->enemies_spawned = 0;
	wave->enemies_alive = 0;
	wave->spawn_timer = wave->spawn_delay;
	wave->prep_timer = WAVE_PREP_TIME;
	wave->active = false;
	wave->completed = false;
	wave->preparing = true;
	wave->current_spawn_type = 0;
	waves_calculate_spawn_data(wave);
	debug_info("Preparing wave %d: %d enemies", wave->number, 
		wave->enemies_total);
}

void	waves_spawn_enemy(t_wave *wave, t_game *game)
{
	int	enemy_type;
	int	type_index;

	if (!validate_pointer(wave) || !validate_pointer(game))
		return ;
	type_index = wave->current_spawn_type;
	while (type_index < 4 && wave->enemy_counts[type_index] <= 0)
		type_index++;
	if (type_index >= 4)
	{
		debug_warning("No more enemies to spawn in current wave");
		return ;
	}
	enemy_type = wave->enemy_types[type_index];
	if (spawn_enemy(game, enemy_type))
	{
		wave->enemies_spawned++;
		wave->enemies_alive++;
		wave->enemy_counts[type_index]--;
		if (wave->enemy_counts[type_index] <= 0)
			wave->current_spawn_type = type_index + 1;
		debug_log("Wave %d: Spawned enemy type %d (%d/%d)", 
			wave->number, enemy_type, wave->enemies_spawned, 
			wave->enemies_total);
	}
}

bool	waves_is_complete(t_wave *wave, t_game *game)
{
	if (!validate_pointer(wave))
		return (false);
	return (wave->completed && waves_all_enemies_dead(game));
}

bool	waves_all_enemies_dead(t_game *game)
{
	if (!validate_pointer(game))
		return (true);
	return (entities_count_active_enemies(game) == 0);
}

void	waves_calculate_spawn_data(t_wave *wave)
{
	if (!validate_pointer(wave))
		return ;
	if (waves_is_boss_wave(wave->number))
		waves_create_boss_wave(wave);
	else if (wave->number % 7 == 0)
		waves_create_flying_wave(wave);
	else if (wave->number % 4 == 0)
		waves_create_rush_wave(wave);
	else
		waves_create_mixed_wave(wave);
}

static void	waves_create_boss_wave(t_wave *wave)
{
	wave->enemies_total = 5 + (wave->number / 5) * 2;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE * 2.0f;
	wave->enemy_types[0] = ENEMY_TANK;
	wave->enemy_counts[0] = wave->enemies_total / 2;
	wave->enemy_types[1] = ENEMY_BASIC;
	wave->enemy_counts[1] = wave->enemies_total / 3;
	wave->enemy_types[2] = ENEMY_FAST;
	wave->enemy_counts[2] = wave->enemies_total - wave->enemy_counts[0] 
		- wave->enemy_counts[1];
	wave->enemy_types[3] = ENEMY_FLYING;
	wave->enemy_counts[3] = 0;
	debug_info("Boss wave %d: %d tanks, %d basic, %d fast", 
		wave->number, wave->enemy_counts[0], wave->enemy_counts[1], 
		wave->enemy_counts[2]);
}

static void	waves_create_rush_wave(t_wave *wave)
{
	wave->enemies_total = WAVE_BASE_ENEMIES + (wave->number - 1) * 4;
	wave->spawn_delay = WAVE_SPAWN_DELAY_MIN;
	wave->enemy_types[0] = ENEMY_FAST;
	wave->enemy_counts[0] = wave->enemies_total * 2 / 3;
	wave->enemy_types[1] = ENEMY_BASIC;
	wave->enemy_counts[1] = wave->enemies_total / 3;
	wave->enemy_types[2] = ENEMY_TANK;
	wave->enemy_counts[2] = 0;
	wave->enemy_types[3] = ENEMY_FLYING;
	wave->enemy_counts[3] = 0;
	debug_info("Rush wave %d: %d fast enemies incoming fast!", 
		wave->number, wave->enemy_counts[0]);
}

static void	waves_create_flying_wave(t_wave *wave)
{
	wave->enemies_total = WAVE_BASE_ENEMIES + (wave->number - 1) * 2;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE * 0.8f;
	wave->enemy_types[0] = ENEMY_FLYING;
	wave->enemy_counts[0] = wave->enemies_total / 2;
	wave->enemy_types[1] = ENEMY_FAST;
	wave->enemy_counts[1] = wave->enemies_total / 3;
	wave->enemy_types[2] = ENEMY_BASIC;
	wave->enemy_counts[2] = wave->enemies_total - wave->enemy_counts[0] 
		- wave->enemy_counts[1];
	wave->enemy_types[3] = ENEMY_TANK;
	wave->enemy_counts[3] = 0;
	debug_info("Flying wave %d: %d flying enemies!", 
		wave->number, wave->enemy_counts[0]);
}

static void	waves_create_mixed_wave(t_wave *wave)
{
	int	base_count;
	int	increment;

	base_count = WAVE_BASE_ENEMIES;
	increment = (wave->number - 1) * WAVE_ENEMY_INCREMENT;
	wave->enemies_total = base_count + increment;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE - (wave->number - 1) * 0.05f;
	if (wave->spawn_delay < WAVE_SPAWN_DELAY_MIN)
		wave->spawn_delay = WAVE_SPAWN_DELAY_MIN;
	if (wave->number <= 3)
	{
		wave->enemy_types[0] = ENEMY_BASIC;
		wave->enemy_counts[0] = wave->enemies_total;
		wave->enemy_counts[1] = 0;
		wave->enemy_counts[2] = 0;
		wave->enemy_counts[3] = 0;
	}
	else if (wave->number <= 8)
	{
		wave->enemy_types[0] = ENEMY_BASIC;
		wave->enemy_counts[0] = wave->enemies_total * 2 / 3;
		wave->enemy_types[1] = ENEMY_FAST;
		wave->enemy_counts[1] = wave->enemies_total / 3;
		wave->enemy_counts[2] = 0;
		wave->enemy_counts[3] = 0;
	}
	else if (wave->number <= 15)
	{
		wave->enemy_types[0] = ENEMY_BASIC;
		wave->enemy_counts[0] = wave->enemies_total / 2;
		wave->enemy_types[1] = ENEMY_FAST;
		wave->enemy_counts[1] = wave->enemies_total / 3;
		wave->enemy_types[2] = ENEMY_TANK;
		wave->enemy_counts[2] = wave->enemies_total / 6;
		wave->enemy_counts[3] = 0;
	}
	else
	{
		wave->enemy_types[0] = ENEMY_BASIC;
		wave->enemy_counts[0] = wave->enemies_total / 3;
		wave->enemy_types[1] = ENEMY_FAST;
		wave->enemy_counts[1] = wave->enemies_total / 3;
		wave->enemy_types[2] = ENEMY_TANK;
		wave->enemy_counts[2] = wave->enemies_total / 4;
		wave->enemy_types[3] = ENEMY_FLYING;
		wave->enemy_counts[3] = wave->enemies_total - wave->enemy_counts[0] 
			- wave->enemy_counts[1] - wave->enemy_counts[2];
	}
}

static bool	waves_is_boss_wave(int wave_number)
{
	return (wave_number % 5 == 0 && wave_number > 0);
}

static void	waves_give_completion_bonus(t_game *game, t_wave *wave)
{
	int	bonus_money;
	int	bonus_score;

	if (!validate_pointer(game) || !validate_pointer(wave))
		return ;
	if (waves_is_boss_wave(wave->number))
	{
		bonus_money = 50 + wave->number * 5;
		bonus_score = 500 + wave->number * 50;
		debug_info("Boss wave completed! Bonus: $%d, %d points", 
			bonus_money, bonus_score);
	}
	else
	{
		bonus_money = 20 + wave->number * 2;
		bonus_score = 100 + wave->number * 25;
	}
	game_add_money(game, bonus_money);
	game_add_score(game, bonus_score);
	if (wave->number % 10 == 0)
	{
		debug_info("Milestone reached! Wave %d completed!", wave->number);
		game_add_money(game, 100);
		game_add_score(game, 1000);
	}
}

int	waves_get_enemy_type_for_wave(int wave_number, int spawn_index)
{
	if (wave_number <= 3)
		return (ENEMY_BASIC);
	if (wave_number <= 6)
		return ((spawn_index % 3 == 0) ? ENEMY_FAST : ENEMY_BASIC);
	if (wave_number <= 10)
	{
		if (spawn_index % 4 == 0)
			return (ENEMY_TANK);
		else if (spawn_index % 3 == 0)
			return (ENEMY_FAST);
		else
			return (ENEMY_BASIC);
	}
	int rand_val = random_int(1, 100);
	if (rand_val <= 30)
		return (ENEMY_BASIC);
	else if (rand_val <= 55)
		return (ENEMY_FAST);
	else if (rand_val <= 80)
		return (ENEMY_TANK);
	else
		return (ENEMY_FLYING);
}

bool	waves_should_show_warning(t_wave *wave)
{
	if (!validate_pointer(wave))
		return (false);
	return (waves_is_boss_wave(wave->number) && wave->preparing && 
		wave->prep_timer > 2.0f);
}

float	waves_get_difficulty_multiplier(int wave_number)
{
	if (wave_number <= 5)
		return (1.0f);
	else if (wave_number <= 10)
		return (1.2f);
	else if (wave_number <= 20)
		return (1.5f);
	else
		return (2.0f);
}

void	waves_apply_difficulty_scaling(t_game *game, int wave_number)
{
	float	multiplier;
	int		i;

	if (!validate_pointer(game))
		return ;
	multiplier = waves_get_difficulty_multiplier(wave_number);
	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].base.active)
		{
			game->enemies[i].base.max_health = (int)(game->enemies[i].base.max_health * multiplier);
			game->enemies[i].base.health = game->enemies[i].base.max_health;
			game->enemies[i].speed *= (1.0f + (multiplier - 1.0f) * 0.5f);
		}
		i++;
	}
}
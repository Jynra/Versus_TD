/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 17:28:29 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entities.h"
#include "utils.h"
#include "game.h"

static int	spawner_get_enemy_type_for_wave(int wave_number);
static void	spawner_prepare_next_wave(t_game *game);
static bool	spawner_can_spawn(t_game *game);

bool	spawn_enemy(t_game *game, int type)
{
	int	enemy_id;

	if (!validate_pointer(game))
		return (false);
	enemy_id = entities_find_free_enemy(game);
	if (enemy_id == -1)
	{
		debug_warning("No free enemy slot available");
		return (false);
	}
	enemy_create(&game->enemies[enemy_id], type);
	game->current_wave.enemies_spawned++;
	game->current_wave.enemies_alive++;
	debug_info("Enemy spawned: type=%d, id=%d, wave=%d/%d", 
		type, enemy_id, game->current_wave.enemies_spawned, 
		game->current_wave.enemies_total);
	return (true);
}

void	spawner_update(t_game *game)
{
	t_wave	*wave;
	int		enemy_type;

	if (!validate_pointer(game))
		return ;
	wave = &game->current_wave;
	if (wave->preparing)
	{
		wave->prep_timer -= game->delta_time;
		if (wave->prep_timer <= 0.0f)
		{
			wave->preparing = false;
			wave->active = true;
			debug_info("Wave %d started!", wave->number);
		}
		return ;
	}
	if (!wave->active || wave->completed)
		return ;
	if (wave->enemies_spawned >= wave->enemies_total)
	{
		if (wave->enemies_alive <= 0)
		{
			wave->completed = true;
			wave->active = false;
			debug_info("Wave %d completed!", wave->number);
			spawner_prepare_next_wave(game);
		}
		return ;
	}
	wave->spawn_timer -= game->delta_time;
	if (wave->spawn_timer <= 0.0f && spawner_can_spawn(game))
	{
		enemy_type = spawner_get_enemy_type_for_wave(wave->number);
		if (spawn_enemy(game, enemy_type))
		{
			wave->spawn_timer = wave->spawn_delay;
		}
	}
}

static bool	spawner_can_spawn(t_game *game)
{
	int	active_enemies;

	active_enemies = entities_count_active_enemies(game);
	return (active_enemies < MAX_ENEMIES - 5);
}

static int	spawner_get_enemy_type_for_wave(int wave_number)
{
	int	rand_val;

	if (wave_number <= 3)
		return (ENEMY_BASIC);
	if (wave_number <= 6)
	{
		rand_val = random_int(1, 100);
		if (rand_val <= 70)
			return (ENEMY_BASIC);
		else
			return (ENEMY_FAST);
	}
	if (wave_number <= 10)
	{
		rand_val = random_int(1, 100);
		if (rand_val <= 50)
			return (ENEMY_BASIC);
		else if (rand_val <= 80)
			return (ENEMY_FAST);
		else
			return (ENEMY_TANK);
	}
	rand_val = random_int(1, 100);
	if (rand_val <= 40)
		return (ENEMY_BASIC);
	else if (rand_val <= 65)
		return (ENEMY_FAST);
	else if (rand_val <= 85)
		return (ENEMY_TANK);
	else
		return (ENEMY_FLYING);
}

static void	spawner_prepare_next_wave(t_game *game)
{
	t_wave	*wave;

	wave = &game->current_wave;
	wave->number++;
	wave->enemies_total = WAVE_BASE_ENEMIES + 
		(wave->number - 1) * WAVE_ENEMY_INCREMENT;
	wave->enemies_spawned = 0;
	wave->enemies_alive = 0;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE - 
		(wave->number - 1) * 0.1f;
	if (wave->spawn_delay < WAVE_SPAWN_DELAY_MIN)
		wave->spawn_delay = WAVE_SPAWN_DELAY_MIN;
	wave->spawn_timer = wave->spawn_delay;
	wave->prep_timer = WAVE_PREP_TIME;
	wave->active = false;
	wave->completed = false;
	wave->preparing = true;
	debug_info("Preparing wave %d: %d enemies, delay=%.2f", 
		wave->number, wave->enemies_total, wave->spawn_delay);
}

void	spawner_force_next_wave(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	if (game->current_wave.preparing)
	{
		game->current_wave.prep_timer = 0.0f;
		debug_info("Wave preparation skipped");
	}
}

int	spawner_get_enemies_remaining(t_game *game)
{
	t_wave	*wave;

	if (!validate_pointer(game))
		return (0);
	wave = &game->current_wave;
	return (wave->enemies_total - wave->enemies_spawned + wave->enemies_alive);
}
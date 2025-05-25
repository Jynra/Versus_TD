/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:28:45 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/* Forward declarations for functions implemented in render.c */
void	render_debug_info(t_game *game);

void	game_run(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	debug_info("Starting game loop...");
	game->state = STATE_PLAYING;
	while (game->running)
	{
		game_update_delta_time(game);
		game_handle_events(game);
		if (!game->paused)
			game_update(game);
		game_render(game);
		SDL_Delay(1);
	}
	debug_info("Game loop ended");
}

void	game_handle_events(t_game *game)
{
	SDL_Event	event;

	input_update(&game->input);
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			game->running = false;
			break ;
		}
		input_handle_event(&game->input, &event);
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				game_toggle_pause(game);
			else if (event.key.keysym.sym == SDLK_F1)
				game->show_debug = !game->show_debug;
			else if (event.key.keysym.sym == SDLK_r)
				game->show_ranges = !game->show_ranges;
		}
	}
}

void	game_update(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	game_update_stats(game);
	if (game->state == STATE_PLAYING)
	{
		waves_update(&game->current_wave, game);
		entities_update_all(game);
		effects_update(game);
		notifications_update(game);
		ui_update(game);
	}
}

void	game_render(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	render_clear(game);
	if (game->state == STATE_PLAYING)
	{
		render_grid(game);
		render_path(game);
		entities_render_all(game);
		if (game->show_ranges)
			render_ranges(game);
		effects_render(game);
		ui_render(game);
		notifications_render(game);
		if (game->show_debug)
			render_debug_info(game);
	}
	render_present(game);
}

void	game_update_delta_time(t_game *game)
{
	Uint32	current_time;

	current_time = SDL_GetTicks();
	game->delta_time = (current_time - game->last_frame_time) / 1000.0f;
	game->last_frame_time = current_time;
	game->fps_timer += game->delta_time;
	game->frame_count++;
	if (game->fps_timer >= 1.0f)
	{
		game->fps = game->frame_count / game->fps_timer;
		game->frame_count = 0;
		game->fps_timer = 0.0f;
	}
}

void	game_update_stats(t_game *game)
{
	game->stats.play_time += game->delta_time;
	game->active_towers = entities_count_active_towers(game);
	game->active_enemies = entities_count_active_enemies(game);
}

float	game_get_delta_time(t_game *game)
{
	if (!validate_pointer(game))
		return (0.0f);
	return (game->delta_time);
}

bool	game_is_running(t_game *game)
{
	if (!validate_pointer(game))
		return (false);
	return (game->running);
}

void	game_set_state(t_game *game, int state)
{
	if (!validate_pointer(game))
		return ;
	if (state != game->state)
	{
		debug_info("Game state changed: %d -> %d", game->state, state);
		game->state = state;
	}
}

int	game_get_state(t_game *game)
{
	if (!validate_pointer(game))
		return (STATE_MENU);
	return (game->state);
}

bool	game_is_paused(t_game *game)
{
	if (!validate_pointer(game))
		return (true);
	return (game->paused);
}

void	game_toggle_pause(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	game->paused = !game->paused;
	debug_info("Game %s", game->paused ? "paused" : "resumed");
}

void	game_add_money(t_game *game, int amount)
{
	if (!validate_pointer(game) || amount < 0)
		return ;
	game->money += amount;
	game->stats.total_money_earned += amount;
}

bool	game_spend_money(t_game *game, int amount)
{
	if (!validate_pointer(game) || amount < 0)
		return (false);
	if (game->money < amount)
		return (false);
	game->money -= amount;
	return (true);
}

void	game_lose_life(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	game->lives--;
	if (game->lives <= 0)
	{
		debug_info("Game Over! Final score: %d", game->score);
		game_set_state(game, STATE_GAME_OVER);
	}
}

void	game_add_score(t_game *game, int points)
{
	if (!validate_pointer(game) || points < 0)
		return ;
	game->score += points * game->multiplier;
	if (game->score > game->stats.best_score)
		game->stats.best_score = game->score;
}
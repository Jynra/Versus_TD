/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:07:25 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	cleanup_fonts(t_game *game);
static void	cleanup_renderer(t_game *game);
static void	cleanup_window(t_game *game);
static void	cleanup_sdl(void);

void	game_cleanup(t_game *game)
{
	if (!game)
	{
		debug_warning("Attempting to cleanup NULL game");
		return ;
	}
	debug_info("Starting game cleanup...");
	cleanup_fonts(game);
	cleanup_renderer(game);
	cleanup_window(game);
	cleanup_sdl();
	debug_info("Game cleanup complete");
}

void	game_shutdown(t_game *game)
{
	if (!game)
		return ;
	debug_info("Shutting down game...");
	game->running = false;
	game_cleanup(game);
}

static void	cleanup_fonts(t_game *game)
{
	debug_info("Cleaning up fonts...");
	if (game->font_small)
	{
		TTF_CloseFont(game->font_small);
		game->font_small = NULL;
	}
	if (game->font_medium)
	{
		TTF_CloseFont(game->font_medium);
		game->font_medium = NULL;
	}
	if (game->font_large)
	{
		TTF_CloseFont(game->font_large);
		game->font_large = NULL;
	}
	debug_info("Fonts cleaned up");
}

static void	cleanup_renderer(t_game *game)
{
	debug_info("Cleaning up renderer...");
	if (game->renderer)
	{
		SDL_DestroyRenderer(game->renderer);
		game->renderer = NULL;
	}
	debug_info("Renderer cleaned up");
}

static void	cleanup_window(t_game *game)
{
	debug_info("Cleaning up window...");
	if (game->window)
	{
		SDL_DestroyWindow(game->window);
		game->window = NULL;
	}
	debug_info("Window cleaned up");
}

static void	cleanup_sdl(void)
{
	debug_info("Shutting down SDL...");
	TTF_Quit();
	SDL_Quit();
	debug_info("SDL shutdown complete");
}
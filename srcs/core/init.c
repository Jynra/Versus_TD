/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:35:12 by student           #+#    #+#             */
/*   Updated: 2025/04/20 02:25:24 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/core/game.h"

bool	init_sdl(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Erreur d'initialisation SDL: %s\n", SDL_GetError());
		return (false);
	}
	game->window = SDL_CreateWindow("Tower Defense",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (!game->window)
	{
		printf("Erreur de création de fenêtre: %s\n", SDL_GetError());
		return (false);
	}
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
	if (!game->renderer)
	{
		printf("Erreur de création du renderer: %s\n", SDL_GetError());
		return (false);
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("Erreur d'initialisation SDL_image: %s\n", IMG_GetError());
		return (false);
	}
	if (TTF_Init() == -1)
	{
		printf("Erreur d'initialisation SDL_ttf: %s\n", TTF_GetError());
		return (false);
	}
	game->tower_texture = NULL;
	game->enemy_texture = NULL;
	game->projectile_texture = NULL;
	game->background_texture = NULL;
	game->font_small = NULL;
	game->font_medium = NULL;
	game->font_large = NULL;
	return (true);
}

bool	init_fonts(t_game *game)
{
	game->font_small = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12);
	if (!game->font_small)
	{
		printf("Erreur de chargement de la police small: %s\n", TTF_GetError());
		return (false);
	}
	game->font_medium = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
	if (!game->font_medium)
	{
		printf("Erreur de chargement de la police medium: %s\n", TTF_GetError());
		return (false);
	}
	game->font_large = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
	if (!game->font_large)
	{
		printf("Erreur de chargement de la police large: %s\n", TTF_GetError());
		return (false);
	}
	return (true);
}

void	cleanup_sdl(t_game *game)
{
	if (game->tower_texture != NULL)
		SDL_DestroyTexture(game->tower_texture);
	if (game->enemy_texture != NULL)
		SDL_DestroyTexture(game->enemy_texture);
	if (game->projectile_texture != NULL)
		SDL_DestroyTexture(game->projectile_texture);
	if (game->background_texture != NULL)
		SDL_DestroyTexture(game->background_texture);
	if (game->font_small != NULL)
		TTF_CloseFont(game->font_small);
	if (game->font_medium != NULL)
		TTF_CloseFont(game->font_medium);
	if (game->font_large != NULL)
		TTF_CloseFont(game->font_large);
	if (game->renderer != NULL)
		SDL_DestroyRenderer(game->renderer);
	if (game->window != NULL)
		SDL_DestroyWindow(game->window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void	init_tower_buttons(t_game *game)
{
	int	start_x;
	int	button_y;

	start_x = (WINDOW_WIDTH - (MAX_TOWER_TYPES
				* (TOWER_BUTTON_SIZE + TOWER_BUTTON_PADDING))) / 2;
	button_y = GAME_AREA_HEIGHT + (TOOLBAR_HEIGHT - TOWER_BUTTON_SIZE) / 2;
	game->tower_buttons[0].rect.x = start_x;
	game->tower_buttons[0].rect.y = button_y;
	game->tower_buttons[0].rect.w = TOWER_BUTTON_SIZE;
	game->tower_buttons[0].rect.h = TOWER_BUTTON_SIZE;
	game->tower_buttons[0].tower_type = TOWER_TYPE_BASIC;
	game->tower_buttons[0].description = "Tour standard";
	game->tower_buttons[0].cost = COST_TOWER_BASIC;
	game->tower_buttons[0].texture = NULL;
	game->tower_buttons[1].rect.x = start_x + TOWER_BUTTON_SIZE + TOWER_BUTTON_PADDING;
	game->tower_buttons[1].rect.y = button_y;
	game->tower_buttons[1].rect.w = TOWER_BUTTON_SIZE;
	game->tower_buttons[1].rect.h = TOWER_BUTTON_SIZE;
	game->tower_buttons[1].tower_type = TOWER_TYPE_SLOW;
	game->tower_buttons[1].description = "Tour ralentissante";
	game->tower_buttons[1].cost = COST_TOWER_SLOW;
	game->tower_buttons[1].texture = NULL;
	game->tower_buttons[2].rect.x = start_x + 2 * (TOWER_BUTTON_SIZE
			+ TOWER_BUTTON_PADDING);
	game->tower_buttons[2].rect.y = button_y;
	game->tower_buttons[2].rect.w = TOWER_BUTTON_SIZE;
	game->tower_buttons[2].rect.h = TOWER_BUTTON_SIZE;
	game->tower_buttons[2].tower_type = TOWER_TYPE_MULTI;
	game->tower_buttons[2].description = "Tour multi-cibles";
	game->tower_buttons[2].cost = COST_TOWER_MULTI;
	game->tower_buttons[2].texture = NULL;
}

void	init_restart_button(t_game *game)
{
	int	button_width;
	int	button_height;

	button_width = 200;
	button_height = 60;
	game->restart_button.x = (WINDOW_WIDTH - button_width) / 2;
	game->restart_button.y = (WINDOW_HEIGHT - button_height) / 2 + 100;
	game->restart_button.w = button_width;
	game->restart_button.h = button_height;
}

void	init_game_variables(t_game *game)
{
	game->is_running = true;
	game->state = STATE_PLAYING;
	game->money = STARTING_MONEY;
	game->lives = STARTING_LIVES;
	game->score = 0;
	game->round = 1;
	game->selected_tower_type = TOWER_TYPE_BASIC;
	game->placing_tower = false;
}

void	init_grid(t_game *game)
{
	int	row;
	int	col;

	row = 0;
	while (row < GRID_ROWS)
	{
		col = 0;
		while (col < GRID_COLS)
		{
			if (row == 5)
				game->grid[row][col] = TERRAIN_PATH;
			else
				game->grid[row][col] = TERRAIN_BUILDABLE;
			col++;
		}
		row++;
	}
}

void	init_path(t_game *game)
{
	game->path_length = 2;
	game->path[0].x = 0;
	game->path[0].y = 5 * GRID_SIZE + GRID_SIZE / 2;
	game->path[1].x = WINDOW_WIDTH;
	game->path[1].y = 5 * GRID_SIZE + GRID_SIZE / 2;
}

void	init_entities(t_game *game)
{
	int	i;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		game->enemies[i].active = false;
		game->enemies[i].texture = NULL;
		i++;
	}
	i = 0;
	while (i < MAX_TOWERS)
	{
		game->towers[i].active = false;
		game->towers[i].texture = NULL;
		i++;
	}
	i = 0;
	while (i < MAX_PROJECTILES)
	{
		game->projectiles[i].active = false;
		game->projectiles[i].texture = NULL;
		i++;
	}
}

void	init_game(t_game *game)
{
	if (GRID_ROWS * GRID_COLS > 0)
		printf("grille initialisee avec dimensions: %d x %d\n", GRID_ROWS, GRID_COLS);
	else
		fprintf(stderr, "ERROR: Dimensions de grille invalides: %d x %d\n",
			GRID_ROWS, GRID_COLS);
	init_game_variables(game);
	init_grid(game);
	init_path(game);
	init_entities(game);
	init_tower_buttons(game);
	init_restart_button(game);
	init_fonts(game);
}

void	reset_game(t_game *game)
{
	init_game_variables(game);
	init_grid(game);
	init_entities(game);
}
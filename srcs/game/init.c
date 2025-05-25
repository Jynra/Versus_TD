/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:21:09 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static bool	init_sdl(void);
static bool	init_window(t_game *game);
static bool	init_renderer(t_game *game);
static bool	init_fonts(t_game *game);
static bool	init_game_data(t_game *game);

bool	game_init(t_game *game)
{
	if (!validate_pointer(game))
		return (false);
	debug_info("Initializing game...");
	memory_set(game, 0, sizeof(t_game));
	if (!init_sdl())
		return (false);
	if (!init_window(game))
		return (false);
	if (!init_renderer(game))
		return (false);
	if (!init_fonts(game))
		return (false);
	if (!init_game_data(game))
		return (false);
	debug_info("Game initialization complete");
	return (true);
}

static bool	init_sdl(void)
{
	debug_info("Initializing SDL...");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
	{
		debug_error("SDL initialization failed: %s", SDL_GetError());
		return (false);
	}
	if (TTF_Init() == -1)
	{
		debug_error("SDL_ttf initialization failed: %s", TTF_GetError());
		SDL_Quit();
		return (false);
	}
	debug_info("SDL initialized successfully");
	return (true);
}

static bool	init_window(t_game *game)
{
	debug_info("Creating window (%dx%d)...", WINDOW_WIDTH, WINDOW_HEIGHT);
	game->window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (!game->window)
	{
		debug_error("Window creation failed: %s", SDL_GetError());
		return (false);
	}
	debug_info("Window created successfully");
	return (true);
}

static bool	init_renderer(t_game *game)
{
	debug_info("Creating renderer...");
	game->renderer = SDL_CreateRenderer(
		game->window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!game->renderer)
	{
		debug_error("Renderer creation failed: %s", SDL_GetError());
		return (false);
	}
	SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
	debug_info("Renderer created successfully");
	return (true);
}

static bool	init_fonts(t_game *game)
{
	debug_info("Loading fonts...");
	game->font_small = TTF_OpenFont(FONT_PATH, FONT_SIZE_SMALL);
	if (!game->font_small)
	{
		debug_warning("Small font loading failed: %s", TTF_GetError());
		debug_info("Using default system font");
	}
	game->font_medium = TTF_OpenFont(FONT_PATH, FONT_SIZE_MEDIUM);
	if (!game->font_medium)
	{
		debug_warning("Medium font loading failed: %s", TTF_GetError());
	}
	game->font_large = TTF_OpenFont(FONT_PATH, FONT_SIZE_LARGE);
	if (!game->font_large)
	{
		debug_warning("Large font loading failed: %s", TTF_GetError());
	}
	debug_info("Font loading complete");
	return (true);
}

static bool	init_game_data(t_game *game)
{
	debug_info("Initializing game data...");
	game->running = true;
	game->paused = false;
	game->state = STATE_MENU;
	game->money = STARTING_MONEY;
	game->lives = STARTING_LIVES;
	game->score = 0;
	game->multiplier = 1;
	game->selected_tower_type = TOWER_BASIC;
	game->placing_tower = false;
	game->selected_tower_id = -1;
	game->show_ranges = false;
	game->show_debug = false;
	game->last_frame_time = SDL_GetTicks();
	game->delta_time = 0.0f;
	game->fps = 0.0f;
	game->frame_count = 0;
	game->fps_timer = 0.0f;
	game_init_world(game);
	input_init(&game->input);
	effects_init(game);
	waves_init(&game->current_wave);
	notifications_init(game);
	ui_init(game);
	random_seed((unsigned int)time(NULL));
	debug_info("Game data initialized");
	return (true);
}

void	game_init_world(t_game *game)
{
	debug_info("Initializing game world...");
	game_init_grid(game);
	game_init_path(game);
	debug_info("Game world initialized");
}

void	game_init_grid(t_game *game)
{
	int	x;
	int	y;

	debug_info("Initializing grid (%dx%d)...", GRID_COLS, GRID_ROWS);
	y = 0;
	while (y < GRID_ROWS)
	{
		x = 0;
		while (x < GRID_COLS)
		{
			game->grid[y][x].type = TERRAIN_GRASS;
			game->grid[y][x].occupied = false;
			game->grid[y][x].tower_id = -1;
			game->grid[y][x].movement_cost = 1.0f;
			x++;
		}
		y++;
	}
}

void	game_init_path(t_game *game)
{
	int			path_y;
	int			x;
	t_vector2	start;
	t_vector2	end;

	debug_info("Initializing enemy path...");
	path_y = GRID_ROWS / 2;
	start = vec2_create(-GRID_SIZE, grid_to_world_y(path_y));
	end = vec2_create(WINDOW_WIDTH + GRID_SIZE, grid_to_world_y(path_y));
	game->path[0].pos = start;
	game->path[0].direction = vec2_create(1.0f, 0.0f);
	game->path[1].pos = end;
	game->path[1].direction = vec2_create(1.0f, 0.0f);
	game->path_length = 2;
	x = 0;
	while (x < GRID_COLS)
	{
		game->grid[path_y][x].type = TERRAIN_PATH;
		game->grid[path_y][x].movement_cost = 0.5f;
		x++;
	}
	debug_info("Path initialized: %d points", game->path_length);
}

bool	game_is_position_valid(t_game *game, t_vector2 pos)
{
	int	grid_x;
	int	grid_y;

	grid_x = world_to_grid_x(pos.x);
	grid_y = world_to_grid_y(pos.y);
	if (!is_valid_grid_pos(grid_x, grid_y))
		return (false);
	return (game->grid[grid_y][grid_x].type != TERRAIN_PATH
		&& !game->grid[grid_y][grid_x].occupied);
}
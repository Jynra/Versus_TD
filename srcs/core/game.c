/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:16:45 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 13:39:20 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"
#include <time.h>

static bool	sdl_init(t_game *game);
static void	init_game_variables(t_game *game);
static void	init_grid(t_game *game);

/**
 * @brief Initialise le jeu et tous ses composants
 * 
 * @param game Pointeur vers la structure du jeu
 * @return true si l'initialisation a réussi, false sinon
 */
bool game_init(t_game *game)
{
    if (!game)
    {
        fprintf(stderr, "ERROR: Null game pointer in game_init\n");
        return (false);
    }
    
    srand(time(NULL));
    
    if (!sdl_init(game))
        return (false);
    
    if (!assets_load_all(game))
    {
        fprintf(stderr, "ERROR: Failed to load assets\n");
        game_cleanup(game);
        return (false);
    }
    
    init_game_variables(game);
    init_grid(game);
    path_init(game);
    ui_init(game);
    
    return (true);
}

/**
 * @brief Nettoie et libère toutes les ressources allouées par le jeu
 * 
 * @param game Pointeur vers la structure du jeu
 */
void game_cleanup(t_game *game)
{
    printf("Starting game cleanup\n");
    
    if (!game)
        return;
    
    /* Libération des ressources graphiques */
    assets_free_all(game);
    
    /* Destruction du renderer */
    if (game->renderer)
    {
        SDL_DestroyRenderer(game->renderer);
        game->renderer = NULL;
        printf("Renderer destroyed\n");
    }
    
    /* Destruction de la fenêtre */
    if (game->window)
    {
        SDL_DestroyWindow(game->window);
        game->window = NULL;
        printf("Window destroyed\n");
    }
    
    /* Fermeture des bibliothèques SDL */
    TTF_Quit();
    printf("TTF_Quit done\n");
    
    IMG_Quit();
    printf("IMG_Quit done\n");
    
    SDL_Quit();
    printf("SDL_Quit done\n");
    
    printf("Game cleanup complete\n");
}

/**
 * @brief Réinitialise le jeu pour une nouvelle partie
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	game_reset(t_game *game)
{
	int	i;
	
	/* Réinitialisation des variables de jeu */
	game->state = STATE_PLAYING;
	game->money = STARTING_MONEY;
	game->lives = STARTING_LIVES;
	game->score = 0;
	game->round = 1;
	game->game_time = 0.0f;
	game->selected_tower_type = TOWER_TYPE_BASIC;
	game->placing_tower = false;
	
	/* Réinitialisation de la grille */
	init_grid(game);
	
	/* Réinitialisation des entités */
	i = 0;
	while (i < MAX_ENEMIES)
	{
		game->enemies[i].active = false;
		i++;
	}
	
	i = 0;
	while (i < MAX_TOWERS)
	{
		game->towers[i].active = false;
		i++;
	}
	
	i = 0;
	while (i < MAX_PROJECTILES)
	{
		game->projectiles[i].active = false;
		i++;
	}
}

/**
 * @brief Gère le rendu du jeu selon son état
 * 
 * @param game Pointeur vers la structure du jeu
 */
void game_render(t_game *game)
{
    /* Effacer l'écran */
    render_clear(game);
    
    /* Rendu en fonction de l'état du jeu */
    if (game->state == STATE_PLAYING)
    {
        render_game_area(game);
        ui_render_toolbar(game);
        ui_render_round_info(game);
        
        /* Affichage des informations de débogage si activé */
        if (DEBUG_MODE)
            render_debug_info(game, true, false);
    }
    else if (game->state == STATE_GAME_OVER)
    {
        render_game_area(game);
        ui_render_toolbar(game);
        ui_render_game_over(game);
    }
    else if (game->state == STATE_PAUSED)
    {
        render_game_area(game);
        ui_render_toolbar(game);
        ui_render_pause_menu(game);
    }
    
    /* Présenter le rendu final à l'écran */
    render_present(game);
}

/**
 * @brief Gère les mises à jour du jeu selon son état
 * 
 * @param game Pointeur vers la structure du jeu
 * @param delta_time Temps écoulé depuis la dernière mise à jour
 */
void	game_update(t_game *game, float delta_time)
{
	if (game->state == STATE_PLAYING)
	{
		game->game_time += delta_time;
		
		/* Mise à jour des entités */
		towers_update(game);
		enemies_update(game);
		projectiles_update(game);
		
		/* Vérification des conditions de fin de jeu */
		if (game->lives <= 0)
		{
			game->state = STATE_GAME_OVER;
			printf("Game Over! Score final: %d\n", game->score);
		}
	}
}

/**
 * @brief Démarre une nouvelle vague d'ennemis
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	game_start_round(t_game *game)
{
	int	i;
	int	num_enemies;
	int	enemy_type;
	
	if (game->state != STATE_PLAYING)
		return;
	
	game->round++;
	num_enemies = 5 + game->round; /* Augmentation progressive du nombre d'ennemis */
	
	/* Planification des ennemis à apparaître */
	for (i = 0; i < num_enemies; i++)
	{
		/* Diversification des types d'ennemis en fonction de la vague */
		if (game->round >= 5 && rand() % 10 == 0)
			enemy_type = ENEMY_TYPE_BOSS;
		else if (game->round >= 3 && rand() % 4 == 0)
			enemy_type = ENEMY_TYPE_FAST;
		else
			enemy_type = ENEMY_TYPE_BASIC;
			
		enemy_spawn(game, enemy_type);
	}
}

/**
 * @brief Ajoute des points au score du joueur
 * 
 * @param game Pointeur vers la structure du jeu
 * @param points Points à ajouter
 */
void	game_add_score(t_game *game, int points)
{
	game->score += points;
}

/**
 * @brief Ajoute de l'argent au joueur
 * 
 * @param game Pointeur vers la structure du jeu
 * @param amount Montant à ajouter
 */
void	game_add_money(t_game *game, int amount)
{
	game->money += amount;
}

/* Fonctions statiques internes */

/**
 * @brief Initialise SDL et ses dépendances
 * 
 * @param game Pointeur vers la structure du jeu
 * @return true si l'initialisation a réussi, false sinon
 */
static bool	sdl_init(t_game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "ERROR: SDL initialization failed: %s\n", SDL_GetError());
		return (false);
	}
	
	game->window = SDL_CreateWindow(
		"Versus Tower Defense",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);
		
	if (!game->window)
	{
		fprintf(stderr, "ERROR: Window creation failed: %s\n", SDL_GetError());
		return (false);
	}
	
	game->renderer = SDL_CreateRenderer(
		game->window, 
		-1,
		SDL_RENDERER_ACCELERATED);
		
	if (!game->renderer)
	{
		fprintf(stderr, "ERROR: Renderer creation failed: %s\n", SDL_GetError());
		return (false);
	}
	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		fprintf(stderr, "ERROR: SDL_image initialization failed: %s\n", IMG_GetError());
		return (false);
	}
	
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "ERROR: SDL_ttf initialization failed: %s\n", TTF_GetError());
		return (false);
	}
	
	return (true);
}

/**
 * @brief Initialise les variables de jeu
 * 
 * @param game Pointeur vers la structure du jeu
 */
static void	init_game_variables(t_game *game)
{
	game->is_running = true;
	game->state = STATE_PLAYING;
	game->money = STARTING_MONEY;
	game->lives = STARTING_LIVES;
	game->score = 0;
	game->round = 1;
	game->game_time = 0.0f;
	game->selected_tower_type = TOWER_TYPE_BASIC;
	game->placing_tower = false;
}

/**
 * @brief Initialise la grille de jeu
 * 
 * @param game Pointeur vers la structure du jeu
 */
static void	init_grid(t_game *game)
{
	int	row;
	int	col;
	
	for (row = 0; row < GRID_ROWS; row++)
	{
		for (col = 0; col < GRID_COLS; col++)
		{
			if (row == 5)
				game->grid[row][col] = TERRAIN_PATH;
			else
				game->grid[row][col] = TERRAIN_BUILDABLE;
		}
	}
}
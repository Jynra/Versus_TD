/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:20:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 13:41:02 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

/**
 * @brief Initialise le système de rendu
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	render_init(t_game *game)
{
	/* Rien de spécial à initialiser pour le moment,
	   les textures sont chargées dans assets_load_all */
	(void)game;
}

/**
 * @brief Efface l'écran pour un nouveau rendu
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	render_clear(t_game *game)
{
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);
}

/**
 * @brief Affiche le rendu final à l'écran
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	render_present(t_game *game)
{
	SDL_RenderPresent(game->renderer);
}

/**
 * @brief Rend la zone de jeu principale
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	render_game_area(t_game *game)
{
	/* Afficher d'abord le fond */
	render_background(game);
	
	/* Puis la grille */
	render_grid(game);
	
	/* Ensuite les entités du jeu */
	towers_render(game);
	projectiles_render(game);
	enemies_render(game);
	
	/* L'aperçu de la tour sélectionnée doit être affiché en dernier */
	if (game->placing_tower)
		tower_preview_render(game);
}

/**
 * @brief Affiche la grille de jeu
 * 
 * @param game Pointeur vers la structure du jeu
 */
void render_grid(t_game *game)
{
    int row;
    int col;
    SDL_Rect rect;

    row = 0;
    while (row < GRID_ROWS)
    {
        col = 0;
        while (col < GRID_COLS)
        {
            rect.x = col * GRID_SIZE;
            rect.y = row * GRID_SIZE;  /* Pas de décalage TOP_BAR_HEIGHT */
            rect.w = GRID_SIZE;
            rect.h = GRID_SIZE;
            
            /* Couleur différente selon le type de terrain */
            if (game->grid[row][col] == TERRAIN_PATH)
                SDL_SetRenderDrawColor(game->renderer, 200, 200, 100, 255);
            else if (game->grid[row][col] == TERRAIN_OCCUPIED)
                SDL_SetRenderDrawColor(game->renderer, 130, 130, 130, 255);
            else
                SDL_SetRenderDrawColor(game->renderer, 100, 150, 100, 255);
                
            SDL_RenderFillRect(game->renderer, &rect);
            
            /* Lignes de grille */
            SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
            SDL_RenderDrawRect(game->renderer, &rect);
            
            col++;
        }
        row++;
    }
}

/**
 * @brief Affiche le fond de la zone de jeu
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	render_background(t_game *game)
{
	SDL_Rect rect;
	
	/* Si une texture de fond est disponible, l'utiliser */
	if (game->textures[TEXTURE_BACKGROUND])
	{
		rect.x = 0;
		rect.y = 0;
		rect.w = WINDOW_WIDTH;
		rect.h = GAME_AREA_HEIGHT;
		SDL_RenderCopy(game->renderer, game->textures[TEXTURE_BACKGROUND], NULL, &rect);
	}
	else
	{
		/* Sinon, utiliser une couleur unie */
		SDL_SetRenderDrawColor(game->renderer, 50, 80, 50, 255);
		rect.x = 0;
		rect.y = 0;
		rect.w = WINDOW_WIDTH;
		rect.h = GAME_AREA_HEIGHT;
		SDL_RenderFillRect(game->renderer, &rect);
	}
}

/**
 * @brief Affiche un effet d'explosion à une position donnée
 * 
 * @param game Pointeur vers la structure du jeu
 * @param x Position X de l'explosion
 * @param y Position Y de l'explosion
 * @param scale Échelle de l'explosion
 */
void	render_explosion(t_game *game, float x, float y, float scale)
{
	int			size;
	SDL_Rect	rect;
	Uint8		alpha;
	int			i;
	
	/* Plusieurs cercles avec transparence décroissante */
	for (i = 0; i < 3; i++)
	{
		size = (int)(10 * scale * (i + 1));
		alpha = 255 - i * 70;
		
		rect.x = (int)x - size / 2;
		rect.y = (int)y - size / 2;
		rect.w = size;
		rect.h = size;
		
		SDL_SetRenderDrawColor(game->renderer, 255, 165, 0, alpha);
		SDL_RenderFillRect(game->renderer, &rect);
	}
}

/**
 * @brief Affiche un effet de ralentissement à une position donnée
 * 
 * @param game Pointeur vers la structure du jeu
 * @param x Position X de l'effet
 * @param y Position Y de l'effet
 * @param radius Rayon de l'effet
 */
void	render_slow_effect(t_game *game, float x, float y, float radius)
{
	SDL_Rect	rect;
	int			size;
	int			i;
	Uint8		alpha;
	
	/* Effet de vague avec des cercles bleus transparents */
	for (i = 0; i < 2; i++)
	{
		size = (int)(radius * (0.6 + 0.2 * i));
		alpha = 100 - i * 40;
		
		rect.x = (int)x - size / 2;
		rect.y = (int)y - size / 2;
		rect.w = size;
		rect.h = size;
		
		SDL_SetRenderDrawColor(game->renderer, 0, 150, 255, alpha);
		SDL_RenderDrawRect(game->renderer, &rect);
	}
}

/**
 * @brief Affiche des informations de débogage à l'écran
 * 
 * @param game Pointeur vers la structure du jeu
 * @param show_fps Indique s'il faut afficher les FPS
 * @param show_grid_coords Indique s'il faut afficher les coordonnées de la grille
 */
void	render_debug_info(t_game *game, bool show_fps, bool show_grid_coords)
{
	char		text[128];
	SDL_Rect	text_rect;
	SDL_Color	debug_color;
	int			active_enemies;
	int			active_towers;
	int			active_projectiles;
	int			i;
	int			row;
	int			col;
	SDL_Rect	coord_rect;
	char		coord_text[32];
	
	/* Seulement en mode DEBUG */
	if (!DEBUG_MODE)
		return;
		
	debug_color = ui_color_yellow();
	
	/* Compter les entités actives */
	active_enemies = 0;
	active_towers = 0;
	active_projectiles = 0;
	
	for (i = 0; i < MAX_ENEMIES; i++)
		if (game->enemies[i].active)
			active_enemies++;
	
	for (i = 0; i < MAX_TOWERS; i++)
		if (game->towers[i].active)
			active_towers++;
			
	for (i = 0; i < MAX_PROJECTILES; i++)
		if (game->projectiles[i].active)
			active_projectiles++;
	
	/* Affichage des informations de débogage */
	sprintf(text, "Actifs: %d ennemis, %d tours, %d projectiles", 
		active_enemies, active_towers, active_projectiles);
		
	text_rect.x = 10;
	text_rect.y = 10;
	text_rect.w = 300;
	text_rect.h = 20;
	
	ui_render_text(game, text, game->fonts[FONT_SMALL], text_rect, debug_color);
	
	/* Affichage des coordonnées de la grille */
	if (show_grid_coords)
	{
		for (row = 0; row < GRID_ROWS; row++)
		{
			for (col = 0; col < GRID_COLS; col++)
			{
				sprintf(coord_text, "%d,%d", col, row);
				
				coord_rect.x = col * GRID_SIZE + 5;
				coord_rect.y = row * GRID_SIZE + 5;
				coord_rect.w = GRID_SIZE - 10;
				coord_rect.h = 15;
				
				ui_render_text(game, coord_text, game->fonts[FONT_SMALL], 
					coord_rect, debug_color);
			}
		}
	}
	
	/* Affichage des FPS */
	if (show_fps)
	{
		/* Calcul approximatif des FPS avec le temps de la frame */
		static Uint32 last_time = 0;
		static int frame_count = 0;
		static float fps = 0.0f;
		Uint32 current_time;
		
		current_time = SDL_GetTicks();
		frame_count++;
		
		if (current_time - last_time >= 1000)
		{
			fps = frame_count * 1000.0f / (current_time - last_time);
			frame_count = 0;
			last_time = current_time;
		}
		
		sprintf(text, "FPS: %.1f", fps);
		
		text_rect.x = WINDOW_WIDTH - 80;
		text_rect.y = 10;
		text_rect.w = 70;
		text_rect.h = 20;
		
		ui_render_text(game, text, game->fonts[FONT_SMALL], text_rect, debug_color);
	}
}

/**
 * @brief Affiche la portée d'une tour spécifique avec une couleur donnée
 * 
 * @param game Pointeur vers la structure du jeu
 * @param tower_index Indice de la tour dans le tableau
 * @param color Couleur de la portée
 */
void	render_tower_range(t_game *game, int tower_index, SDL_Color color)
{
	SDL_Rect	range_rect;
	int			radius;
	Uint8		r, g, b, a;
	
	/* Vérification de la validité de l'indice */
	if (tower_index < 0 || tower_index >= MAX_TOWERS)
		return;
		
	/* Vérification si la tour est active */
	if (!game->towers[tower_index].active)
		return;
		
	/* Sauvegarde des couleurs actuelles */
	SDL_GetRenderDrawColor(game->renderer, &r, &g, &b, &a);
	
	/* Définition de la couleur */
	SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
	
	/* Calcul du rectangle de portée */
	radius = (int)game->towers[tower_index].range;
	range_rect.x = (int)game->towers[tower_index].x - radius;
	range_rect.y = (int)game->towers[tower_index].y - radius;
	range_rect.w = radius * 2;
	range_rect.h = radius * 2;
	
	/* Affichage du cercle de portée */
	SDL_RenderDrawRect(game->renderer, &range_rect);
	
	/* Restauration des couleurs */
	SDL_SetRenderDrawColor(game->renderer, r, g, b, a);
}
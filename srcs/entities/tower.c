/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tower.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:15:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:37:55 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

static void	set_tower_properties_by_type(t_tower *tower, int type);
static void	shift_targets(int *target_indices, float *target_distances, 
				int insert_index, int targets_found);
static void	shift_targets_insert(int *target_indices, float *target_distances, 
				int insert_index, int targets_to_find);

/**
 * @brief Initialise une tour avec les propriétés basées sur son type
 * 
 * @param tower Pointeur vers la tour à initialiser
 * @param type Type de la tour (TOWER_TYPE_BASIC, etc.)
 * @param x Position X de la tour
 * @param y Position Y de la tour
 */
void	tower_init(t_tower *tower, int type, float x, float y)
{
	tower->active = true;
	tower->x = x;
	tower->y = y;
	tower->width = 30;
	tower->height = 30;
	tower->type = type;
	tower->fire_timer = 0.0f;
	
	set_tower_properties_by_type(tower, type);
}

/**
 * @brief Initialise les boutons de sélection de tours
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	tower_buttons_init(t_game *game)
{
	int	start_x;
	int	button_y;

	start_x = (WINDOW_WIDTH - (MAX_TOWER_TYPES 
				* (TOWER_BUTTON_SIZE + TOWER_BUTTON_PADDING))) / 2;
	button_y = GAME_AREA_HEIGHT + (TOOLBAR_HEIGHT - TOWER_BUTTON_SIZE) / 2;
	
	/* Bouton tour basique */
	game->tower_buttons[0].rect.x = start_x;
	game->tower_buttons[0].rect.y = button_y;
	game->tower_buttons[0].rect.w = TOWER_BUTTON_SIZE;
	game->tower_buttons[0].rect.h = TOWER_BUTTON_SIZE;
	game->tower_buttons[0].tower_type = TOWER_TYPE_BASIC;
	game->tower_buttons[0].description = "Basic Tower";
	game->tower_buttons[0].cost = COST_TOWER_BASIC;
	
	/* Bouton tour ralentissante */
	game->tower_buttons[1].rect.x = start_x + TOWER_BUTTON_SIZE + TOWER_BUTTON_PADDING;
	game->tower_buttons[1].rect.y = button_y;
	game->tower_buttons[1].rect.w = TOWER_BUTTON_SIZE;
	game->tower_buttons[1].rect.h = TOWER_BUTTON_SIZE;
	game->tower_buttons[1].tower_type = TOWER_TYPE_SLOW;
	game->tower_buttons[1].description = "Slow Tower";
	game->tower_buttons[1].cost = COST_TOWER_SLOW;
	
	/* Bouton tour multi-cibles */
	game->tower_buttons[2].rect.x = start_x + 2 * (TOWER_BUTTON_SIZE + TOWER_BUTTON_PADDING);
	game->tower_buttons[2].rect.y = button_y;
	game->tower_buttons[2].rect.w = TOWER_BUTTON_SIZE;
	game->tower_buttons[2].rect.h = TOWER_BUTTON_SIZE;
	game->tower_buttons[2].tower_type = TOWER_TYPE_MULTI;
	game->tower_buttons[2].description = "Multi Tower";
	game->tower_buttons[2].cost = COST_TOWER_MULTI;
}

/**
 * @brief Vérifie si une tour peut être placée à une position donnée
 * 
 * @param game Pointeur vers la structure du jeu
 * @param grid_x Coordonnée X sur la grille
 * @param grid_y Coordonnée Y sur la grille
 * @return true si la tour peut être placée, false sinon
 */
bool	tower_can_be_placed(t_game *game, int grid_x, int grid_y)
{
	if (grid_x < 0 || grid_x >= GRID_COLS || grid_y < 0 || grid_y >= GRID_ROWS)
		return (false);
	if (game->grid[grid_y][grid_x] != TERRAIN_BUILDABLE)
		return (false);
	
	/* Vérifier si le joueur a assez d'argent */
	if (game->selected_tower_type == TOWER_TYPE_BASIC)
		return (game->money >= COST_TOWER_BASIC);
	else if (game->selected_tower_type == TOWER_TYPE_SLOW)
		return (game->money >= COST_TOWER_SLOW);
	else if (game->selected_tower_type == TOWER_TYPE_MULTI)
		return (game->money >= COST_TOWER_MULTI);
	
	return (false);
}

/**
 * @brief Place une tour sur la grille de jeu
 * 
 * @param game Pointeur vers la structure du jeu
 * @param grid_x Coordonnée X sur la grille
 * @param grid_y Coordonnée Y sur la grille
 */
void	tower_place(t_game *game, int grid_x, int grid_y)
{
	int	tower_cost;
	int	i;
	int	center_x;
	int	center_y;

	if (!tower_can_be_placed(game, grid_x, grid_y))
		return;

	/* Calcul du coût en fonction du type de tour */
	if (game->selected_tower_type == TOWER_TYPE_BASIC)
		tower_cost = COST_TOWER_BASIC;
	else if (game->selected_tower_type == TOWER_TYPE_SLOW)
		tower_cost = COST_TOWER_SLOW;
	else if (game->selected_tower_type == TOWER_TYPE_MULTI)
		tower_cost = COST_TOWER_MULTI;
	else
		return;

	/* Trouver un emplacement libre pour la tour */
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (!game->towers[i].active)
		{
			center_x = grid_x * GRID_SIZE + GRID_SIZE / 2;
			center_y = grid_y * GRID_SIZE + GRID_SIZE / 2;
			
			tower_init(&game->towers[i], game->selected_tower_type, 
						center_x, center_y);
			
			game->money -= tower_cost;
			game->grid[grid_y][grid_x] = TERRAIN_OCCUPIED;
			break;
		}
		i++;
	}
}

/**
 * @brief Met à jour toutes les tours actives
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	towers_update(t_game *game)
{
	int	i;
	int	targets_to_find;
	int	target_indices[TOWER_MULTI_TARGETS];
	int	t;
	int	targets_found;

	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].active)
		{
			/* Mise à jour du timer de tir */
			game->towers[i].fire_timer += 1.0f / FPS;
			
			/* Si prêt à tirer */
			if (game->towers[i].fire_timer >= 1.0f / game->towers[i].fire_rate)
			{
				/* Déterminer le nombre de cibles en fonction du type de tour */
				targets_to_find = 1;
				if (game->towers[i].type == TOWER_TYPE_MULTI)
					targets_to_find = TOWER_MULTI_TARGETS;
				
				/* Trouver les cibles */
				tower_find_targets(game, &game->towers[i], targets_to_find, target_indices);
				
				/* Tirer sur les cibles trouvées */
				targets_found = 0;
				t = 0;
				while (t < targets_to_find && target_indices[t] != -1)
				{
					tower_fire_at_target(game, &game->towers[i], target_indices[t]);
					targets_found++;
					t++;
				}
				
				/* Réinitialiser le timer si au moins une cible a été trouvée */
				if (targets_found > 0)
					game->towers[i].fire_timer = 0.0f;
			}
		}
		i++;
	}
}

/**
 * @brief Cherche des cibles pour une tour
 * 
 * @param game Pointeur vers la structure du jeu
 * @param tower Pointeur vers la tour
 * @param targets_to_find Nombre de cibles à trouver
 * @param target_indices Tableau pour stocker les indices des cibles trouvées
 */
void	tower_find_targets(t_game *game, t_tower *tower, int targets_to_find,
		int *target_indices)
{
	int		i;
	int		j;
	int		insert_index;
	float	dist;
	float	target_distances[TOWER_MULTI_TARGETS];
	int		targets_found;

	targets_found = 0;
	
	/* Initialiser les tableaux */
	i = 0;
	while (i < TOWER_MULTI_TARGETS)
	{
		target_indices[i] = -1;
		target_distances[i] = 0;
		i++;
	}
	
	/* Parcourir tous les ennemis */
	j = 0;
	while (j < MAX_ENEMIES)
	{
		if (game->enemies[j].active)
		{
			dist = distance(tower->x, tower->y,
					game->enemies[j].x, game->enemies[j].y);
					
			/* Si l'ennemi est à portée */
			if (dist < tower->range)
			{
				/* Trouver la position d'insertion */
				insert_index = targets_found;
				while (insert_index > 0 && dist < target_distances[insert_index - 1])
					insert_index--;
					
				/* Insérer la cible au bon endroit */
				if (insert_index < targets_found && targets_found < targets_to_find)
					shift_targets(target_indices, target_distances, insert_index,
							targets_found);
							
				if (targets_found < targets_to_find)
				{
					target_indices[insert_index] = j;
					target_distances[insert_index] = dist;
					targets_found++;
				}
				else if (insert_index < targets_to_find)
				{
					shift_targets_insert(target_indices, target_distances,
							insert_index, targets_to_find);
					target_indices[insert_index] = j;
					target_distances[insert_index] = dist;
				}
			}
		}
		j++;
	}
}

/**
 * @brief Tire un projectile sur une cible
 * 
 * @param game Pointeur vers la structure du jeu
 * @param tower Pointeur vers la tour qui tire
 * @param target_index Indice de la cible dans le tableau d'ennemis
 */
void	tower_fire_at_target(t_game *game, t_tower *tower, int target_index)
{
	projectile_create(game, tower->x, tower->y,
		game->enemies[target_index].x, game->enemies[target_index].y,
		tower->damage, tower->type);
}

/**
 * @brief Affiche toutes les tours actives
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	towers_render(t_game *game)
{
	int			i;
	SDL_Rect	rect;
	SDL_Rect	range_rect;
	int			radius;

	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].active)
		{
			rect.x = (int)game->towers[i].x - game->towers[i].width / 2;
			rect.y = (int)game->towers[i].y - game->towers[i].height / 2;
			rect.w = game->towers[i].width;
			rect.h = game->towers[i].height;
			
			/* Couleur différente selon le type de tour */
			if (game->towers[i].type == TOWER_TYPE_BASIC)
				SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 255);
			else if (game->towers[i].type == TOWER_TYPE_SLOW)
				SDL_SetRenderDrawColor(game->renderer, 0, 150, 200, 255);
			else if (game->towers[i].type == TOWER_TYPE_MULTI)
				SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 255);
				
			SDL_RenderFillRect(game->renderer, &rect);
			
			/* Affichage de la portée */
			SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 100);
			radius = (int)game->towers[i].range;
			range_rect.x = (int)game->towers[i].x - radius;
			range_rect.y = (int)game->towers[i].y - radius;
			range_rect.w = radius * 2;
			range_rect.h = radius * 2;
			SDL_RenderDrawRect(game->renderer, &range_rect);
		}
		i++;
	}
}

/**
 * @brief Affiche les boutons de sélection de tours
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	tower_buttons_render(t_game *game)
{
	int			i;
	SDL_Rect	tower_rect;
	SDL_Rect	text_rect;
	char		cost_text[20];
	SDL_Color	text_color;

	i = 0;
	while (i < MAX_TOWER_TYPES)
	{
		/* Couleur du fond du bouton */
		if (game->placing_tower 
			&& game->selected_tower_type == game->tower_buttons[i].tower_type)
			SDL_SetRenderDrawColor(game->renderer, 150, 150, 200, 255);
		else if (game->money >= game->tower_buttons[i].cost)
			SDL_SetRenderDrawColor(game->renderer, 120, 120, 150, 255);
		else
			SDL_SetRenderDrawColor(game->renderer, 90, 90, 90, 255);
			
		SDL_RenderFillRect(game->renderer, &game->tower_buttons[i].rect);
		
		/* Contour du bouton */
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
		SDL_RenderDrawRect(game->renderer, &game->tower_buttons[i].rect);
		
		/* Icône de la tour */
		tower_rect.x = game->tower_buttons[i].rect.x + game->tower_buttons[i].rect.w / 4;
		tower_rect.y = game->tower_buttons[i].rect.y + game->tower_buttons[i].rect.h / 4;
		tower_rect.w = game->tower_buttons[i].rect.w / 2;
		tower_rect.h = game->tower_buttons[i].rect.h / 2;
		
		if (game->tower_buttons[i].tower_type == TOWER_TYPE_BASIC)
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 255);
		else if (game->tower_buttons[i].tower_type == TOWER_TYPE_SLOW)
			SDL_SetRenderDrawColor(game->renderer, 0, 150, 200, 255);
		else if (game->tower_buttons[i].tower_type == TOWER_TYPE_MULTI)
			SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 255);
			
		SDL_RenderFillRect(game->renderer, &tower_rect);
		
		/* Affichage du coût */
		sprintf(cost_text, "%d$", game->tower_buttons[i].cost);
		
		text_color = ui_color_white();
		if (game->money < game->tower_buttons[i].cost)
			text_color = ui_color_red();
			
		text_rect.x = game->tower_buttons[i].rect.x + 5;
		text_rect.y = game->tower_buttons[i].rect.y + game->tower_buttons[i].rect.h - 15;
		text_rect.w = 50;
		text_rect.h = 15;
		
		ui_render_text(game, cost_text, game->font_small, text_rect, text_color);
		
		i++;
	}
}

/**
 * @brief Affiche l'aperçu de la tour lors du placement
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	tower_preview_render(t_game *game)
{
	int			mouse_x;
	int			mouse_y;
	int			grid_x;
	int			grid_y;
	bool		valid_placement;
	int			center_x;
	int			center_y;
	SDL_Rect	preview_rect;
	float		range;
	SDL_Rect	range_rect;

	if (!game->placing_tower)
		return;
		
	SDL_GetMouseState(&mouse_x, &mouse_y);
	
	if (mouse_y >= GAME_AREA_HEIGHT)
		return;
		
	grid_x = mouse_x / GRID_SIZE;
	grid_y = mouse_y / GRID_SIZE;
	
	valid_placement = tower_can_be_placed(game, grid_x, grid_y);
	
	center_x = grid_x * GRID_SIZE + GRID_SIZE / 2;
	center_y = grid_y * GRID_SIZE + GRID_SIZE / 2;
	
	preview_rect.x = center_x - 15;
	preview_rect.y = center_y - 15;
	preview_rect.w = 30;
	preview_rect.h = 30;
	
	if (valid_placement)
	{
		if (game->selected_tower_type == TOWER_TYPE_BASIC)
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 180);
		else if (game->selected_tower_type == TOWER_TYPE_SLOW)
			SDL_SetRenderDrawColor(game->renderer, 0, 150, 200, 180);
		else if (game->selected_tower_type == TOWER_TYPE_MULTI)
			SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 180);
	}
	else
		SDL_SetRenderDrawColor(game->renderer, 200, 0, 0, 180);
		
	SDL_RenderFillRect(game->renderer, &preview_rect);
	
	/* Affichage de la portée */
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 100);
	
	range = 0.0f;
	if (game->selected_tower_type == TOWER_TYPE_BASIC)
		range = TOWER_BASIC_RANGE;
	else if (game->selected_tower_type == TOWER_TYPE_SLOW)
		range = TOWER_SLOW_RANGE;
	else if (game->selected_tower_type == TOWER_TYPE_MULTI)
		range = TOWER_MULTI_RANGE;
		
	range_rect.x = center_x - (int)range;
	range_rect.y = center_y - (int)range;
	range_rect.w = (int)range * 2;
	range_rect.h = (int)range * 2;
	
	SDL_RenderDrawRect(game->renderer, &range_rect);
}

/* Fonctions statiques internes */

/**
 * @brief Configure les propriétés d'une tour en fonction de son type
 * 
 * @param tower Pointeur vers la tour à configurer
 * @param type Type de la tour
 */
static void	set_tower_properties_by_type(t_tower *tower, int type)
{
	if (type == TOWER_TYPE_BASIC)
	{
		tower->cost = COST_TOWER_BASIC;
		tower->damage = TOWER_BASIC_DAMAGE;
		tower->range = TOWER_BASIC_RANGE;
		tower->fire_rate = TOWER_BASIC_RATE;
	}
	else if (type == TOWER_TYPE_SLOW)
	{
		tower->cost = COST_TOWER_SLOW;
		tower->damage = TOWER_SLOW_DAMAGE;
		tower->range = TOWER_SLOW_RANGE;
		tower->fire_rate = TOWER_SLOW_RATE;
	}
	else if (type == TOWER_TYPE_MULTI)
	{
		tower->cost = COST_TOWER_MULTI;
		tower->damage = TOWER_MULTI_DAMAGE;
		tower->range = TOWER_MULTI_RANGE;
		tower->fire_rate = TOWER_MULTI_RATE;
	}
}

/**
 * @brief Décale les indices des cibles pour insérer une nouvelle cible
 * 
 * @param target_indices Tableau des indices des cibles
 * @param target_distances Tableau des distances des cibles
 * @param insert_index Position d'insertion
 * @param targets_found Nombre de cibles déjà trouvées
 */
static void	shift_targets(int *target_indices, float *target_distances,
		int insert_index, int targets_found)
{
	int	k;

	k = targets_found - 1;
	while (k >= insert_index)
	{
		target_indices[k + 1] = target_indices[k];
		target_distances[k + 1] = target_distances[k];
		k--;
	}
}

/**
 * @brief Décale les indices des cibles et insère une nouvelle cible
 * 
 * @param target_indices Tableau des indices des cibles
 * @param target_distances Tableau des distances des cibles
 * @param insert_index Position d'insertion
 * @param targets_to_find Nombre total de cibles à trouver
 */
static void	shift_targets_insert(int *target_indices, float *target_distances,
		int insert_index, int targets_to_find)
{
	int	k;

	k = targets_to_find - 1;
	while (k > insert_index)
	{
		target_indices[k] = target_indices[k - 1];
		target_distances[k] = target_distances[k - 1];
		k--;
	}
}
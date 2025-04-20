/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:05:22 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:42:53 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"
#include "../../includes/entities/enemy.h"

/**
 * @brief Initialise l'état des entrées
 * 
 * @param input Pointeur vers la structure d'état des entrées
 */
void	input_init(t_input_state *input)
{
	int	i;

	if (!input)
		return;
		
	/* Réinitialisation des états des touches */
	for (i = 0; i < SDL_NUM_SCANCODES; i++)
		input->keys[i] = false;
		
	/* Réinitialisation des états des boutons de la souris */
	for (i = 0; i < 6; i++)
		input->mouse_buttons[i] = false;
		
	input->mouse_x = 0;
	input->mouse_y = 0;
	input->quit_requested = false;
}

/**
 * @brief Met à jour l'état des entrées
 * 
 * @param game Pointeur vers la structure du jeu
 * @param input Pointeur vers la structure d'état des entrées
 */
void	input_update(t_game *game, t_input_state *input)
{
	if (!game || !input)
		return;
		
	/* Mise à jour de la position de la souris */
	SDL_GetMouseState(&input->mouse_x, &input->mouse_y);
	
	/* Traitement des entrées selon l'état du jeu */
	if (game->state == STATE_PLAYING)
		input_handle_playing_state(game, input);
	else if (game->state == STATE_GAME_OVER)
		input_handle_game_over_state(game, input);
	else if (game->state == STATE_PAUSED)
		input_handle_paused_state(game, input);
		
	/* Gestion de la demande de fermeture */
	if (input->quit_requested)
		game->is_running = false;
}

/**
 * @brief Gère les événements SDL et met à jour l'état des entrées
 * 
 * @param game Pointeur vers la structure du jeu
 * @param input Pointeur vers la structure d'état des entrées
 */
void	input_handle_events(t_game *game, t_input_state *input)
{
	SDL_Event	event;
	
	if (!game || !input)
		return;
		
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			input->quit_requested = true;
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.scancode < SDL_NUM_SCANCODES)
				input->keys[event.key.keysym.scancode] = true;
				
			/* Touche Escape pour mettre en pause ou quitter */
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			{
				if (game->state == STATE_PLAYING)
					game->state = STATE_PAUSED;
				else if (game->state == STATE_PAUSED)
					game->state = STATE_PLAYING;
			}
		}
		else if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.scancode < SDL_NUM_SCANCODES)
				input->keys[event.key.keysym.scancode] = false;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button < 6)
				input->mouse_buttons[event.button.button] = true;
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button < 6)
				input->mouse_buttons[event.button.button] = false;
		}
	}
}

/**
 * @brief Traite les entrées pour l'état de jeu en cours
 * 
 * @param game Pointeur vers la structure du jeu
 * @param input Pointeur vers la structure d'état des entrées
 */
void	input_handle_playing_state(t_game *game, t_input_state *input)
{
	int		grid_x;
	int		grid_y;
	int		i;
	bool	clicked_on_button;
	
	/* Gestion des touches du clavier */
	if (input_is_key_just_pressed(input, SDL_SCANCODE_SPACE))
		enemy_spawn(game, ENEMY_TYPE_BASIC);
		
	if (input_is_key_just_pressed(input, SDL_SCANCODE_1))
	{
		if (game->money >= COST_TOWER_BASIC)
		{
			game->selected_tower_type = TOWER_TYPE_BASIC;
			game->placing_tower = true;
		}
	}
	else if (input_is_key_just_pressed(input, SDL_SCANCODE_2))
	{
		if (game->money >= COST_TOWER_SLOW)
		{
			game->selected_tower_type = TOWER_TYPE_SLOW;
			game->placing_tower = true;
		}
	}
	else if (input_is_key_just_pressed(input, SDL_SCANCODE_3))
	{
		if (game->money >= COST_TOWER_MULTI)
		{
			game->selected_tower_type = TOWER_TYPE_MULTI;
			game->placing_tower = true;
		}
	}
	
	/* Gestion du clic gauche */
	if (input_is_mouse_button_just_pressed(input, SDL_BUTTON_LEFT))
	{
		/* Vérifier d'abord si un bouton de la barre d'outils a été cliqué */
		clicked_on_button = false;
		
		/* Bouton de pause */
		if (ui_button_clicked(game->pause_button, input->mouse_x, input->mouse_y))
		{
			game->state = STATE_PAUSED;
			clicked_on_button = true;
		}
		
		/* Boutons de tour */
		i = 0;
		while (i < MAX_TOWER_TYPES && !clicked_on_button)
		{
			if (ui_button_clicked(game->tower_buttons[i].rect, 
					input->mouse_x, input->mouse_y))
			{
				if (game->money >= game->tower_buttons[i].cost)
				{
					game->selected_tower_type = game->tower_buttons[i].tower_type;
					game->placing_tower = true;
					clicked_on_button = true;
				}
			}
			i++;
		}
		
		/* Si aucun bouton n'a été cliqué et qu'on est en mode placement de tour */
		if (!clicked_on_button && game->placing_tower)
		{
			if (input->mouse_y < GAME_AREA_HEIGHT)
			{
				/* Convertir les coordonnées de la souris en coordonnées de grille */
				grid_x = input->mouse_x / GRID_SIZE;
				grid_y = input->mouse_y / GRID_SIZE;
				
				/* Placer la tour */
				tower_place(game, grid_x, grid_y);
			}
		}
	}
	
	/* Clic droit pour annuler le placement de tour */
	if (input_is_mouse_button_just_pressed(input, SDL_BUTTON_RIGHT))
	{
		game->placing_tower = false;
	}
}

/**
 * @brief Traite les entrées pour l'état de fin de jeu
 * 
 * @param game Pointeur vers la structure du jeu
 * @param input Pointeur vers la structure d'état des entrées
 */
void	input_handle_game_over_state(t_game *game, t_input_state *input)
{
	/* Touche Enter pour redémarrer */
	if (input_is_key_just_pressed(input, SDL_SCANCODE_RETURN))
	{
		game_reset(game);
	}
	
	/* Clic sur le bouton de redémarrage */
	if (input_is_mouse_button_just_pressed(input, SDL_BUTTON_LEFT))
	{
		if (ui_button_clicked(game->restart_button, input->mouse_x, input->mouse_y))
		{
			game_reset(game);
		}
	}
}

/**
 * @brief Traite les entrées pour l'état de pause
 * 
 * @param game Pointeur vers la structure du jeu
 * @param input Pointeur vers la structure d'état des entrées
 */
void	input_handle_paused_state(t_game *game, t_input_state *input)
{
	SDL_Rect	resume_button;
	SDL_Rect	quit_button;
	
	/* Définir les rectangles des boutons pour le menu de pause */
	resume_button.x = (WINDOW_WIDTH - 200) / 2;
	resume_button.y = WINDOW_HEIGHT / 2;
	resume_button.w = 200;
	resume_button.h = 50;
	
	quit_button.x = (WINDOW_WIDTH - 200) / 2;
	quit_button.y = WINDOW_HEIGHT / 2 + 70;
	quit_button.w = 200;
	quit_button.h = 50;
	
	/* Gestion des clics sur les boutons */
	if (input_is_mouse_button_just_pressed(input, SDL_BUTTON_LEFT))
	{
		/* Bouton Resume */
		if (ui_button_clicked(resume_button, input->mouse_x, input->mouse_y))
		{
			game->state = STATE_PLAYING;
		}
		
		/* Bouton Quit */
		if (ui_button_clicked(quit_button, input->mouse_x, input->mouse_y))
		{
			input->quit_requested = true;
		}
	}
}

/**
 * @brief Vérifie si une touche est actuellement enfoncée
 * 
 * @param input Pointeur vers la structure d'état des entrées
 * @param key Code de la touche à vérifier
 * @return true si la touche est enfoncée, false sinon
 */
bool	input_is_key_pressed(t_input_state *input, SDL_Scancode key)
{
	if (!input || key >= SDL_NUM_SCANCODES)
		return (false);
		
	return (input->keys[key]);
}

/**
 * @brief Vérifie si une touche vient d'être enfoncée (une seule fois)
 * Note: cette fonction n'est pas parfaite sans un système de détection de "just pressed"
 * Elle sera implémentée complètement dans une version future
 * 
 * @param input Pointeur vers la structure d'état des entrées
 * @param key Code de la touche à vérifier
 * @return true si la touche vient d'être enfoncée, false sinon
 */
bool	input_is_key_just_pressed(t_input_state *input, SDL_Scancode key)
{
	static bool		previous_keys[SDL_NUM_SCANCODES];
	bool			result;
	
	if (!input || key >= SDL_NUM_SCANCODES)
		return (false);
		
	result = input->keys[key] && !previous_keys[key];
	previous_keys[key] = input->keys[key];
	
	return (result);
}

/**
 * @brief Vérifie si un bouton de la souris est actuellement enfoncé
 * 
 * @param input Pointeur vers la structure d'état des entrées
 * @param button Bouton de la souris à vérifier
 * @return true si le bouton est enfoncé, false sinon
 */
bool	input_is_mouse_button_pressed(t_input_state *input, int button)
{
	if (!input || button >= 6)
		return (false);
		
	return (input->mouse_buttons[button]);
}

/**
 * @brief Vérifie si un bouton de la souris vient d'être enfoncé (une seule fois)
 * Note: cette fonction n'est pas parfaite sans un système de détection de "just pressed"
 * Elle sera implémentée complètement dans une version future
 * 
 * @param input Pointeur vers la structure d'état des entrées
 * @param button Bouton de la souris à vérifier
 * @return true si le bouton vient d'être enfoncé, false sinon
 */
bool	input_is_mouse_button_just_pressed(t_input_state *input, int button)
{
	static bool		previous_buttons[6];
	bool			result;
	
	if (!input || button >= 6)
		return (false);
		
	result = input->mouse_buttons[button] && !previous_buttons[button];
	previous_buttons[button] = input->mouse_buttons[button];
	
	return (result);
}

/**
 * @brief Convertit les coordonnées de la souris en coordonnées de grille
 * 
 * @param game Pointeur vers la structure du jeu
 * @param grid_x Pointeur pour stocker la coordonnée X de la grille
 * @param grid_y Pointeur pour stocker la coordonnée Y de la grille
 */
void	input_get_grid_position(t_game *game, int *grid_x, int *grid_y)
{
	int	mouse_x;
	int	mouse_y;
	
	(void)game; /* Supprime l'avertissement de paramètre inutilisé */
	
	SDL_GetMouseState(&mouse_x, &mouse_y);
	
	*grid_x = mouse_x / GRID_SIZE;
	*grid_y = mouse_y / GRID_SIZE;
	
	/* Assurer que les coordonnées sont dans les limites de la grille */
	if (*grid_x < 0)
		*grid_x = 0;
	else if (*grid_x >= GRID_COLS)
		*grid_x = GRID_COLS - 1;
		
	if (*grid_y < 0)
		*grid_y = 0;
	else if (*grid_y >= GRID_ROWS)
		*grid_y = GRID_ROWS - 1;
}
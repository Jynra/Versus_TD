/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:06:05 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:35:37 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <SDL2/SDL.h>
# include <stdbool.h>

typedef struct s_game t_game;

/* Structure pour stocker l'état des entrées */
typedef struct s_input_state
{
	bool	keys[SDL_NUM_SCANCODES];  /* État des touches du clavier */
	bool	mouse_buttons[6];         /* État des boutons de la souris */
	int		mouse_x;                  /* Position X de la souris */
	int		mouse_y;                  /* Position Y de la souris */
	bool	quit_requested;           /* Demande de fermeture du programme */
}	t_input_state;

/* Fonctions principales de gestion des entrées */
void	input_init(t_input_state *input);
void	input_update(t_game *game, t_input_state *input);
void	input_handle_events(t_game *game, t_input_state *input);

/* Fonctions de traitement des entrées selon l'état du jeu */
void	input_handle_playing_state(t_game *game, t_input_state *input);
void	input_handle_game_over_state(t_game *game, t_input_state *input);
void	input_handle_paused_state(t_game *game, t_input_state *input);

/* Fonctions utilitaires */
bool	input_is_key_pressed(t_input_state *input, SDL_Scancode key);
bool	input_is_key_just_pressed(t_input_state *input, SDL_Scancode key);
bool	input_is_mouse_button_pressed(t_input_state *input, int button);
bool	input_is_mouse_button_just_pressed(t_input_state *input, int button);
void	input_get_grid_position(t_game *game, int *grid_x, int *grid_y);

#endif
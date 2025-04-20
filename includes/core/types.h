/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:42 by student           #+#    #+#             */
/*   Updated: 2025/04/20 02:35:08 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <stdbool.h>
# include "config.h"  /* Assurez-vous que config.h est inclus */

/* États du jeu */
typedef enum e_game_state
{
	STATE_PLAYING,
	STATE_GAME_OVER,
	STATE_PAUSED        /* Ajout d'un état de pause */
}	t_game_state;

/* Path point structure for enemy path */
typedef struct s_path_point
{
	int	x;
	int	y;
}	t_path_point;

/* Enemy structure */
typedef struct s_enemy
{
	float		x;
	float		y;
	int			width;
	int			height;
	float		speed;
	float		base_speed;
	int			health;
	int			max_health;
	int			path_index;
	bool		active;
	SDL_Texture	*texture;
	bool		slowed;
	float		slow_timer;
	int         type;       /* Ajout du type d'ennemi */
	int         reward;     /* Récompense lorsque l'ennemi est vaincu */
	int         score;      /* Points de score lorsque l'ennemi est vaincu */
}	t_enemy;

/* Tower structure */
typedef struct s_tower
{
	float		x;
	float		y;
	int			width;
	int			height;
	int			cost;
	int			damage;
	float		range;
	float		fire_rate;
	float		fire_timer;
	bool		active;
	SDL_Texture	*texture;
	int         type;       /* Ajout du type de tour */
}	t_tower;

/* Projectile structure */
typedef struct s_projectile
{
	float		x;
	float		y;
	float		dx;
	float		dy;
	int			damage;
	float		speed;
	bool		active;
	SDL_Texture	*texture;
	int         source_type; /* Type de la tour qui a tiré ce projectile */
}	t_projectile;

/* Tower selection button structure */
typedef struct s_tower_button
{
	SDL_Rect	rect;
	int			tower_type;
	SDL_Texture	*texture;
	char		*description;
	int			cost;
}	t_tower_button;

/* Main game structure */
typedef struct s_game
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	bool			is_running;
	t_game_state	state;
	int				money;
	int				lives;
	int				score;
	int				round;
	int				grid[GRID_ROWS][GRID_COLS];
	t_enemy			enemies[MAX_ENEMIES];
	t_tower			towers[MAX_TOWERS];
	t_projectile	projectiles[MAX_PROJECTILES];
	t_path_point	path[20];
	int				path_length;
	SDL_Texture		*tower_texture;
	SDL_Texture		*enemy_texture;
	SDL_Texture		*projectile_texture;
	SDL_Texture		*background_texture;
	int				selected_tower_type;
	bool			placing_tower;
	t_tower_button	tower_buttons[MAX_TOWER_TYPES];
	/* Bouton de redémarrage pour l'écran de Game Over */
	SDL_Rect		restart_button;
	/* Polices pour le texte */
	TTF_Font		*font_small;
	TTF_Font		*font_medium;
	TTF_Font		*font_large;
	/* Nouveau membre pour le temps de jeu */
	float           game_time;      /* Temps écoulé depuis le début de la partie */
	/* Bouton de pause */
	SDL_Rect        pause_button;
	/* Tableaux pour les ressources - définition à 11 et 3 comme valeurs fixes */
	SDL_Texture     *textures[11];  /* Pour les 11 textures disponibles */
	TTF_Font        *fonts[3];      /* Pour les 3 polices disponibles */
}	t_game;

#endif
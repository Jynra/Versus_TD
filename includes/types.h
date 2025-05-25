/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 21:32:06 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include <stdbool.h>
# include "config.h"

/* Basic 2D Vector */
typedef struct s_vector2
{
	float	x;
	float	y;
}	t_vector2;

/* Integer Rectangle */
typedef struct s_rect
{
	int		x;
	int		y;
	int		w;
	int		h;
}	t_rect;

/* Color Structure */
typedef struct s_color
{
	int		r;
	int		g;
	int		b;
	int		a;
}	t_color;

/* Base Entity Structure */
typedef struct s_entity
{
	t_vector2	pos;
	t_vector2	vel;
	float		radius;
	int			health;
	int			max_health;
	int			type;
	bool		active;
	float		rotation;
}	t_entity;

/* Tower Structure - ENHANCED FOR PHASE 4 */
typedef struct s_tower
{
	t_entity	base;
	float		range;
	int			damage;
	float		fire_rate;
	float		fire_timer;
	int			level;
	int			kills;
	int			cost;
	int			total_damage_dealt;
	bool		selected;
	int			upgrade_cost;
	bool		explosive;          /* NEW: For cannon upgrades */
	float		explosion_radius;   /* NEW: For explosion range */
}	t_tower;

/* Enemy Structure */
typedef struct s_enemy
{
	t_entity	base;
	float		speed;
	float		base_speed;
	int			path_index;
	int			reward;
	int			score_value;
	bool		slowed;
	float		slow_timer;
	bool		flying;
	float		spawn_time;
}	t_enemy;

/* Projectile Structure - ENHANCED FOR PHASE 4 */
typedef struct s_projectile
{
	t_entity	base;
	int			damage;
	int			target_id;
	float		lifetime;
	bool		piercing;
	bool		explosive;          /* FIXED: Explicit explosive flag */
	float		explosion_radius;
}	t_projectile;

/* Particle Structure */
typedef struct s_particle
{
	t_vector2	pos;
	t_vector2	vel;
	float		life;
	float		max_life;
	float		size;
	float		start_size;
	t_color		color;
	t_color		start_color;
	int			type;
	bool		active;
	float		gravity;
}	t_particle;

/* Notification Structure */
typedef struct s_notification
{
	char		text[256];
	float		timer;
	float		max_time;
	int			type;
	bool		active;
	float		alpha;
	t_vector2	pos;
}	t_notification;

/* Wave Structure */
typedef struct s_wave
{
	int			number;
	int			enemies_total;
	int			enemies_spawned;
	int			enemies_alive;
	float		spawn_delay;
	float		spawn_timer;
	float		prep_timer;
	bool		active;
	bool		completed;
	bool		preparing;
	int			enemy_types[4];
	int			enemy_counts[4];
	int			current_spawn_type;
}	t_wave;

/* Path Point Structure */
typedef struct s_path_point
{
	t_vector2	pos;
	t_vector2	direction;
}	t_path_point;

/* UI Button Structure */
typedef struct s_button
{
	t_rect		rect;
	char		text[32];
	int			type;
	bool		enabled;
	bool		pressed;
	bool		hovered;
	t_color		color;
}	t_button;

/* Game Statistics */
typedef struct s_stats
{
	int			total_enemies_killed;
	int			total_money_earned;
	int			total_damage_dealt;
	int			towers_built;
	int			waves_completed;
	float		play_time;
	int			highest_wave;
	int			best_score;
}	t_stats;

/* Game Input State */
typedef struct s_input
{
	t_vector2	mouse_pos;
	t_vector2	mouse_world_pos;
	bool		mouse_left_down;
	bool		mouse_right_down;
	bool		mouse_left_clicked;
	bool		mouse_right_clicked;
	bool		keys[SDL_NUM_SCANCODES];
	bool		keys_pressed[SDL_NUM_SCANCODES];
}	t_input;

/* Grid Cell Structure */
typedef struct s_grid_cell
{
	int			type;
	bool		occupied;
	int			tower_id;
	float		movement_cost;
}	t_grid_cell;

/* Main Game Structure - ENHANCED FOR PHASE 4 */
typedef struct s_game
{
	/* SDL Components */
	SDL_Window		*window;
	SDL_Renderer	*renderer;
	TTF_Font		*font_small;
	TTF_Font		*font_medium;
	TTF_Font		*font_large;

	/* Game State */
	bool			running;
	bool			paused;
	int				state;
	float			delta_time;
	Uint32			last_frame_time;

	/* Game Resources */
	int				money;
	int				lives;
	int				score;
	int				multiplier;

	/* Entities Arrays */
	t_tower			towers[MAX_TOWERS];
	t_enemy			enemies[MAX_ENEMIES];
	t_projectile	projectiles[MAX_PROJECTILES];
	t_particle		particles[MAX_PARTICLES];
	t_notification	notifications[MAX_NOTIFICATIONS];

	/* Game Systems */
	t_wave			current_wave;
	t_input			input;
	t_stats			stats;

	/* World Data */
	t_grid_cell		grid[GRID_ROWS][GRID_COLS];
	t_path_point	path[MAX_PATH_POINTS];
	int				path_length;

	/* UI State */
	int				selected_tower_type;
	bool			placing_tower;
	int				selected_tower_id;
	t_button		tower_buttons[4];
	t_button		ui_buttons[8];
	bool			show_ranges;
	bool			show_debug;

	/* Performance Counters - ENHANCED FOR PHASE 4 */
	int				active_towers;
	int				active_enemies;
	int				active_projectiles;
	int				active_particles;    /* ADDED: Track active particles */
	float			fps;
	int				frame_count;
	float			fps_timer;
}	t_game;

/* Function Pointer Types */
typedef void	(*t_update_func)(void *entity, t_game *game);
typedef void	(*t_render_func)(void *entity, t_game *game);
typedef void	(*t_cleanup_func)(void *entity);

/* Entity System Function Table */
typedef struct s_entity_funcs
{
	t_update_func	update;
	t_render_func	render;
	t_cleanup_func	cleanup;
}	t_entity_funcs;

#endif
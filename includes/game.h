/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:42 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:22:07 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "config.h"
# include "types.h"
# include "utils.h"

/* Initialization and cleanup functions */
bool	init_sdl(t_game *game);
void	cleanup_sdl(t_game *game);
void	init_game(t_game *game);
void	reset_game(t_game *game);
void	init_tower_buttons(t_game *game);
void	init_restart_button(t_game *game);
bool	init_fonts(t_game *game);

/* Interface functions */
void	render_toolbar(t_game *game);
void	render_game_area(t_game *game);
bool	point_in_rect(int x, int y, SDL_Rect rect);

/* Main game functions */
void	handle_events(t_game *game);
void	update(t_game *game);
void	render(t_game *game);

/* Entity management functions */
void	spawn_enemy(t_game *game);
void	place_tower(t_game *game, int grid_x, int grid_y);
void	update_towers(t_game *game);
void	update_enemies(t_game *game);
void	update_projectiles(t_game *game);

/* Color helpers */
SDL_Color	get_white_color(void);
SDL_Color	get_black_color(void);
SDL_Color	get_red_color(void);
SDL_Color	get_green_color(void);

#endif
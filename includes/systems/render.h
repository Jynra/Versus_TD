/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:04:02 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:35:31 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include <SDL2/SDL.h>
# include <stdbool.h>

typedef struct s_game t_game;

/* Main rendering functions */
void	render_init(t_game *game);
void	render_clear(t_game *game);
void	render_present(t_game *game);

/* Game area rendering */
void	render_game_area(t_game *game);
void	render_grid(t_game *game);
void	render_background(t_game *game);

/* Special effects */
void	render_explosion(t_game *game, float x, float y, float scale);
void	render_slow_effect(t_game *game, float x, float y, float radius);

/* Debug rendering */
void	render_debug_info(t_game *game, bool show_fps, bool show_grid_coords);
void	render_tower_range(t_game *game, int tower_index, SDL_Color color);

#endif
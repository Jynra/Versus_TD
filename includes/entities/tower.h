/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tower.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:48:22 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:21:29 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOWER_H
# define TOWER_H

# include <SDL2/SDL.h>
# include <stdbool.h>
# include "../core/config.h"
# include "../core/types.h"

/* Fonctions d'initialisation de tours */
void	tower_init(t_tower *tower, int type, float x, float y);
void	tower_buttons_init(t_game *game);

/* Fonctions de gestion des tours */
bool	tower_can_be_placed(t_game *game, int grid_x, int grid_y);
void	tower_place(t_game *game, int grid_x, int grid_y);
void	towers_update(t_game *game);
void	tower_find_targets(t_game *game, t_tower *tower, int targets_to_find, 
			int *target_indices);
void	tower_fire_at_target(t_game *game, t_tower *tower, int target_index);

/* Fonctions de rendu */
void	towers_render(t_game *game);
void	tower_buttons_render(t_game *game);
void	tower_preview_render(t_game *game);

#endif
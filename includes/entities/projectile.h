/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:52:25 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:21:35 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROJECTILE_H
# define PROJECTILE_H

# include <SDL2/SDL.h>
# include <stdbool.h>
# include "../core/types.h"

/* Fonctions d'initialisation */
void	projectile_init(t_projectile *projectile, float x, float y, 
			float target_x, float target_y, int damage, int source_type);

/* Fonctions de gestion des projectiles */
void	projectile_create(t_game *game, float x, float y, 
			float target_x, float target_y, int damage, int source_type);
void	projectiles_update(t_game *game);
bool	projectile_check_collision(t_game *game, t_projectile *projectile);

/* Fonctions de rendu */
void	projectiles_render(t_game *game);

#endif
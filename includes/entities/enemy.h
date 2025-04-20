/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:50:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:19:58 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENEMY_H
# define ENEMY_H

# include <SDL2/SDL.h>
# include <stdbool.h>
# include "../core/types.h"
# include "../core/config.h"

/* Fonctions d'initialisation */
void	enemy_init(t_enemy *enemy, int type, t_path_point start_point);
void	path_init(t_game *game);

/* Fonctions de gestion des ennemis */
void	enemy_spawn(t_game *game, int type);
void	enemies_update(t_game *game);
void	enemy_take_damage(t_game *game, t_enemy *enemy, int damage);
void	enemy_apply_slow(t_enemy *enemy, float slow_effect, float duration);

/* Fonctions de rendu */
void	enemies_render(t_game *game);

#endif
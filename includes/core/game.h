/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 10:55:18 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:20:12 by ellucas          ###   ########.fr       */
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
# include "../entities/enemy.h"
# include "../entities/tower.h"
# include "../entities/projectile.h"
# include "../systems/ui.h"
# include "../systems/render.h"
# include "../systems/input.h"
# include "../utils/math_utils.h"
# include "../utils/asset_loader.h"

/* Fonctions d'initialisation et de nettoyage */
bool	game_init(t_game *game);
void	game_cleanup(t_game *game);
void	game_reset(t_game *game);

/* Fonctions principales de la boucle de jeu */
void	game_handle_events(t_game *game);
void	game_update(t_game *game, float delta_time);
void	game_render(t_game *game);

/* Fonctions utilitaires pour le jeu */
void	game_start_round(t_game *game);
bool	game_check_end_condition(t_game *game);
void	game_add_score(t_game *game, int points);
void	game_add_money(t_game *game, int amount);

#endif
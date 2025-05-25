/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 12:36:40 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "types.h"
# include "entities.h"
# include "systems.h"
# include "utils.h"

/* Core Game Functions */
bool	game_init(t_game *game);
void	game_run(t_game *game);
void	game_cleanup(t_game *game);
void	game_shutdown(t_game *game);

/* Game Loop Functions */
void	game_update(t_game *game);
void	game_render(t_game *game);
void	game_handle_events(t_game *game);

/* Game State Management */
void	game_set_state(t_game *game, int state);
int		game_get_state(t_game *game);
bool	game_is_paused(t_game *game);
void	game_toggle_pause(t_game *game);

/* Game Resources Management */
void	game_add_money(t_game *game, int amount);
bool	game_spend_money(t_game *game, int amount);
void	game_lose_life(t_game *game);
void	game_add_score(t_game *game, int points);

/* Game Utility Functions */
float	game_get_delta_time(t_game *game);
void	game_update_delta_time(t_game *game);
bool	game_is_running(t_game *game);

/* Game World Functions */
void	game_init_world(t_game *game);
void	game_init_path(t_game *game);
void	game_init_grid(t_game *game);
bool	game_is_position_valid(t_game *game, t_vector2 pos);

/* Game Statistics */
void	game_update_stats(t_game *game);
void	game_reset_stats(t_game *game);

#endif
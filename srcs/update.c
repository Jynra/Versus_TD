/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:10:42 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:10:39 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

void	update_playing_state(t_game *game)
{
	update_towers(game);
	update_enemies(game);
	update_projectiles(game);
	
	if (game->lives <= 0)
	{
		printf("Game Over! Score final: %d\n", game->score);
		game->state = STATE_GAME_OVER;
	}
}

void	update_game_over_state(t_game *game)
{
	/* Pas de mise à jour nécessaire en état Game Over */
	(void)game;
}

void	update(t_game *game)
{
	if (game->state == STATE_PLAYING)
		update_playing_state(game);
	else if (game->state == STATE_GAME_OVER)
		update_game_over_state(game);
}
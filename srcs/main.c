/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:50:15 by student           #+#    #+#             */
/*   Updated: 2025/04/19 01:41:45 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

int	main(void)
{
	t_game	game;
	Uint32	frame_start;
	int		frame_time;

	if (!init_sdl(&game))
		return (1);
	init_game(&game);
	while (game.is_running)
	{
		frame_start = SDL_GetTicks();
		handle_events(&game);
		update(&game);
		render(&game);
		frame_time = SDL_GetTicks() - frame_start;
		if (FRAME_DELAY > frame_time)
			SDL_Delay(FRAME_DELAY - frame_time);
	}
	cleanup_sdl(&game);
	return (0);
}
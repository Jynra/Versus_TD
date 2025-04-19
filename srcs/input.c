/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:43:25 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:11:57 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

bool	point_in_rect(int x, int y, SDL_Rect rect)
{
	return (x >= rect.x && x < rect.x + rect.w
		&& y >= rect.y && y < rect.y + rect.h);
}

void	handle_key_press(t_game *game, SDL_KeyboardEvent key_event)
{
	if (game->state == STATE_PLAYING)
	{
		if (key_event.keysym.sym == SDLK_ESCAPE)
		{
			if (game->placing_tower)
				game->placing_tower = false;
		}
		else if (key_event.keysym.sym == SDLK_1)
		{
			if (game->money >= COST_TOWER_BASIC)
			{
				game->selected_tower_type = TOWER_TYPE_BASIC;
				game->placing_tower = true;
			}
		}
		else if (key_event.keysym.sym == SDLK_2)
		{
			if (game->money >= COST_TOWER_SLOW)
			{
				game->selected_tower_type = TOWER_TYPE_SLOW;
				game->placing_tower = true;
			}
		}
		else if (key_event.keysym.sym == SDLK_3)
		{
			if (game->money >= COST_TOWER_MULTI)
			{
				game->selected_tower_type = TOWER_TYPE_MULTI;
				game->placing_tower = true;
			}
		}
		else if (key_event.keysym.sym == SDLK_SPACE)
			spawn_enemy(game);
	}
	else if (game->state == STATE_GAME_OVER)
	{
		if (key_event.keysym.sym == SDLK_RETURN)
		{
			reset_game(game);
		}
	}
}

void	handle_playing_state_mouse(t_game *game, SDL_MouseButtonEvent button_event)
{
	int	mouse_x;
	int	mouse_y;
	int	i;
	int	grid_x;
	int	grid_y;
	bool clicked_on_button;

	if (button_event.button != SDL_BUTTON_LEFT)
		return ;
	mouse_x = button_event.x;
	mouse_y = button_event.y;
	clicked_on_button = false;
	i = 0;
	while (i < MAX_TOWER_TYPES)
	{
		if (point_in_rect(mouse_x, mouse_y, game->tower_buttons[i].rect))
		{
			if (game->money >= game->tower_buttons[i].cost)
			{
				game->selected_tower_type = game->tower_buttons[i].tower_type;
				game->placing_tower = true;
				clicked_on_button = true;
			}
			break ;
		}
		i++;
	}
	if (!clicked_on_button && game->placing_tower)
	{
		if (mouse_y < GAME_AREA_HEIGHT)
		{
			grid_x = mouse_x / GRID_SIZE;
			grid_y = mouse_y / GRID_SIZE;
			place_tower(game, grid_x, grid_y);
		}
	}
}

void	handle_game_over_state_mouse(t_game *game, SDL_MouseButtonEvent button_event)
{
	int	mouse_x;
	int	mouse_y;

	if (button_event.button != SDL_BUTTON_LEFT)
		return ;
	
	mouse_x = button_event.x;
	mouse_y = button_event.y;
	
	if (point_in_rect(mouse_x, mouse_y, game->restart_button))
	{
		reset_game(game);
	}
}

void	handle_mouse_button_down(t_game *game, SDL_MouseButtonEvent button_event)
{
	if (game->state == STATE_PLAYING)
		handle_playing_state_mouse(game, button_event);
	else if (game->state == STATE_GAME_OVER)
		handle_game_over_state_mouse(game, button_event);
}

void	handle_events(t_game *game)
{
	SDL_Event	event;

	if (!game)
	{
		fprintf(stderr, "ERROR: Pointeur de jeu nul dans handle_events\n");
		return ;
	}
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			game->is_running = false;
		else if (event.type == SDL_KEYDOWN)
			handle_key_press(game, event.key);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			handle_mouse_button_down(game, event.button);
		else if (event.type == SDL_MOUSEBUTTONUP
			&& event.button.button == SDL_BUTTON_RIGHT
			&& game->state == STATE_PLAYING)
			game->placing_tower = false;
	}
}
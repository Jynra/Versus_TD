/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 22:07:25 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"
#include "entities.h"

static void	handle_tower_placement(t_game *game, t_vector2 click_pos);
static void	handle_ui_click(t_game *game, t_vector2 click_pos);

void	input_init(t_input *input)
{
	int	i;

	if (!validate_pointer(input))
		return ;
	input->mouse_pos = vec2_zero();
	input->mouse_world_pos = vec2_zero();
	input->mouse_left_down = false;
	input->mouse_right_down = false;
	input->mouse_left_clicked = false;
	input->mouse_right_clicked = false;
	i = 0;
	while (i < SDL_NUM_SCANCODES)
	{
		input->keys[i] = false;
		input->keys_pressed[i] = false;
		i++;
	}
	debug_info("Input system initialized");
}

void	input_update(t_input *input)
{
	int	i;

	if (!validate_pointer(input))
		return ;
	input->mouse_left_clicked = false;
	input->mouse_right_clicked = false;
	i = 0;
	while (i < SDL_NUM_SCANCODES)
	{
		input->keys_pressed[i] = false;
		i++;
	}
}

void	input_handle_event(t_input *input, SDL_Event *event, t_game *game)
{
	t_vector2	click_pos;

	if (!validate_pointer(input) || !validate_pointer(event))
		return ;
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			input->mouse_left_down = true;
			input->mouse_left_clicked = true;
			click_pos = vec2_create((float)event->button.x, 
				(float)event->button.y);
			if (game && click_pos.y >= GAME_AREA_HEIGHT)
				handle_ui_click(game, click_pos);
			else if (game)
				handle_tower_placement(game, click_pos);
		}
		else if (event->button.button == SDL_BUTTON_RIGHT)
		{
			input->mouse_right_down = true;
			input->mouse_right_clicked = true;
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
			input->mouse_left_down = false;
		else if (event->button.button == SDL_BUTTON_RIGHT)
			input->mouse_right_down = false;
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		input->mouse_pos.x = (float)event->motion.x;
		input->mouse_pos.y = (float)event->motion.y;
		input->mouse_world_pos = input->mouse_pos;
	}
	else if (event->type == SDL_KEYDOWN && !event->key.repeat)
	{
		input->keys[event->key.keysym.scancode] = true;
		input->keys_pressed[event->key.keysym.scancode] = true;
	}
	else if (event->type == SDL_KEYUP)
		input->keys[event->key.keysym.scancode] = false;
}

static void	handle_tower_placement(t_game *game, t_vector2 click_pos)
{
	t_vector2	grid_center;
	int			grid_x;
	int			grid_y;
	t_tower		*clicked_tower;

	clicked_tower = get_tower_at_position(game, click_pos);
	if (clicked_tower)
	{
		int tower_id = clicked_tower - game->towers;
		select_tower(game, tower_id);
		debug_info("Tower selected at (%.1f, %.1f)", 
			clicked_tower->base.pos.x, clicked_tower->base.pos.y);
		return ;
	}
	if (game->placing_tower && game->selected_tower_type > 0)
	{
		grid_x = world_to_grid_x(click_pos.x);
		grid_y = world_to_grid_y(click_pos.y);
		grid_center.x = grid_to_world_x(grid_x);
		grid_center.y = grid_to_world_y(grid_y);
		if (spawn_tower(game, grid_center, game->selected_tower_type))
		{
			debug_info("Tower placed successfully at grid (%d, %d)", 
				grid_x, grid_y);
		}
		else
		{
			debug_warning("Failed to place tower at grid (%d, %d)", 
				grid_x, grid_y);
		}
	}
	else
	{
		deselect_all_towers(game);
	}
}

static void	handle_ui_click(t_game *game, t_vector2 click_pos)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (rect_contains_point(game->tower_buttons[i].rect, click_pos))
		{
			game->selected_tower_type = game->tower_buttons[i].type;
			game->placing_tower = true;
			deselect_all_towers(game);
			debug_info("Tower type selected: %d", game->selected_tower_type);
			return ;
		}
		i++;
	}
}
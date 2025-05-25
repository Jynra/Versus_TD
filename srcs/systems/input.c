/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 14:07:25 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"

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

void	input_handle_event(t_input *input, SDL_Event *event)
{
	if (!validate_pointer(input) || !validate_pointer(event))
		return ;
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			input->mouse_left_down = true;
			input->mouse_left_clicked = true;
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

void	effects_init(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < MAX_PARTICLES)
	{
		game->particles[i].active = false;
		i++;
	}
	debug_info("Effects system initialized");
}

void	waves_init(t_wave *wave)
{
	int	i;

	if (!validate_pointer(wave))
		return ;
	wave->number = 1;
	wave->enemies_total = WAVE_BASE_ENEMIES;
	wave->enemies_spawned = 0;
	wave->enemies_alive = 0;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE;
	wave->spawn_timer = 0.0f;
	wave->prep_timer = 0.0f;
	wave->active = false;
	wave->completed = false;
	wave->preparing = true;
	wave->current_spawn_type = 0;
	i = 0;
	while (i < 4)
	{
		wave->enemy_types[i] = ENEMY_BASIC;
		wave->enemy_counts[i] = 0;
		i++;
	}
	wave->enemy_types[0] = ENEMY_BASIC;
	wave->enemy_counts[0] = wave->enemies_total;
	debug_info("Wave system initialized");
}

void	notifications_init(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < MAX_NOTIFICATIONS)
	{
		game->notifications[i].active = false;
		i++;
	}
	debug_info("Notification system initialized");
}

void	ui_init(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < 4)
	{
		game->tower_buttons[i].rect = rect_create(
			10 + i * (UI_BUTTON_WIDTH + UI_BUTTON_PADDING),
			GAME_AREA_HEIGHT + UI_BUTTON_PADDING,
			UI_BUTTON_WIDTH,
			UI_BUTTON_HEIGHT
		);
		game->tower_buttons[i].type = i + 1;
		game->tower_buttons[i].enabled = true;
		game->tower_buttons[i].pressed = false;
		game->tower_buttons[i].hovered = false;
		game->tower_buttons[i].color = color_gray();
		i++;
	}
	string_copy(game->tower_buttons[0].text, "Basic");
	string_copy(game->tower_buttons[1].text, "Sniper");
	string_copy(game->tower_buttons[2].text, "Cannon");
	string_copy(game->tower_buttons[3].text, "Freeze");
	debug_info("UI system initialized");
}
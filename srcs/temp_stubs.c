/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_stubs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:34:04 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/* Temporary stub implementations for functions not yet implemented */

/* Entity functions - Phase 3 */
void	entities_update_all(t_game *game)
{
	(void)game;
	/* TODO: Implement entity updates in Phase 3 */
}

void	entities_render_all(t_game *game)
{
	(void)game;
	/* TODO: Implement entity rendering in Phase 3 */
}

int	entities_count_active_towers(t_game *game)
{
	int	count;
	int	i;

	if (!validate_pointer(game))
		return (0);
	count = 0;
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
			count++;
		i++;
	}
	return (count);
}

int	entities_count_active_enemies(t_game *game)
{
	int	count;
	int	i;

	if (!validate_pointer(game))
		return (0);
	count = 0;
	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].base.active)
			count++;
		i++;
	}
	return (count);
}

/* Wave system functions */
void	waves_update(t_wave *wave, t_game *game)
{
	(void)wave;
	(void)game;
	/* TODO: Implement wave logic in Phase 3 */
}

/* Effects system functions */
void	effects_update(t_game *game)
{
	(void)game;
	/* TODO: Implement effects updates in Phase 3 */
}

void	effects_render(t_game *game)
{
	(void)game;
	/* TODO: Implement effects rendering in Phase 3 */
}

/* Notification system functions */
void	notifications_update(t_game *game)
{
	(void)game;
	/* TODO: Implement notifications updates in Phase 3 */
}

void	notifications_render(t_game *game)
{
	(void)game;
	/* TODO: Implement notifications rendering in Phase 3 */
}

/* UI system functions */
void	ui_update(t_game *game)
{
	(void)game;
	/* TODO: Implement UI updates in Phase 3 */
}

void	ui_render(t_game *game)
{
	int		i;
	t_color	button_color;
	t_color	text_color;
	t_vector2	text_pos;

	if (!validate_pointer(game))
		return ;
	/* Simple tower button rendering */
	i = 0;
	while (i < 4)
	{
		button_color = game->tower_buttons[i].enabled ? 
			color_create(100, 100, 150, 255) : color_create(50, 50, 50, 255);
		text_color = color_white();
		render_rect(game, game->tower_buttons[i].rect, button_color);
		text_pos.x = game->tower_buttons[i].rect.x + 5;
		text_pos.y = game->tower_buttons[i].rect.y + 5;
		if (game->font_small)
			render_text(game, game->tower_buttons[i].text, text_pos, 
				text_color, game->font_small);
		i++;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_stubs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 22:30:20 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/* ========================================================================== */
/* STUBS PHASE 4 - Pour build stable uniquement                             */
/* ========================================================================== */

/* Effects System Stubs */
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
	debug_info("Effects system initialized (STUB)");
}

void	effects_update(t_game *game)
{
	(void)game;
	/* Stub - no visual effects in stable build */
}

void	effects_render(t_game *game)
{
	(void)game;
	/* Stub - no visual effects in stable build */
}

void	effects_create_explosion(t_game *game, t_vector2 pos, int intensity)
{
	(void)game;
	(void)pos;
	(void)intensity;
	/* Stub - no visual effects in stable build */
}

void	effects_create_blood(t_game *game, t_vector2 pos)
{
	(void)game;
	(void)pos;
	/* Stub - no visual effects in stable build */
}

void	effects_create_smoke(t_game *game, t_vector2 pos)
{
	(void)game;
	(void)pos;
	/* Stub - no visual effects in stable build */
}

void	effects_create_sparks(t_game *game, t_vector2 pos, int count)
{
	(void)game;
	(void)pos;
	(void)count;
	/* Stub - no visual effects in stable build */
}

void	effects_create_freeze_aura(t_game *game, t_vector2 pos, float radius)
{
	(void)game;
	(void)pos;
	(void)radius;
	/* Stub - no visual effects in stable build */
}

/* Wave System Stubs */
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
	debug_info("Wave system initialized (STUB)");
}

void	waves_update(t_wave *wave, t_game *game)
{
	/* Wave updates are now handled by spawner_update() */
	(void)wave;
	(void)game;
}

/* UI System Stubs */
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
	debug_info("UI system initialized (STUB)");
}

void	ui_update(t_game *game)
{
	(void)game;
	/* Stub UI update */
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
	
	/* Game stats display */
	if (game->show_debug)
	{
		char stats_text[256];
		sprintf(stats_text, "Wave: %d | Enemies: %d | Money: %d | Lives: %d", 
			game->current_wave.number, 
			entities_count_active_enemies(game),
			game->money, 
			game->lives);
		text_pos = vec2_create(10, GAME_AREA_HEIGHT + 50);
		if (game->font_small)
			render_text(game, stats_text, text_pos, color_white(), game->font_small);
	}
}
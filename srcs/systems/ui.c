/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:30:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 19:27:17 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"
#include "game.h"

static void	ui_render_tower_tooltip(t_game *game, int tower_type, t_vector2 pos);
static void	ui_render_selected_tower_info(t_game *game);
static void	ui_render_game_stats(t_game *game);
static void	ui_render_wave_progress(t_game *game);
static void	ui_render_resources(t_game *game);

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
	debug_info("Advanced UI system initialized");
}

void	ui_update(t_game *game)
{
	int			i;
	t_vector2	mouse_pos;

	if (!validate_pointer(game))
		return ;
	mouse_pos = game->input.mouse_pos;
	i = 0;
	while (i < 4)
	{
		ui_button_update(&game->tower_buttons[i], mouse_pos);
		i++;
	}
}

void	ui_button_update(t_button *button, t_vector2 mouse_pos)
{
	if (!validate_pointer(button))
		return ;
	button->hovered = rect_contains_point(button->rect, mouse_pos);
	if (button->hovered)
		button->color = color_create(120, 120, 180, 255);
	else
		button->color = color_create(100, 100, 150, 255);
}

void	ui_render(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	ui_render_hud(game);
	ui_render_tower_buttons(game);
	if (game->selected_tower_id >= 0)
		ui_render_selected_tower_info(game);
}

void	ui_render_hud(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	ui_render_resources(game);
	ui_render_game_stats(game);
	ui_render_wave_progress(game);
}

void	ui_render_tower_buttons(t_game *game)
{
	int		i;
	t_color	button_color;
	t_color	text_color;
	t_color	border_color;
	t_vector2	text_pos;
	t_vector2	cost_pos;
	char	cost_text[32];
	int		tower_costs[4] = {TOWER_BASIC_COST, TOWER_SNIPER_COST, 
		TOWER_CANNON_COST, TOWER_FREEZE_COST};

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < 4)
	{
		/* Determine button colors based on affordability and selection */
		if (game->money < tower_costs[i])
		{
			button_color = color_create(80, 80, 80, 255);
			text_color = color_create(120, 120, 120, 255);
			border_color = color_create(60, 60, 60, 255);
		}
		else if (game->selected_tower_type == i + 1)
		{
			button_color = color_create(200, 200, 100, 255);
			text_color = color_black();
			border_color = color_create(255, 255, 0, 255);
		}
		else if (game->tower_buttons[i].hovered)
		{
			button_color = color_create(150, 150, 200, 255);
			text_color = color_white();
			border_color = color_create(200, 200, 255, 255);
		}
		else
		{
			button_color = color_create(100, 100, 150, 255);
			text_color = color_white();
			border_color = color_create(80, 80, 120, 255);
		}
		
		/* Render button background and border */
		render_rect(game, game->tower_buttons[i].rect, button_color);
		render_rect(game, rect_create(game->tower_buttons[i].rect.x - 2,
			game->tower_buttons[i].rect.y - 2,
			game->tower_buttons[i].rect.w + 4,
			game->tower_buttons[i].rect.h + 4), border_color);
		render_rect(game, game->tower_buttons[i].rect, button_color);
		
		/* Render tower name */
		text_pos.x = game->tower_buttons[i].rect.x + 5;
		text_pos.y = game->tower_buttons[i].rect.y + 5;
		if (game->font_small)
			render_text(game, game->tower_buttons[i].text, text_pos, 
				text_color, game->font_small);
		
		/* Render cost */
		sprintf(cost_text, "$%d", tower_costs[i]);
		cost_pos.x = game->tower_buttons[i].rect.x + 5;
		cost_pos.y = game->tower_buttons[i].rect.y + 35;
		if (game->font_small)
			render_text(game, cost_text, cost_pos, text_color, game->font_small);
		
		/* Render tooltip on hover */
		if (game->tower_buttons[i].hovered)
		{
			t_vector2 tooltip_pos = vec2_create(
				game->tower_buttons[i].rect.x,
				game->tower_buttons[i].rect.y - 80
			);
			ui_render_tower_tooltip(game, i + 1, tooltip_pos);
		}
		i++;
	}
}

static void	ui_render_tower_tooltip(t_game *game, int tower_type, t_vector2 pos)
{
	t_rect	tooltip_rect;
	char	damage_text[64];
	char	range_text[64];
	char	rate_text[64];
	t_color	bg_color;
	t_color	border_color;
	t_color	text_color;
	t_vector2	text_pos;

	tooltip_rect = rect_create((int)pos.x, (int)pos.y, 120, 70);
	bg_color = color_create(40, 40, 40, 220);
	border_color = color_create(100, 100, 100, 255);
	text_color = color_white();
	
	/* Render tooltip background */
	render_rect(game, tooltip_rect, bg_color);
	render_rect(game, rect_create(tooltip_rect.x - 1, tooltip_rect.y - 1,
		tooltip_rect.w + 2, tooltip_rect.h + 2), border_color);
	render_rect(game, tooltip_rect, bg_color);
	
	/* Prepare tooltip text based on tower type */
	if (tower_type == TOWER_BASIC)
	{
		sprintf(damage_text, "Damage: %d", TOWER_BASIC_DAMAGE);
		sprintf(range_text, "Range: %.0f", TOWER_BASIC_RANGE);
		sprintf(rate_text, "Rate: %.1f/s", TOWER_BASIC_FIRE_RATE);
	}
	else if (tower_type == TOWER_SNIPER)
	{
		sprintf(damage_text, "Damage: %d", TOWER_SNIPER_DAMAGE);
		sprintf(range_text, "Range: %.0f", TOWER_SNIPER_RANGE);
		sprintf(rate_text, "Rate: %.1f/s", TOWER_SNIPER_FIRE_RATE);
	}
	else if (tower_type == TOWER_CANNON)
	{
		sprintf(damage_text, "Damage: %d", TOWER_CANNON_DAMAGE);
		sprintf(range_text, "Range: %.0f", TOWER_CANNON_RANGE);
		sprintf(rate_text, "Rate: %.1f/s", TOWER_CANNON_FIRE_RATE);
	}
	else if (tower_type == TOWER_FREEZE)
	{
		sprintf(damage_text, "Damage: %d", TOWER_FREEZE_DAMAGE);
		sprintf(range_text, "Range: %.0f", TOWER_FREEZE_RANGE);
		sprintf(rate_text, "Slows enemies");
	}
	
	/* Render tooltip text */
	text_pos = vec2_create(pos.x + 5, pos.y + 5);
	if (game->font_small)
	{
		render_text(game, damage_text, text_pos, text_color, game->font_small);
		text_pos.y += 15;
		render_text(game, range_text, text_pos, text_color, game->font_small);
		text_pos.y += 15;
		render_text(game, rate_text, text_pos, text_color, game->font_small);
	}
}

static void	ui_render_resources(t_game *game)
{
	char	money_text[64];
	char	lives_text[64];
	char	score_text[64];
	t_vector2	text_pos;
	t_color	money_color;
	t_color	lives_color;
	t_color	score_color;

	sprintf(money_text, "Money: $%d", game->money);
	sprintf(lives_text, "Lives: %d", game->lives);
	sprintf(score_text, "Score: %d", game->score);
	
	/* Color coding for resources */
	money_color = (game->money >= 50) ? color_create(0, 255, 0, 255) : 
		color_create(255, 255, 0, 255);
	lives_color = (game->lives > 10) ? color_create(0, 255, 0, 255) : 
		(game->lives > 5) ? color_create(255, 255, 0, 255) : 
		color_create(255, 0, 0, 255);
	score_color = color_white();
	
	/* Render resource texts */
	text_pos = vec2_create(WINDOW_WIDTH - 250, 10);
	if (game->font_medium)
	{
		render_text(game, money_text, text_pos, money_color, game->font_medium);
		text_pos.y += 25;
		render_text(game, lives_text, text_pos, lives_color, game->font_medium);
		text_pos.y += 25;
		render_text(game, score_text, text_pos, score_color, game->font_medium);
	}
}

static void	ui_render_game_stats(t_game *game)
{
	char	enemies_text[64];
	char	towers_text[64];
	t_vector2	text_pos;
	t_color	text_color;

	sprintf(enemies_text, "Enemies: %d", entities_count_active_enemies(game));
	sprintf(towers_text, "Towers: %d", entities_count_active_towers(game));
	
	text_color = color_create(200, 200, 200, 255);
	text_pos = vec2_create(10, 10);
	
	if (game->font_small)
	{
		render_text(game, enemies_text, text_pos, text_color, game->font_small);
		text_pos.y += 20;
		render_text(game, towers_text, text_pos, text_color, game->font_small);
	}
}

static void	ui_render_wave_progress(t_game *game)
{
	char	wave_text[64];
	char	progress_text[64];
	t_vector2	text_pos;
	t_color	wave_color;
	t_rect	progress_bar_bg;
	t_rect	progress_bar_fill;
	float	progress_ratio;

	sprintf(wave_text, "Wave %d", game->current_wave.number);
	
	if (game->current_wave.preparing)
	{
		sprintf(progress_text, "Preparing... %.1fs", 
			game->current_wave.prep_timer);
		wave_color = color_create(255, 255, 0, 255);
		progress_ratio = 1.0f - (game->current_wave.prep_timer / WAVE_PREP_TIME);
	}
	else if (game->current_wave.active)
	{
		sprintf(progress_text, "%d/%d enemies", 
			game->current_wave.enemies_spawned, 
			game->current_wave.enemies_total);
		wave_color = color_create(255, 100, 100, 255);
		progress_ratio = (float)game->current_wave.enemies_spawned / 
			(float)game->current_wave.enemies_total;
	}
	else
	{
		sprintf(progress_text, "Wave Complete!");
		wave_color = color_create(0, 255, 0, 255);
		progress_ratio = 1.0f;
	}
	
	/* Render wave text */
	text_pos = vec2_create(WINDOW_WIDTH / 2 - 50, 10);
	if (game->font_medium)
	{
		render_text(game, wave_text, text_pos, wave_color, game->font_medium);
		text_pos.y += 25;
		render_text(game, progress_text, text_pos, color_white(), 
			game->font_small);
	}
	
	/* Render progress bar */
	progress_bar_bg = rect_create(WINDOW_WIDTH / 2 - 75, 50, 150, 8);
	progress_bar_fill = rect_create(WINDOW_WIDTH / 2 - 75, 50, 
		(int)(150 * progress_ratio), 8);
	
	render_rect(game, progress_bar_bg, color_create(50, 50, 50, 255));
	render_rect(game, progress_bar_fill, wave_color);
}

static void	ui_render_selected_tower_info(t_game *game)
{
	t_tower	*tower;
	t_rect	info_panel;
	char	level_text[32];
	char	damage_text[32];
	char	kills_text[32];
	char	upgrade_text[64];
	t_vector2	text_pos;
	t_color	bg_color;
	t_color	text_color;

	if (game->selected_tower_id < 0 || game->selected_tower_id >= MAX_TOWERS)
		return ;
	tower = &game->towers[game->selected_tower_id];
	if (!tower->base.active)
		return ;
	
	/* Info panel setup */
	info_panel = rect_create(WINDOW_WIDTH - 200, 100, 190, 120);
	bg_color = color_create(30, 30, 30, 200);
	text_color = color_white();
	
	/* Render info panel background */
	render_rect(game, info_panel, bg_color);
	render_rect(game, rect_create(info_panel.x - 2, info_panel.y - 2,
		info_panel.w + 4, info_panel.h + 4), color_create(100, 100, 100, 255));
	render_rect(game, info_panel, bg_color);
	
	/* Prepare info text */
	sprintf(level_text, "Level %d", tower->level);
	sprintf(damage_text, "Damage: %d", tower->damage);
	sprintf(kills_text, "Kills: %d", tower->kills);
	if (tower->level < TOWER_MAX_LEVEL)
		sprintf(upgrade_text, "Upgrade: $%d", tower->upgrade_cost);
	else
		sprintf(upgrade_text, "Max Level");
	
	/* Render info text */
	text_pos = vec2_create(info_panel.x + 10, info_panel.y + 10);
	if (game->font_small)
	{
		render_text(game, level_text, text_pos, color_create(255, 215, 0, 255), 
			game->font_small);
		text_pos.y += 20;
		render_text(game, damage_text, text_pos, text_color, game->font_small);
		text_pos.y += 20;
		sprintf(damage_text, "Range: %.0f", tower->range);
		render_text(game, damage_text, text_pos, text_color, game->font_small);
		text_pos.y += 20;
		render_text(game, kills_text, text_pos, text_color, game->font_small);
		text_pos.y += 20;
		t_color upgrade_color = (tower->level < TOWER_MAX_LEVEL && 
			game->money >= tower->upgrade_cost) ? 
			color_create(0, 255, 0, 255) : color_create(150, 150, 150, 255);
		render_text(game, upgrade_text, text_pos, upgrade_color, game->font_small);
	}
}

void	ui_render_tower_info(t_game *game)
{
	ui_render_selected_tower_info(game);
}

void	ui_render_wave_info(t_game *game)
{
	ui_render_wave_progress(game);
}

void	ui_handle_click(t_game *game, t_vector2 pos)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < 4)
	{
		if (ui_button_clicked(&game->tower_buttons[i], pos))
		{
			if (game->money >= (i == 0 ? TOWER_BASIC_COST : 
				i == 1 ? TOWER_SNIPER_COST :
				i == 2 ? TOWER_CANNON_COST : TOWER_FREEZE_COST))
			{
				game->selected_tower_type = i + 1;
				game->placing_tower = true;
				debug_info("Tower type %d selected for placement", i + 1);
			}
			else
			{
				debug_warning("Not enough money for tower type %d", i + 1);
			}
			return ;
		}
		i++;
	}
}

bool	ui_button_clicked(t_button *button, t_vector2 pos)
{
	if (!validate_pointer(button))
		return (false);
	return (rect_contains_point(button->rect, pos));
}
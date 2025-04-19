/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:53:25 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:27:12 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

SDL_Color	get_white_color(void)
{
	SDL_Color	color;

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	return (color);
}

SDL_Color	get_black_color(void)
{
	SDL_Color	color;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	return (color);
}

SDL_Color	get_red_color(void)
{
	SDL_Color	color;

	color.r = 255;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	return (color);
}

SDL_Color	get_green_color(void)
{
	SDL_Color	color;

	color.r = 0;
	color.g = 255;
	color.b = 0;
	color.a = 255;
	return (color);
}

void	render_tower_button_text(t_game *game, int i)
{
	SDL_Rect	cost_rect;
	SDL_Rect	name_rect;
	char		cost_text[20];
	char		*name_text;
	SDL_Color	text_color;

	if (game->money >= game->tower_buttons[i].cost)
		text_color = get_white_color();
	else
		text_color = get_red_color();
	sprintf(cost_text, "%d$", game->tower_buttons[i].cost);
	cost_rect.x = game->tower_buttons[i].rect.x + 5;
	cost_rect.y = game->tower_buttons[i].rect.y + game->tower_buttons[i].rect.h - 15;
	cost_rect.w = 50;
	cost_rect.h = 15;
	render_text(game, cost_text, game->font_small, cost_rect, text_color);
	if (game->tower_buttons[i].tower_type == TOWER_TYPE_BASIC)
		name_text = "Basic";
	else if (game->tower_buttons[i].tower_type == TOWER_TYPE_SLOW)
		name_text = "Slow";
	else
		name_text = "Multi";
	name_rect.x = game->tower_buttons[i].rect.x + 5;
	name_rect.y = game->tower_buttons[i].rect.y + 2;
	name_rect.w = 50;
	name_rect.h = 15;
	render_text(game, name_text, game->font_small, name_rect, text_color);
}

void	render_tower_buttons(t_game *game)
{
	int			i;
	SDL_Rect	tower_rect;

	i = 0;
	while (i < MAX_TOWER_TYPES)
	{
		if (game->placing_tower
			&& game->selected_tower_type == game->tower_buttons[i].tower_type)
			SDL_SetRenderDrawColor(game->renderer, 150, 150, 200, 255);
		else if (game->money >= game->tower_buttons[i].cost)
			SDL_SetRenderDrawColor(game->renderer, 120, 120, 150, 255);
		else
			SDL_SetRenderDrawColor(game->renderer, 90, 90, 90, 255);
		SDL_RenderFillRect(game->renderer, &game->tower_buttons[i].rect);
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
		SDL_RenderDrawRect(game->renderer, &game->tower_buttons[i].rect);
		tower_rect.x = game->tower_buttons[i].rect.x
			+ game->tower_buttons[i].rect.w / 4;
		tower_rect.y = game->tower_buttons[i].rect.y
			+ game->tower_buttons[i].rect.h / 4;
		tower_rect.w = game->tower_buttons[i].rect.w / 2;
		tower_rect.h = game->tower_buttons[i].rect.h / 2;
		if (game->tower_buttons[i].tower_type == TOWER_TYPE_BASIC)
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 255);
		else if (game->tower_buttons[i].tower_type == TOWER_TYPE_SLOW)
			SDL_SetRenderDrawColor(game->renderer, 0, 150, 200, 255);
		else if (game->tower_buttons[i].tower_type == TOWER_TYPE_MULTI)
			SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 255);
		SDL_RenderFillRect(game->renderer, &tower_rect);
		render_tower_button_text(game, i);
		i++;
	}
}

void	render_stats(t_game *game)
{
	SDL_Rect	money_text_rect;
	SDL_Rect	lives_text_rect;
	SDL_Rect	score_text_rect;
	char		money_text[50];
	char		lives_text[50];
	char		score_text[50];
	SDL_Color	text_color;

	text_color = get_white_color();
	sprintf(money_text, "Money: %d$", game->money);
	sprintf(lives_text, "Lives: %d", game->lives);
	sprintf(score_text, "Score: %d", game->score);

	money_text_rect.x = 20;
	money_text_rect.y = GAME_AREA_HEIGHT + 20;
	money_text_rect.w = 100;
	money_text_rect.h = 20;
	render_text(game, money_text, game->font_medium, money_text_rect, text_color);

	lives_text_rect.x = WINDOW_WIDTH - 120;
	lives_text_rect.y = GAME_AREA_HEIGHT + 20;
	lives_text_rect.w = 100;
	lives_text_rect.h = 20;
	render_text(game, lives_text, game->font_medium, lives_text_rect, text_color);

	// Score déplacé sous les vies
	score_text_rect.x = WINDOW_WIDTH - 120;
	score_text_rect.y = GAME_AREA_HEIGHT + 45; // Position en dessous de l'affichage des vies
	score_text_rect.w = 100;
	score_text_rect.h = 20;
	render_text(game, score_text, game->font_medium, score_text_rect, text_color);
}

void	render_toolbar(t_game *game)
{
	SDL_Rect	toolbar_rect;

	toolbar_rect.x = 0;
	toolbar_rect.y = GAME_AREA_HEIGHT;
	toolbar_rect.w = WINDOW_WIDTH;
	toolbar_rect.h = TOOLBAR_HEIGHT;
	SDL_SetRenderDrawColor(game->renderer, 80, 80, 100, 255);
	SDL_RenderFillRect(game->renderer, &toolbar_rect);
	render_tower_buttons(game);
	render_stats(game);
}

void	render_grid(t_game *game)
{
	int			row;
	int			col;
	SDL_Rect	rect;

	row = 0;
	while (row < GRID_ROWS)
	{
		col = 0;
		while (col < GRID_COLS)
		{
			rect.x = col * GRID_SIZE;
			rect.y = row * GRID_SIZE;
			rect.w = GRID_SIZE;
			rect.h = GRID_SIZE;
			if (game->grid[row][col] == TERRAIN_PATH)
				SDL_SetRenderDrawColor(game->renderer, 200, 200, 100, 255);
			else if (game->grid[row][col] == TERRAIN_OCCUPIED)
				SDL_SetRenderDrawColor(game->renderer, 130, 130, 130, 255);
			else
				SDL_SetRenderDrawColor(game->renderer, 100, 150, 100, 255);
			SDL_RenderFillRect(game->renderer, &rect);
			SDL_SetRenderDrawColor(game->renderer, 50, 50, 50, 255);
			SDL_RenderDrawRect(game->renderer, &rect);
			col++;
		}
		row++;
	}
}

void	render_towers(t_game *game)
{
	int			i;
	SDL_Rect	rect;
	SDL_Rect	range_rect;
	int			radius;

	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].active)
		{
			rect.x = (int)game->towers[i].x - game->towers[i].width / 2;
			rect.y = (int)game->towers[i].y - game->towers[i].height / 2;
			rect.w = game->towers[i].width;
			rect.h = game->towers[i].height;
			if (game->towers[i].damage == TOWER_BASIC_DAMAGE)
				SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 255);
			else if (game->towers[i].damage == TOWER_SLOW_DAMAGE)
				SDL_SetRenderDrawColor(game->renderer, 0, 150, 200, 255);
			else if (game->towers[i].damage == TOWER_MULTI_DAMAGE)
				SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 255);
			SDL_RenderFillRect(game->renderer, &rect);
			SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 100);
			radius = (int)game->towers[i].range;
			range_rect.x = (int)game->towers[i].x - radius;
			range_rect.y = (int)game->towers[i].y - radius;
			range_rect.w = radius * 2;
			range_rect.h = radius * 2;
			SDL_RenderDrawRect(game->renderer, &range_rect);
		}
		i++;
	}
}

void	render_enemies(t_game *game)
{
	int			i;
	SDL_Rect	rect;
	SDL_Rect	health_rect;
	int			health_width;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].active)
		{
			rect.x = (int)game->enemies[i].x - game->enemies[i].width / 2;
			rect.y = (int)game->enemies[i].y - game->enemies[i].height / 2;
			rect.w = game->enemies[i].width;
			rect.h = game->enemies[i].height;
			if (game->enemies[i].slowed)
				SDL_SetRenderDrawColor(game->renderer, 150, 100, 100, 255);
			else
				SDL_SetRenderDrawColor(game->renderer, 200, 0, 0, 255);
			SDL_RenderFillRect(game->renderer, &rect);
			health_width = (int)((float)game->enemies[i].health
					/ game->enemies[i].max_health * game->enemies[i].width);
			health_rect.x = (int)game->enemies[i].x - game->enemies[i].width / 2;
			health_rect.y = (int)game->enemies[i].y - game->enemies[i].height / 2 - 5;
			health_rect.w = health_width;
			health_rect.h = 3;
			SDL_SetRenderDrawColor(game->renderer, 0, 200, 0, 255);
			SDL_RenderFillRect(game->renderer, &health_rect);
		}
		i++;
	}
}

void	render_projectiles(t_game *game)
{
	int			i;
	SDL_Rect	rect;

	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (game->projectiles[i].active)
		{
			rect.x = (int)game->projectiles[i].x - 2;
			rect.y = (int)game->projectiles[i].y - 2;
			rect.w = 4;
			rect.h = 4;
			if (game->projectiles[i].damage == TOWER_BASIC_DAMAGE)
				SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
			else if (game->projectiles[i].damage == TOWER_SLOW_DAMAGE)
				SDL_SetRenderDrawColor(game->renderer, 0, 255, 255, 255);
			else if (game->projectiles[i].damage == TOWER_MULTI_DAMAGE)
				SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 255);
			else
				SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(game->renderer, &rect);
		}
		i++;
	}
}

void	render_tower_preview(t_game *game)
{
	int			mouse_x;
	int			mouse_y;
	int			grid_x;
	int			grid_y;
	bool		valid_placement;
	int			center_x;
	int			center_y;
	SDL_Rect	preview_rect;
	float		range;
	SDL_Rect	range_rect;

	if (!game->placing_tower)
		return ;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (mouse_y >= GAME_AREA_HEIGHT)
		return ;
	grid_x = mouse_x / GRID_SIZE;
	grid_y = mouse_y / GRID_SIZE;
	valid_placement = (grid_x >= 0 && grid_x < GRID_COLS
			&& grid_y >= 0 && grid_y < GRID_ROWS
			&& game->grid[grid_y][grid_x] == TERRAIN_BUILDABLE);
	center_x = grid_x * GRID_SIZE + GRID_SIZE / 2;
	center_y = grid_y * GRID_SIZE + GRID_SIZE / 2;
	preview_rect.x = center_x - 15;
	preview_rect.y = center_y - 15;
	preview_rect.w = 30;
	preview_rect.h = 30;
	if (valid_placement)
	{
		if (game->selected_tower_type == TOWER_TYPE_BASIC)
			SDL_SetRenderDrawColor(game->renderer, 0, 0, 200, 180);
		else if (game->selected_tower_type == TOWER_TYPE_SLOW)
			SDL_SetRenderDrawColor(game->renderer, 0, 150, 200, 180);
		else if (game->selected_tower_type == TOWER_TYPE_MULTI)
			SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 180);
	}
	else
		SDL_SetRenderDrawColor(game->renderer, 200, 0, 0, 180);
	SDL_RenderFillRect(game->renderer, &preview_rect);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 100);
	range = 0.0f;
	if (game->selected_tower_type == TOWER_TYPE_BASIC)
		range = TOWER_BASIC_RANGE;
	else if (game->selected_tower_type == TOWER_TYPE_SLOW)
		range = TOWER_SLOW_RANGE;
	else if (game->selected_tower_type == TOWER_TYPE_MULTI)
		range = TOWER_MULTI_RANGE;
	else
		range = 100.0f;
	range_rect.x = center_x - (int)range;
	range_rect.y = center_y - (int)range;
	range_rect.w = (int)range * 2;
	range_rect.h = (int)range * 2;
	SDL_RenderDrawRect(game->renderer, &range_rect);
}

void	render_game_area(t_game *game)
{
	render_grid(game);
	render_towers(game);
	render_enemies(game);
	render_projectiles(game);
	render_tower_preview(game);
}

void	render_game_over_screen(t_game *game)
{
	SDL_Rect	overlay;
	SDL_Rect	title_rect;
	SDL_Rect	score_rect;
	SDL_Rect	restart_text_rect;
	SDL_Color	white_color;
//	SDL_Color	black_color;
	char		score_text[50];

	white_color = get_white_color();
//	black_color = get_black_color();
	
	// Overlay semi-transparent
	overlay.x = 0;
	overlay.y = 0;
	overlay.w = WINDOW_WIDTH;
	overlay.h = WINDOW_HEIGHT;
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(game->renderer, &overlay);

	// Titre "Game Over"
	title_rect.x = (WINDOW_WIDTH - 300) / 2;
	title_rect.y = WINDOW_HEIGHT / 4;
	title_rect.w = 300;
	title_rect.h = 60;
	SDL_SetRenderDrawColor(game->renderer, 150, 0, 0, 255);
	SDL_RenderFillRect(game->renderer, &title_rect);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &title_rect);
	render_text(game, "GAME OVER", game->font_large, title_rect, white_color);

	// Score final
	sprintf(score_text, "Score final: %d", game->score);
	score_rect.x = (WINDOW_WIDTH - 200) / 2;
	score_rect.y = WINDOW_HEIGHT / 4 + 80;
	score_rect.w = 200;
	score_rect.h = 40;
	SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(game->renderer, &score_rect);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &score_rect);
	render_text(game, score_text, game->font_medium, score_rect, white_color);

	// Bouton Restart
	SDL_SetRenderDrawColor(game->renderer, 50, 100, 50, 255);
	SDL_RenderFillRect(game->renderer, &game->restart_button);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &game->restart_button);
	
	// Texte du bouton
	restart_text_rect = game->restart_button;
	render_text(game, "RESTART", game->font_medium, restart_text_rect, white_color);
}

void	render(t_game *game)
{
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	if (game->state == STATE_PLAYING)
	{
		render_game_area(game);
		render_toolbar(game);
	}
	else if (game->state == STATE_GAME_OVER)
	{
		render_game_area(game);
		render_toolbar(game);
		render_game_over_screen(game);
	}

	SDL_RenderPresent(game->renderer);
}
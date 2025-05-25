/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:33:39 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	render_clear(t_game *game)
{
	t_color	bg_color;

	if (!validate_pointer(game))
		return ;
	bg_color = color_create(32, 48, 32, 255);
	SDL_SetRenderDrawColor(game->renderer, bg_color.r, bg_color.g, 
		bg_color.b, bg_color.a);
	SDL_RenderClear(game->renderer);
}

void	render_present(t_game *game)
{
	if (!validate_pointer(game))
		return ;
	SDL_RenderPresent(game->renderer);
}

void	render_grid(t_game *game)
{
	int		x;
	int		y;
	t_color	path_color;
	t_rect	cell_rect;

	if (!validate_pointer(game) || !game->show_debug)
		return ;
	path_color = color_create(139, 69, 19, 255);
	y = 0;
	while (y < GRID_ROWS)
	{
		x = 0;
		while (x < GRID_COLS)
		{
			cell_rect = rect_create(x * GRID_SIZE, y * GRID_SIZE, 
				GRID_SIZE, GRID_SIZE);
			if (game->grid[y][x].type == TERRAIN_PATH)
				render_rect(game, cell_rect, path_color);
			else if (game->grid[y][x].occupied)
				render_rect(game, cell_rect, color_create(128, 128, 128, 128));
			x++;
		}
		y++;
	}
}

void	render_path(t_game *game)
{
	int			i;
	t_vector2	start;
	t_vector2	end;
	t_color		path_color;

	if (!validate_pointer(game) || game->path_length < 2)
		return ;
	path_color = color_create(255, 255, 0, 128);
	i = 0;
	while (i < game->path_length - 1)
	{
		start = game->path[i].pos;
		end = game->path[i + 1].pos;
		render_line(game, start, end, path_color);
		i++;
	}
}

void	render_ranges(t_game *game)
{
	int		i;
	t_color	range_color;

	if (!validate_pointer(game))
		return ;
	range_color = color_create(255, 255, 255, 64);
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
		{
			render_circle(game, game->towers[i].base.pos, 
				game->towers[i].range, range_color);
		}
		i++;
	}
}

void	render_circle(t_game *game, t_vector2 center, float radius, t_color color)
{
	int		segments;
	int		i;
	float	angle;
	float	next_angle;
	t_vector2	point1;
	t_vector2	point2;

	if (!validate_pointer(game) || radius <= 0)
		return ;
	segments = (int)(radius * 0.5f);
	if (segments < 8)
		segments = 8;
	if (segments > 64)
		segments = 64;
	SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
	i = 0;
	while (i < segments)
	{
		angle = (2.0f * PI * i) / segments;
		next_angle = (2.0f * PI * (i + 1)) / segments;
		point1.x = center.x + cosf(angle) * radius;
		point1.y = center.y + sinf(angle) * radius;
		point2.x = center.x + cosf(next_angle) * radius;
		point2.y = center.y + sinf(next_angle) * radius;
		SDL_RenderDrawLineF(game->renderer, point1.x, point1.y, 
			point2.x, point2.y);
		i++;
	}
}

void	render_rect(t_game *game, t_rect rect, t_color color)
{
	SDL_Rect	sdl_rect;

	if (!validate_pointer(game))
		return ;
	sdl_rect.x = rect.x;
	sdl_rect.y = rect.y;
	sdl_rect.w = rect.w;
	sdl_rect.h = rect.h;
	SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(game->renderer, &sdl_rect);
}

void	render_line(t_game *game, t_vector2 start, t_vector2 end, t_color color)
{
	if (!validate_pointer(game))
		return ;
	SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLineF(game->renderer, start.x, start.y, end.x, end.y);
}

void	render_text(t_game *game, const char *text, t_vector2 pos, t_color color,
		TTF_Font *font)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	SDL_Color	sdl_color;
	SDL_Rect	dest_rect;
	int			text_w;
	int			text_h;

	if (!validate_pointer(game) || !validate_string(text) || !font)
		return ;
	sdl_color = color_to_sdl(color);
	surface = TTF_RenderText_Blended(font, text, sdl_color);
	if (!surface)
		return ;
	texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (!texture)
	{
		SDL_FreeSurface(surface);
		return ;
	}
	TTF_SizeText(font, text, &text_w, &text_h);
	dest_rect.x = (int)pos.x;
	dest_rect.y = (int)pos.y;
	dest_rect.w = text_w;
	dest_rect.h = text_h;
	SDL_RenderCopy(game->renderer, texture, NULL, &dest_rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void	render_debug_info(t_game *game)
{
	char		fps_text[64];
	char		entities_text[128];
	char		game_info[128];
	t_vector2	text_pos;
	t_color		debug_color;

	if (!validate_pointer(game))
		return ;
	debug_color = color_white();
	sprintf(fps_text, "FPS: %.1f", game->fps);
	sprintf(entities_text, "Towers: %d | Enemies: %d | Projectiles: %d",
		game->active_towers, game->active_enemies, game->active_projectiles);
	sprintf(game_info, "Money: %d | Lives: %d | Score: %d | Wave: %d",
		game->money, game->lives, game->score, game->current_wave.number);
	text_pos = vec2_create(10, 10);
	render_text(game, fps_text, text_pos, debug_color, game->font_small);
	text_pos.y += 20;
	render_text(game, entities_text, text_pos, debug_color, game->font_small);
	text_pos.y += 20;
	render_text(game, game_info, text_pos, debug_color, game->font_small);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:12:25 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:21:47 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "../includes/game.h"

float	distance(float x1, float y1, float x2, float y2)
{
	float	dx;
	float	dy;

	dx = x2 - x1;
	dy = y2 - y1;
	return (sqrt(dx * dx + dy * dy));
}

void	render_text(t_game *game, char *text, TTF_Font *font,
	SDL_Rect dest, SDL_Color color)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;

	if (!font)
		return ;
	surface = TTF_RenderText_Blended(font, text, color);
	if (!surface)
	{
		printf("Erreur de rendu du texte: %s\n", TTF_GetError());
		return ;
	}
	texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (!texture)
	{
		printf("Erreur de création de texture: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
		return ;
	}
	SDL_RenderCopy(game->renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}
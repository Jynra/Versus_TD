/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:42 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:18:41 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>

typedef struct s_game t_game;

/* Calculate distance between two points */
float	distance(float x1, float y1, float x2, float y2);

/* Render text using SDL_ttf */
void	render_text(t_game *game, char *text, TTF_Font *font,
			SDL_Rect dest, SDL_Color color);

#endif
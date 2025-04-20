/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:01:22 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:35:25 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include <stdbool.h>

typedef struct s_game t_game;

/* UI related functions */
void	ui_init(t_game *game);
void	ui_render_toolbar(t_game *game);
void	ui_render_stats(t_game *game);
void	ui_render_game_over(t_game *game);
void	ui_render_pause_menu(t_game *game);
void	ui_render_round_info(t_game *game);

/* Text rendering helper functions */
void	ui_render_text(t_game *game, char *text, TTF_Font *font,
			SDL_Rect dest, SDL_Color color);
void	ui_render_text_centered(t_game *game, char *text, TTF_Font *font,
			SDL_Rect dest, SDL_Color color);

/* UI interaction helpers */
bool	ui_point_in_rect(int x, int y, SDL_Rect rect);
bool	ui_button_clicked(SDL_Rect button, int mouse_x, int mouse_y);

/* Color helpers */
SDL_Color	ui_color_white(void);
SDL_Color	ui_color_black(void);
SDL_Color	ui_color_red(void);
SDL_Color	ui_color_green(void);
SDL_Color	ui_color_blue(void);
SDL_Color	ui_color_yellow(void);
SDL_Color	ui_color_rgba(int r, int g, int b, int a);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_loader.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:09:45 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:35:00 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASSET_LOADER_H
# define ASSET_LOADER_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <stdbool.h>

typedef struct s_game t_game;

/* Texture index constants */
enum e_texture_index {
	TEXTURE_TOWER_BASIC,
	TEXTURE_TOWER_SLOW,
	TEXTURE_TOWER_MULTI,
	TEXTURE_ENEMY_BASIC,
	TEXTURE_ENEMY_FAST,
	TEXTURE_ENEMY_BOSS,
	TEXTURE_PROJECTILE_BASIC,
	TEXTURE_PROJECTILE_SLOW,
	TEXTURE_PROJECTILE_MULTI,
	TEXTURE_BACKGROUND,
	TEXTURE_UI_BUTTON,
	TEXTURE_COUNT_ENUM  /* Renommé pour éviter le conflit */
};

/* Font index constants */
enum e_font_index {
	FONT_SMALL,
	FONT_MEDIUM,
	FONT_LARGE,
	FONT_COUNT_ENUM  /* Renommé pour éviter le conflit */
};

/* Fonctions de chargement des ressources */
bool	assets_load_all(t_game *game);
void	assets_free_all(t_game *game);

/* Fonctions de chargement spécifiques */
bool	assets_load_textures(t_game *game);
bool	assets_load_fonts(t_game *game);
bool	assets_load_audio(t_game *game);

/* Fonctions utilitaires */
SDL_Texture	*texture_load(SDL_Renderer *renderer, const char *path);
SDL_Texture	*texture_create_color(SDL_Renderer *renderer, int width, int height, 
			SDL_Color color);
void	texture_free(SDL_Texture *texture);
TTF_Font	*font_load(const char *path, int size);
void	font_free(TTF_Font *font);

#endif
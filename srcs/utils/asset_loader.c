/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asset_loader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:40:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:06:13 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/asset_loader.h"
#include "../../includes/core/game.h"
#include <stdio.h>

/**
 * @brief Charge toutes les ressources nécessaires au jeu
 * 
 * @param game Pointeur vers la structure du jeu
 * @return true si toutes les ressources ont été chargées avec succès, false sinon
 */
bool	assets_load_all(t_game *game)
{
	if (!game)
		return (false);
		
	/* Initialisation des pointeurs de textures */
	for (int i = 0; i < TEXTURE_COUNT; i++)
		game->textures[i] = NULL;
		
	/* Initialisation des pointeurs de polices */
	for (int i = 0; i < FONT_COUNT; i++)
		game->fonts[i] = NULL;
		
	/* Chargement des textures */
	if (!assets_load_textures(game))
	{
		fprintf(stderr, "ERROR: Failed to load textures\n");
		return (false);
	}
	
	/* Chargement des polices */
	if (!assets_load_fonts(game))
	{
		fprintf(stderr, "ERROR: Failed to load fonts\n");
		return (false);
	}
	
	return (true);
}

/**
 * @brief Libère toutes les ressources chargées
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	assets_free_all(t_game *game)
{
	int	i;
	
	if (!game)
		return;
		
	/* Libération des textures */
	for (i = 0; i < TEXTURE_COUNT; i++)
	{
		if (game->textures[i])
		{
			SDL_DestroyTexture(game->textures[i]);
			game->textures[i] = NULL;
		}
	}
	
	/* Libération des polices */
	for (i = 0; i < FONT_COUNT; i++)
	{
		if (game->fonts[i])
		{
			TTF_CloseFont(game->fonts[i]);
			game->fonts[i] = NULL;
		}
	}
}

/**
 * @brief Charge toutes les textures nécessaires au jeu
 * 
 * @param game Pointeur vers la structure du jeu
 * @return true si toutes les textures ont été chargées avec succès, false sinon
 */
bool	assets_load_textures(t_game *game)
{
	/* Dans le cas où aucun fichier de texture n'est disponible,
	   on va créer des textures de couleur unie */
	
	/* Tour de base - bleu */
	game->textures[TEXTURE_TOWER_BASIC] = texture_create_color(
		game->renderer, 30, 30, ui_color_rgba(0, 0, 200, 255));
		
	/* Tour ralentissante - cyan */
	game->textures[TEXTURE_TOWER_SLOW] = texture_create_color(
		game->renderer, 30, 30, ui_color_rgba(0, 150, 200, 255));
		
	/* Tour multi-cibles - violet */
	game->textures[TEXTURE_TOWER_MULTI] = texture_create_color(
		game->renderer, 30, 30, ui_color_rgba(200, 0, 200, 255));
		
	/* Ennemi de base - rouge */
	game->textures[TEXTURE_ENEMY_BASIC] = texture_create_color(
		game->renderer, 30, 30, ui_color_rgba(200, 0, 0, 255));
		
	/* Ennemi rapide - orange */
	game->textures[TEXTURE_ENEMY_FAST] = texture_create_color(
		game->renderer, 30, 30, ui_color_rgba(255, 165, 0, 255));
		
	/* Ennemi boss - rouge foncé */
	game->textures[TEXTURE_ENEMY_BOSS] = texture_create_color(
		game->renderer, 40, 40, ui_color_rgba(150, 0, 0, 255));
		
	/* Projectile de base - jaune */
	game->textures[TEXTURE_PROJECTILE_BASIC] = texture_create_color(
		game->renderer, PROJECTILE_BASIC_SIZE, PROJECTILE_BASIC_SIZE, 
		ui_color_rgba(255, 255, 0, 255));
		
	/* Projectile ralentissant - cyan */
	game->textures[TEXTURE_PROJECTILE_SLOW] = texture_create_color(
		game->renderer, PROJECTILE_SLOW_SIZE, PROJECTILE_SLOW_SIZE, 
		ui_color_rgba(0, 255, 255, 255));
		
	/* Projectile multi-cibles - magenta */
	game->textures[TEXTURE_PROJECTILE_MULTI] = texture_create_color(
		game->renderer, PROJECTILE_MULTI_SIZE, PROJECTILE_MULTI_SIZE, 
		ui_color_rgba(255, 0, 255, 255));
		
	/* Fond du jeu - vert foncé */
	game->textures[TEXTURE_BACKGROUND] = texture_create_color(
		game->renderer, WINDOW_WIDTH, GAME_AREA_HEIGHT, 
		ui_color_rgba(50, 80, 50, 255));
		
	/* Bouton UI - gris */
	game->textures[TEXTURE_UI_BUTTON] = texture_create_color(
		game->renderer, TOWER_BUTTON_SIZE, TOWER_BUTTON_SIZE, 
		ui_color_rgba(120, 120, 150, 255));
	
	/* Vérification que toutes les textures ont été créées */
	for (int i = 0; i < TEXTURE_COUNT; i++)
	{
		if (!game->textures[i])
		{
			fprintf(stderr, "ERROR: Failed to create texture %d\n", i);
			return (false);
		}
	}
	
	return (true);
}

/**
 * @brief Charge toutes les polices nécessaires au jeu
 * 
 * @param game Pointeur vers la structure du jeu
 * @return true si toutes les polices ont été chargées avec succès, false sinon
 */
bool	assets_load_fonts(t_game *game)
{
	/* Chargement des différentes tailles de police */
	game->fonts[FONT_SMALL] = font_load(FONT_PATH, 12);
	game->fonts[FONT_MEDIUM] = font_load(FONT_PATH, 18);
	game->fonts[FONT_LARGE] = font_load(FONT_PATH, 24);
	
	/* Vérification que toutes les polices ont été chargées */
	for (int i = 0; i < FONT_COUNT; i++)
	{
		if (!game->fonts[i])
		{
			fprintf(stderr, "ERROR: Failed to load font %d\n", i);
			return (false);
		}
	}
	
	return (true);
}

/**
 * @brief Charge une texture à partir d'un fichier
 * 
 * @param renderer Pointeur vers le renderer SDL
 * @param path Chemin vers le fichier de texture
 * @return SDL_Texture* Pointeur vers la texture chargée, NULL en cas d'erreur
 */
SDL_Texture	*texture_load(SDL_Renderer *renderer, const char *path)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	
	if (!renderer || !path)
		return (NULL);
		
	/* Chargement de la surface depuis le fichier */
	surface = IMG_Load(path);
	if (!surface)
	{
		fprintf(stderr, "ERROR: Failed to load image %s: %s\n", 
			path, IMG_GetError());
		return (NULL);
	}
	
	/* Conversion de la surface en texture */
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		fprintf(stderr, "ERROR: Failed to create texture from %s: %s\n", 
			path, SDL_GetError());
		SDL_FreeSurface(surface);
		return (NULL);
	}
	
	/* Libération de la surface, plus besoin */
	SDL_FreeSurface(surface);
	
	return (texture);
}

/**
 * @brief Crée une texture de couleur unie
 * 
 * @param renderer Pointeur vers le renderer SDL
 * @param width Largeur de la texture
 * @param height Hauteur de la texture
 * @param color Couleur de la texture
 * @return SDL_Texture* Pointeur vers la texture créée, NULL en cas d'erreur
 */
SDL_Texture	*texture_create_color(SDL_Renderer *renderer, int width, int height, 
				SDL_Color color)
{
	SDL_Texture	*texture;
	Uint32		format;
	
	if (!renderer || width <= 0 || height <= 0)
		return (NULL);
		
	/* Création d'une texture vierge avec accès au pixel */
	format = SDL_PIXELFORMAT_RGBA8888;
	texture = SDL_CreateTexture(renderer, format, 
		SDL_TEXTUREACCESS_TARGET, width, height);
		
	if (!texture)
	{
		fprintf(stderr, "ERROR: Failed to create texture: %s\n", SDL_GetError());
		return (NULL);
	}
	
	/* Rendre la texture modifiable */
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	/* Enregistrer le renderer actuel et définir la texture comme cible */
	SDL_Texture *previous_target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, texture);
	
	/* Remplir la texture avec la couleur spécifiée */
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(renderer);
	
	/* Restaurer la cible précédente */
	SDL_SetRenderTarget(renderer, previous_target);
	
	return (texture);
}

/**
 * @brief Libère une texture
 * 
 * @param texture Pointeur vers la texture à libérer
 */
void	texture_free(SDL_Texture *texture)
{
	if (texture)
		SDL_DestroyTexture(texture);
}

/**
 * @brief Charge une police depuis un fichier
 * 
 * @param path Chemin vers le fichier de police
 * @param size Taille de la police
 * @return TTF_Font* Pointeur vers la police chargée, NULL en cas d'erreur
 */
TTF_Font	*font_load(const char *path, int size)
{
	TTF_Font	*font;
	
	if (!path || size <= 0)
		return (NULL);
		
	font = TTF_OpenFont(path, size);
	if (!font)
	{
		fprintf(stderr, "ERROR: Failed to load font %s: %s\n", 
			path, TTF_GetError());
		return (NULL);
	}
	
	return (font);
}

/**
 * @brief Libère une police
 * 
 * @param font Pointeur vers la police à libérer
 */
void	font_free(TTF_Font *font)
{
	if (font)
		TTF_CloseFont(font);
}
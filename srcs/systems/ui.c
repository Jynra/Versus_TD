/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:35:42 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:59:26 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

/**
 * @brief Initialise l'interface utilisateur
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	ui_init(t_game *game)
{
	int	button_width;
	int	button_height;

	/* Initialisation des boutons de tours */
	tower_buttons_init(game);
	
	/* Initialisation du bouton de redémarrage pour l'écran de Game Over */
	button_width = 200;
	button_height = 60;
	game->restart_button.x = (WINDOW_WIDTH - button_width) / 2;
	game->restart_button.y = (WINDOW_HEIGHT - button_height) / 2 + 100;
	game->restart_button.w = button_width;
	game->restart_button.h = button_height;
	
	/* Initialisation du bouton de pause */
	button_width = 40;
	button_height = 40;
	game->pause_button.x = WINDOW_WIDTH - button_width - 10;
	game->pause_button.y = GAME_AREA_HEIGHT + (TOOLBAR_HEIGHT - button_height) / 2;
	game->pause_button.w = button_width;
	game->pause_button.h = button_height;
}

/**
 * @brief Affiche la barre d'outils en bas de l'écran
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	ui_render_toolbar(t_game *game)
{
	SDL_Rect	toolbar_rect;

	/* Fond de la barre d'outils */
	toolbar_rect.x = 0;
	toolbar_rect.y = GAME_AREA_HEIGHT;
	toolbar_rect.w = WINDOW_WIDTH;
	toolbar_rect.h = TOOLBAR_HEIGHT;
	SDL_SetRenderDrawColor(game->renderer, 80, 80, 100, 255);
	SDL_RenderFillRect(game->renderer, &toolbar_rect);
	
	/* Affichage des boutons de tours */
	tower_buttons_render(game);
	
	/* Affichage des statistiques du jeu */
	ui_render_stats(game);
	
	/* Affichage du bouton de pause */
	SDL_SetRenderDrawColor(game->renderer, 120, 120, 150, 255);
	SDL_RenderFillRect(game->renderer, &game->pause_button);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &game->pause_button);
	
	/* Deux barres verticales pour le symbole de pause */
	SDL_Rect pause_symbol1;
	SDL_Rect pause_symbol2;
	
	pause_symbol1.x = game->pause_button.x + game->pause_button.w / 3 - 2;
	pause_symbol1.y = game->pause_button.y + game->pause_button.h / 4;
	pause_symbol1.w = 4;
	pause_symbol1.h = game->pause_button.h / 2;
	
	pause_symbol2.x = game->pause_button.x + 2 * game->pause_button.w / 3 - 2;
	pause_symbol2.y = game->pause_button.y + game->pause_button.h / 4;
	pause_symbol2.w = 4;
	pause_symbol2.h = game->pause_button.h / 2;
	
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderFillRect(game->renderer, &pause_symbol1);
	SDL_RenderFillRect(game->renderer, &pause_symbol2);
}

/**
 * @brief Affiche les statistiques du jeu (argent, score, vies)
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	ui_render_stats(t_game *game)
{
	SDL_Rect	money_text_rect;
	SDL_Rect	lives_text_rect;
	SDL_Rect	score_text_rect;
	char		money_text[50];
	char		lives_text[50];
	char		score_text[50];
	SDL_Color	text_color;

	text_color = ui_color_white();
	
	sprintf(money_text, "Money: %d$", game->money);
	sprintf(lives_text, "Lives: %d", game->lives);
	sprintf(score_text, "Score: %d", game->score);

	money_text_rect.x = 20;
	money_text_rect.y = GAME_AREA_HEIGHT + 20;
	money_text_rect.w = 100;
	money_text_rect.h = 20;
	ui_render_text(game, money_text, game->fonts[FONT_MEDIUM], money_text_rect, text_color);

	lives_text_rect.x = WINDOW_WIDTH - 120;
	lives_text_rect.y = GAME_AREA_HEIGHT + 20;
	lives_text_rect.w = 100;
	lives_text_rect.h = 20;
	ui_render_text(game, lives_text, game->fonts[FONT_MEDIUM], lives_text_rect, text_color);

	score_text_rect.x = WINDOW_WIDTH - 120;
	score_text_rect.y = GAME_AREA_HEIGHT + 45;
	score_text_rect.w = 100;
	score_text_rect.h = 20;
	ui_render_text(game, score_text, game->fonts[FONT_MEDIUM], score_text_rect, text_color);
}

/**
 * @brief Affiche l'écran de Game Over
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	ui_render_game_over(t_game *game)
{
	SDL_Rect	overlay;
	SDL_Rect	title_rect;
	SDL_Rect	score_rect;
	SDL_Rect	restart_text_rect;
	SDL_Color	white_color;
	char		score_text[50];

	white_color = ui_color_white();
	
	/* Overlay semi-transparent */
	overlay.x = 0;
	overlay.y = 0;
	overlay.w = WINDOW_WIDTH;
	overlay.h = WINDOW_HEIGHT;
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(game->renderer, &overlay);

	/* Titre "Game Over" */
	title_rect.x = (WINDOW_WIDTH - 300) / 2;
	title_rect.y = WINDOW_HEIGHT / 4;
	title_rect.w = 300;
	title_rect.h = 60;
	SDL_SetRenderDrawColor(game->renderer, 150, 0, 0, 255);
	SDL_RenderFillRect(game->renderer, &title_rect);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &title_rect);
	ui_render_text_centered(game, "GAME OVER", game->fonts[FONT_LARGE], title_rect, white_color);

	/* Score final */
	sprintf(score_text, "Final Score: %d", game->score);
	score_rect.x = (WINDOW_WIDTH - 200) / 2;
	score_rect.y = WINDOW_HEIGHT / 4 + 80;
	score_rect.w = 200;
	score_rect.h = 40;
	SDL_SetRenderDrawColor(game->renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(game->renderer, &score_rect);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &score_rect);
	ui_render_text_centered(game, score_text, game->fonts[FONT_MEDIUM], score_rect, white_color);

	/* Bouton Restart */
	SDL_SetRenderDrawColor(game->renderer, 50, 100, 50, 255);
	SDL_RenderFillRect(game->renderer, &game->restart_button);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &game->restart_button);
	
	/* Texte du bouton */
	restart_text_rect = game->restart_button;
	ui_render_text_centered(game, "RESTART", game->fonts[FONT_MEDIUM], 
		restart_text_rect, white_color);
}

/**
 * @brief Affiche le menu de pause
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	ui_render_pause_menu(t_game *game)
{
	SDL_Rect	overlay;
	SDL_Rect	title_rect;
	SDL_Rect	resume_button;
	SDL_Rect	quit_button;
	SDL_Color	white_color;

	white_color = ui_color_white();
	
	/* Overlay semi-transparent */
	overlay.x = 0;
	overlay.y = 0;
	overlay.w = WINDOW_WIDTH;
	overlay.h = WINDOW_HEIGHT;
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(game->renderer, &overlay);

	/* Titre "PAUSE" */
	title_rect.x = (WINDOW_WIDTH - 200) / 2;
	title_rect.y = WINDOW_HEIGHT / 4;
	title_rect.w = 200;
	title_rect.h = 60;
	SDL_SetRenderDrawColor(game->renderer, 100, 100, 150, 255);
	SDL_RenderFillRect(game->renderer, &title_rect);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &title_rect);
	ui_render_text_centered(game, "PAUSE", game->fonts[FONT_LARGE], title_rect, white_color);

	/* Bouton Resume */
	resume_button.x = (WINDOW_WIDTH - 200) / 2;
	resume_button.y = WINDOW_HEIGHT / 2;
	resume_button.w = 200;
	resume_button.h = 50;
	SDL_SetRenderDrawColor(game->renderer, 50, 100, 50, 255);
	SDL_RenderFillRect(game->renderer, &resume_button);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &resume_button);
	ui_render_text_centered(game, "RESUME", game->fonts[FONT_MEDIUM], 
		resume_button, white_color);

	/* Bouton Quit */
	quit_button.x = (WINDOW_WIDTH - 200) / 2;
	quit_button.y = WINDOW_HEIGHT / 2 + 70;
	quit_button.w = 200;
	quit_button.h = 50;
	SDL_SetRenderDrawColor(game->renderer, 150, 50, 50, 255);
	SDL_RenderFillRect(game->renderer, &quit_button);
	SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 255);
	SDL_RenderDrawRect(game->renderer, &quit_button);
	ui_render_text_centered(game, "QUIT", game->fonts[FONT_MEDIUM], 
		quit_button, white_color);
}

/**
 * @brief Affiche les informations sur la vague en cours
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	ui_render_round_info(t_game *game)
{
	SDL_Rect	round_text_rect;
	char		round_text[50];
	SDL_Color	text_color;

	text_color = ui_color_white();
	
	sprintf(round_text, "Wave: %d", game->round);

	round_text_rect.x = (WINDOW_WIDTH - 100) / 2;
	round_text_rect.y = GAME_AREA_HEIGHT + 20;
	round_text_rect.w = 100;
	round_text_rect.h = 20;
	
	ui_render_text_centered(game, round_text, game->fonts[FONT_MEDIUM], 
		round_text_rect, text_color);
}

/**
 * @brief Affiche un texte dans un rectangle donné
 * 
 * @param game Pointeur vers la structure du jeu
 * @param text Texte à afficher
 * @param font Police à utiliser
 * @param dest Rectangle de destination
 * @param color Couleur du texte
 */
void	ui_render_text(t_game *game, char *text, TTF_Font *font,
		SDL_Rect dest, SDL_Color color)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;

	if (!font)
		return;
		
	surface = TTF_RenderText_Blended(font, text, color);
	if (!surface)
	{
		fprintf(stderr, "ERROR: Failed to render text: %s\n", TTF_GetError());
		return;
	}
	
	texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (!texture)
	{
		fprintf(stderr, "ERROR: Failed to create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
		return;
	}
	
	SDL_RenderCopy(game->renderer, texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

/**
 * @brief Affiche un texte centré dans un rectangle donné
 * 
 * @param game Pointeur vers la structure du jeu
 * @param text Texte à afficher
 * @param font Police à utiliser
 * @param dest Rectangle de destination
 * @param color Couleur du texte
 */
void	ui_render_text_centered(t_game *game, char *text, TTF_Font *font,
		SDL_Rect dest, SDL_Color color)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	int			text_width;
	int			text_height;
	SDL_Rect	adjusted_dest;

	if (!font)
		return;
		
	surface = TTF_RenderText_Blended(font, text, color);
	if (!surface)
	{
		fprintf(stderr, "ERROR: Failed to render text: %s\n", TTF_GetError());
		return;
	}
	
	texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (!texture)
	{
		fprintf(stderr, "ERROR: Failed to create texture: %s\n", SDL_GetError());
		SDL_FreeSurface(surface);
		return;
	}
	
	text_width = surface->w;
	text_height = surface->h;
	
	/* Centrage du texte dans le rectangle de destination */
	adjusted_dest = dest;
	adjusted_dest.x = dest.x + (dest.w - text_width) / 2;
	adjusted_dest.y = dest.y + (dest.h - text_height) / 2;
	adjusted_dest.w = text_width;
	adjusted_dest.h = text_height;
	
	SDL_RenderCopy(game->renderer, texture, NULL, &adjusted_dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

/**
 * @brief Vérifie si un point est à l'intérieur d'un rectangle
 * 
 * @param x Coordonnée X du point
 * @param y Coordonnée Y du point
 * @param rect Rectangle à vérifier
 * @return true si le point est dans le rectangle, false sinon
 */
bool	ui_point_in_rect(int x, int y, SDL_Rect rect)
{
	return (x >= rect.x && x < rect.x + rect.w
		&& y >= rect.y && y < rect.y + rect.h);
}

/**
 * @brief Vérifie si un bouton a été cliqué
 * 
 * @param button Rectangle du bouton
 * @param mouse_x Coordonnée X de la souris
 * @param mouse_y Coordonnée Y de la souris
 * @return true si le bouton a été cliqué, false sinon
 */
bool	ui_button_clicked(SDL_Rect button, int mouse_x, int mouse_y)
{
	return ui_point_in_rect(mouse_x, mouse_y, button);
}

/**
 * @brief Renvoie la couleur blanche
 * 
 * @return SDL_Color Structure de couleur blanche
 */
SDL_Color	ui_color_white(void)
{
	SDL_Color	color;

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	return (color);
}

/**
 * @brief Renvoie la couleur noire
 * 
 * @return SDL_Color Structure de couleur noire
 */
SDL_Color	ui_color_black(void)
{
	SDL_Color	color;

	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	return (color);
}

/**
 * @brief Renvoie la couleur rouge
 * 
 * @return SDL_Color Structure de couleur rouge
 */
SDL_Color	ui_color_red(void)
{
	SDL_Color	color;

	color.r = 255;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	return (color);
}

/**
 * @brief Renvoie la couleur verte
 * 
 * @return SDL_Color Structure de couleur verte
 */
SDL_Color	ui_color_green(void)
{
	SDL_Color	color;

	color.r = 0;
	color.g = 255;
	color.b = 0;
	color.a = 255;
	return (color);
}

/**
 * @brief Renvoie la couleur bleue
 * 
 * @return SDL_Color Structure de couleur bleue
 */
SDL_Color	ui_color_blue(void)
{
	SDL_Color	color;

	color.r = 0;
	color.g = 0;
	color.b = 255;
	color.a = 255;
	return (color);
}

/**
 * @brief Renvoie la couleur jaune
 * 
 * @return SDL_Color Structure de couleur jaune
 */
SDL_Color	ui_color_yellow(void)
{
	SDL_Color	color;

	color.r = 255;
	color.g = 255;
	color.b = 0;
	color.a = 255;
	return (color);
}

/**
 * @brief Crée une couleur RGBA personnalisée
 * 
 * @param r Composante rouge (0-255)
 * @param g Composante verte (0-255)
 * @param b Composante bleue (0-255)
 * @param a Composante alpha (0-255)
 * @return SDL_Color Structure de couleur créée
 */
SDL_Color	ui_color_rgba(int r, int g, int b, int a)
{
	SDL_Color	color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return (color);
}
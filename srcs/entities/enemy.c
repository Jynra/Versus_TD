/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:30:15 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:53:50 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

static void	set_enemy_properties_by_type(t_enemy *enemy, int type);
static void	enemy_move(t_game *game, t_enemy *enemy);
static void	enemy_check_path_completion(t_game *game, t_enemy *enemy);

/**
 * @brief Initialise un ennemi avec ses propriétés
 * 
 * @param enemy Pointeur vers l'ennemi à initialiser
 * @param type Type d'ennemi
 * @param start_point Point de départ sur le chemin
 */
void	enemy_init(t_enemy *enemy, int type, t_path_point start_point)
{
	enemy->active = true;
	enemy->x = start_point.x;
	enemy->y = start_point.y;
	enemy->width = 30;
	enemy->height = 30;
	enemy->type = type;
	enemy->path_index = 0;
	enemy->slowed = false;
	enemy->slow_timer = 0.0f;
	
	set_enemy_properties_by_type(enemy, type);
}

/**
 * @brief Initialise le chemin que les ennemis suivront
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	path_init(t_game *game)
{
	/* Exemple de chemin simple de gauche à droite */
	game->path_length = 2;
	game->path[0].x = 0;
	game->path[0].y = 5 * GRID_SIZE + GRID_SIZE / 2; /* Milieu de la rangée 5 */
	game->path[1].x = WINDOW_WIDTH;
	game->path[1].y = 5 * GRID_SIZE + GRID_SIZE / 2;
	
	/* Pour un chemin plus complexe, ajouter plus de points */
	/* Exemple d'un chemin en zigzag:
	game->path_length = 6;
	game->path[0].x = 0;
	game->path[0].y = 1 * GRID_SIZE + GRID_SIZE / 2;
	game->path[1].x = WINDOW_WIDTH / 4;
	game->path[1].y = 1 * GRID_SIZE + GRID_SIZE / 2;
	game->path[2].x = WINDOW_WIDTH / 4;
	game->path[2].y = 8 * GRID_SIZE + GRID_SIZE / 2;
	game->path[3].x = WINDOW_WIDTH / 2;
	game->path[3].y = 8 * GRID_SIZE + GRID_SIZE / 2;
	game->path[4].x = WINDOW_WIDTH / 2;
	game->path[4].y = 3 * GRID_SIZE + GRID_SIZE / 2;
	game->path[5].x = WINDOW_WIDTH;
	game->path[5].y = 3 * GRID_SIZE + GRID_SIZE / 2;
	*/
}

/**
 * @brief Crée un nouvel ennemi
 * 
 * @param game Pointeur vers la structure du jeu
 * @param type Type d'ennemi à créer
 */
void	enemy_spawn(t_game *game, int type)
{
	int	i;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (!game->enemies[i].active)
		{
			enemy_init(&game->enemies[i], type, game->path[0]);
			break;
		}
		i++;
	}
}

/**
 * @brief Met à jour tous les ennemis actifs
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	enemies_update(t_game *game)
{
	int	i;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].active)
		{
			/* Gestion de l'effet de ralentissement */
			if (game->enemies[i].slowed)
			{
				game->enemies[i].slow_timer -= 1.0f / FPS;
				if (game->enemies[i].slow_timer <= 0)
				{
					game->enemies[i].slowed = false;
					game->enemies[i].speed = game->enemies[i].base_speed;
				}
			}
			
			/* Déplacement de l'ennemi */
			enemy_move(game, &game->enemies[i]);
			
			/* Vérification si l'ennemi a atteint la fin du chemin */
			enemy_check_path_completion(game, &game->enemies[i]);
			
			/* Vérification si l'ennemi est mort */
			if (game->enemies[i].health <= 0)
			{
				game->enemies[i].active = false;
				game_add_money(game, game->enemies[i].reward);
				game_add_score(game, game->enemies[i].score);
			}
		}
		i++;
	}
}

/**
 * @brief Inflige des dégâts à un ennemi
 * 
 * @param game Pointeur vers la structure du jeu
 * @param enemy Pointeur vers l'ennemi ciblé
 * @param damage Quantité de dégâts à infliger
 */
void	enemy_take_damage(t_game *game, t_enemy *enemy, int damage)
{
	if (!enemy || !enemy->active)
		return;
	
	enemy->health -= damage;
	
	if (enemy->health <= 0)
	{
		enemy->active = false;
		game_add_money(game, enemy->reward);
		game_add_score(game, enemy->score);
	}
}

/**
 * @brief Applique un effet de ralentissement à un ennemi
 * 
 * @param enemy Pointeur vers l'ennemi ciblé
 * @param slow_effect Facteur de ralentissement (0.0 à 1.0)
 * @param duration Durée de l'effet en secondes
 */
void	enemy_apply_slow(t_enemy *enemy, float slow_effect, float duration)
{
	if (!enemy || !enemy->active)
		return;
	
	enemy->slowed = true;
	
	/* Mise à jour de la durée seulement si elle est plus longue que la durée actuelle */
	if (enemy->slow_timer < duration)
		enemy->slow_timer = duration;
	
	/* Application du facteur de ralentissement */
	enemy->speed = enemy->base_speed * slow_effect;
}

/**
 * @brief Affiche tous les ennemis actifs
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	enemies_render(t_game *game)
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
			/* Rectangle principal de l'ennemi */
			rect.x = (int)game->enemies[i].x - game->enemies[i].width / 2;
			rect.y = (int)game->enemies[i].y - game->enemies[i].height / 2;
			rect.w = game->enemies[i].width;
			rect.h = game->enemies[i].height;
			
			/* Couleur différente selon le type et l'état */
			if (game->enemies[i].slowed)
			{
				/* Ennemi ralenti */
				if (game->enemies[i].type == ENEMY_TYPE_BASIC)
					SDL_SetRenderDrawColor(game->renderer, 150, 50, 50, 255);
				else if (game->enemies[i].type == ENEMY_TYPE_FAST)
					SDL_SetRenderDrawColor(game->renderer, 150, 100, 50, 255);
				else /* ENEMY_TYPE_BOSS */
					SDL_SetRenderDrawColor(game->renderer, 100, 50, 100, 255);
			}
			else
			{
				/* Ennemi normal */
				if (game->enemies[i].type == ENEMY_TYPE_BASIC)
					SDL_SetRenderDrawColor(game->renderer, 200, 0, 0, 255);
				else if (game->enemies[i].type == ENEMY_TYPE_FAST)
					SDL_SetRenderDrawColor(game->renderer, 255, 165, 0, 255);
				else /* ENEMY_TYPE_BOSS */
					SDL_SetRenderDrawColor(game->renderer, 200, 0, 200, 255);
			}
			
			SDL_RenderFillRect(game->renderer, &rect);
			
			/* Barre de vie */
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

/* Fonctions statiques internes */

/**
 * @brief Configure les propriétés d'un ennemi en fonction de son type
 * 
 * @param enemy Pointeur vers l'ennemi à configurer
 * @param type Type d'ennemi
 */
static void	set_enemy_properties_by_type(t_enemy *enemy, int type)
{
	if (type == ENEMY_TYPE_BASIC)
	{
		enemy->health = ENEMY_BASIC_HEALTH;
		enemy->max_health = ENEMY_BASIC_HEALTH;
		enemy->base_speed = ENEMY_BASIC_SPEED;
		enemy->speed = ENEMY_BASIC_SPEED;
		enemy->reward = ENEMY_BASIC_REWARD;
		enemy->score = ENEMY_BASIC_SCORE;
	}
	else if (type == ENEMY_TYPE_FAST)
	{
		enemy->health = ENEMY_FAST_HEALTH;
		enemy->max_health = ENEMY_FAST_HEALTH;
		enemy->base_speed = ENEMY_FAST_SPEED;
		enemy->speed = ENEMY_FAST_SPEED;
		enemy->reward = ENEMY_FAST_REWARD;
		enemy->score = ENEMY_FAST_SCORE;
	}
	else if (type == ENEMY_TYPE_BOSS)
	{
		enemy->health = ENEMY_BOSS_HEALTH;
		enemy->max_health = ENEMY_BOSS_HEALTH;
		enemy->base_speed = ENEMY_BOSS_SPEED;
		enemy->speed = ENEMY_BOSS_SPEED;
		enemy->reward = ENEMY_BOSS_REWARD;
		enemy->score = ENEMY_BOSS_SCORE;
		
		/* Le boss est légèrement plus grand */
		enemy->width = 40;
		enemy->height = 40;
	}
}

/**
 * @brief Déplace un ennemi le long du chemin
 * 
 * @param game Pointeur vers la structure du jeu
 * @param enemy Pointeur vers l'ennemi à déplacer
 */
static void	enemy_move(t_game *game, t_enemy *enemy)
{
	float	target_x;
	float	target_y;
	float	dx;
	float	dy;
	float	distance;
	
	/* Vérification de sécurité */
	if (enemy->path_index >= game->path_length - 1)
		return;
	
	/* Point cible suivant sur le chemin */
	target_x = game->path[enemy->path_index + 1].x;
	target_y = game->path[enemy->path_index + 1].y;
	
	/* Calcul du vecteur de direction */
	dx = target_x - enemy->x;
	dy = target_y - enemy->y;
	distance = sqrt(dx * dx + dy * dy);
	
	/* Si proche du point cible, passer au point suivant */
	if (distance < enemy->speed)
	{
		enemy->path_index++;
		
		/* Si c'était le dernier point, laisser la fonction de vérification
		 * de fin de chemin gérer la suite */
		if (enemy->path_index >= game->path_length - 1)
			return;
			
		/* Recalcul des vecteurs pour le nouveau point cible */
		target_x = game->path[enemy->path_index + 1].x;
		target_y = game->path[enemy->path_index + 1].y;
		dx = target_x - enemy->x;
		dy = target_y - enemy->y;
		distance = sqrt(dx * dx + dy * dy);
	}
	
	/* Déplacement dans la direction calculée */
	if (distance > 0)
	{
		enemy->x += dx / distance * enemy->speed;
		enemy->y += dy / distance * enemy->speed;
	}
}

/**
 * @brief Vérifie si un ennemi a atteint la fin du chemin
 * 
 * @param game Pointeur vers la structure du jeu
 * @param enemy Pointeur vers l'ennemi à vérifier
 */
static void	enemy_check_path_completion(t_game *game, t_enemy *enemy)
{
	/* Si l'ennemi a atteint le dernier point du chemin */
	if (enemy->path_index >= game->path_length - 1)
	{
		enemy->active = false;
		game->lives--;
		
		/* Vérification de la condition de fin de jeu */
		if (game->lives <= 0)
		{
			game->state = STATE_GAME_OVER;
			printf("Game Over! Final score: %d\n", game->score);
		}
	}
}
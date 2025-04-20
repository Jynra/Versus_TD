/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projectile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:10:22 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 01:54:24 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core/game.h"

/**
 * @brief Initialise un projectile avec ses propriétés
 * 
 * @param projectile Pointeur vers le projectile à initialiser
 * @param x Position X de départ
 * @param y Position Y de départ
 * @param target_x Position X de la cible
 * @param target_y Position Y de la cible
 * @param damage Dégâts que le projectile infligera
 * @param source_type Type de la tour qui a tiré ce projectile
 */
void	projectile_init(t_projectile *projectile, float x, float y, 
			float target_x, float target_y, int damage, int source_type)
{
	float	dx;
	float	dy;
	float	length;

	projectile->x = x;
	projectile->y = y;
	projectile->active = true;
	projectile->damage = damage;
	projectile->speed = PROJECTILE_SPEED;
	projectile->source_type = source_type;
	
	/* Calcul du vecteur de direction normalisé */
	dx = target_x - x;
	dy = target_y - y;
	length = sqrt(dx * dx + dy * dy);
	
	if (length > 0)
	{
		projectile->dx = dx / length * projectile->speed;
		projectile->dy = dy / length * projectile->speed;
	}
	else
	{
		/* En cas d'erreur, direction par défaut vers la droite */
		projectile->dx = projectile->speed;
		projectile->dy = 0;
	}
}

/**
 * @brief Crée un nouveau projectile
 * 
 * @param game Pointeur vers la structure du jeu
 * @param x Position X de départ
 * @param y Position Y de départ
 * @param target_x Position X de la cible
 * @param target_y Position Y de la cible
 * @param damage Dégâts que le projectile infligera
 * @param source_type Type de la tour qui a tiré ce projectile
 */
void	projectile_create(t_game *game, float x, float y, 
			float target_x, float target_y, int damage, int source_type)
{
	int	i;

	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (!game->projectiles[i].active)
		{
			projectile_init(&game->projectiles[i], x, y, 
					target_x, target_y, damage, source_type);
			break;
		}
		i++;
	}
}

/**
 * @brief Met à jour tous les projectiles actifs
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	projectiles_update(t_game *game)
{
	int	i;

	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (game->projectiles[i].active)
		{
			/* Mise à jour de la position */
			game->projectiles[i].x += game->projectiles[i].dx;
			game->projectiles[i].y += game->projectiles[i].dy;
			
			/* Vérification si le projectile est sorti de l'écran */
			if (game->projectiles[i].x < 0 || game->projectiles[i].x > WINDOW_WIDTH
				|| game->projectiles[i].y < 0 || game->projectiles[i].y > WINDOW_HEIGHT)
			{
				game->projectiles[i].active = false;
			}
			/* Vérification des collisions avec les ennemis */
			else if (projectile_check_collision(game, &game->projectiles[i]))
			{
				game->projectiles[i].active = false;
			}
		}
		i++;
	}
}

/**
 * @brief Vérifie si un projectile entre en collision avec un ennemi
 * 
 * @param game Pointeur vers la structure du jeu
 * @param projectile Pointeur vers le projectile à vérifier
 * @return true si une collision a lieu, false sinon
 */
bool	projectile_check_collision(t_game *game, t_projectile *projectile)
{
	int		j;
	float	dist;
	bool	collision;

	collision = false;
	j = 0;
	while (j < MAX_ENEMIES)
	{
		if (game->enemies[j].active)
		{
			dist = distance(projectile->x, projectile->y,
					game->enemies[j].x, game->enemies[j].y);
					
			if (dist < game->enemies[j].width / 2)
			{
				/* Dégâts infligés à l'ennemi */
				enemy_take_damage(game, &game->enemies[j], projectile->damage);
				
				/* Effet spécial selon le type de projectile */
				if (projectile->source_type == TOWER_TYPE_SLOW)
				{
					enemy_apply_slow(&game->enemies[j], 
						TOWER_SLOW_EFFECT, TOWER_SLOW_DURATION);
				}
				
				collision = true;
				break;
			}
		}
		j++;
	}
	
	return (collision);
}

/**
 * @brief Affiche tous les projectiles actifs
 * 
 * @param game Pointeur vers la structure du jeu
 */
void	projectiles_render(t_game *game)
{
	int			i;
	SDL_Rect	rect;
	int			size;

	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (game->projectiles[i].active)
		{
			/* Taille différente selon le type de projectile */
			if (game->projectiles[i].source_type == TOWER_TYPE_BASIC)
				size = PROJECTILE_BASIC_SIZE;
			else if (game->projectiles[i].source_type == TOWER_TYPE_SLOW)
				size = PROJECTILE_SLOW_SIZE;
			else /* TOWER_TYPE_MULTI */
				size = PROJECTILE_MULTI_SIZE;
				
			rect.x = (int)game->projectiles[i].x - size / 2;
			rect.y = (int)game->projectiles[i].y - size / 2;
			rect.w = size;
			rect.h = size;
			
			/* Couleur différente selon le type de projectile */
			if (game->projectiles[i].source_type == TOWER_TYPE_BASIC)
				SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
			else if (game->projectiles[i].source_type == TOWER_TYPE_SLOW)
				SDL_SetRenderDrawColor(game->renderer, 0, 255, 255, 255);
			else if (game->projectiles[i].source_type == TOWER_TYPE_MULTI)
				SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 255);
			else
				SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
				
			SDL_RenderFillRect(game->renderer, &rect);
		}
		i++;
	}
}
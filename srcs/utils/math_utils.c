/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:35:15 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:10:35 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/utils/math_utils.h"
#include "../../includes/core/game.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Calcule la distance euclidienne entre deux points
 * 
 * @param x1 Coordonnée X du premier point
 * @param y1 Coordonnée Y du premier point
 * @param x2 Coordonnée X du deuxième point
 * @param y2 Coordonnée Y du deuxième point
 * @return float Distance entre les deux points
 */
float	distance(float x1, float y1, float x2, float y2)
{
	float	dx;
	float	dy;

	dx = x2 - x1;
	dy = y2 - y1;
	return (sqrt(dx * dx + dy * dy));
}

/**
 * @brief Calcule l'interpolation linéaire entre deux valeurs
 * 
 * @param a Première valeur
 * @param b Deuxième valeur
 * @param t Facteur d'interpolation (0.0 à 1.0)
 * @return float Valeur interpolée
 */
float	lerp(float a, float b, float t)
{
	return (a + t * (b - a));
}

/**
 * @brief Limite une valeur entre un minimum et un maximum
 * 
 * @param value Valeur à limiter
 * @param min Limite inférieure
 * @param max Limite supérieure
 * @return float Valeur limitée
 */
float	clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

/**
 * @brief Convertit des radians en degrés
 * 
 * @param rad Angle en radians
 * @return float Angle en degrés
 */
float	rad_to_deg(float rad)
{
	return (rad * 180.0f / M_PI);
}

/**
 * @brief Convertit des degrés en radians
 * 
 * @param deg Angle en degrés
 * @return float Angle en radians
 */
float	deg_to_rad(float deg)
{
	return (deg * M_PI / 180.0f);
}

/**
 * @brief Calcule la longueur d'un vecteur 2D
 * 
 * @param x Composante X du vecteur
 * @param y Composante Y du vecteur
 * @return float Longueur du vecteur
 */
float	vector_length(float x, float y)
{
	return (sqrt(x * x + y * y));
}

/**
 * @brief Normalise un vecteur 2D (longueur = 1)
 * 
 * @param x Pointeur vers la composante X du vecteur
 * @param y Pointeur vers la composante Y du vecteur
 */
void	normalize_vector(float *x, float *y)
{
	float	length;

	length = vector_length(*x, *y);
	if (length > 0)
	{
		*x /= length;
		*y /= length;
	}
}

/**
 * @brief Génère un nombre entier aléatoire entre min et max inclus
 * 
 * @param min Valeur minimale
 * @param max Valeur maximale
 * @return int Nombre aléatoire
 */
int	random_int(int min, int max)
{
	static int	initialized = 0;
	
	if (!initialized)
	{
		srand(time(NULL));
		initialized = 1;
	}
	
	return (min + rand() % (max - min + 1));
}

/**
 * @brief Génère un nombre flottant aléatoire entre min et max
 * 
 * @param min Valeur minimale
 * @param max Valeur maximale
 * @return float Nombre aléatoire
 */
float	random_float(float min, float max)
{
	static int	initialized = 0;
	
	if (!initialized)
	{
		srand(time(NULL));
		initialized = 1;
	}
	
	return (min + (max - min) * (rand() / (float)RAND_MAX));
}

/**
 * @brief Convertit une coordonnée de grille X en coordonnée pixel
 * 
 * @param grid_x Coordonnée X de la grille
 * @return int Coordonnée X en pixels
 */
int	grid_to_pixel_x(int grid_x)
{
	return (grid_x * GRID_SIZE + GRID_SIZE / 2);
}

/**
 * @brief Convertit une coordonnée de grille Y en coordonnée pixel
 * 
 * @param grid_y Coordonnée Y de la grille
 * @return int Coordonnée Y en pixels
 */
int	grid_to_pixel_y(int grid_y)
{
	return (grid_y * GRID_SIZE + GRID_SIZE / 2);
}

/**
 * @brief Convertit une coordonnée pixel X en coordonnée de grille
 * 
 * @param pixel_x Coordonnée X en pixels
 * @return int Coordonnée X de la grille
 */
int	pixel_to_grid_x(int pixel_x)
{
	return (pixel_x / GRID_SIZE);
}

/**
 * @brief Convertit une coordonnée pixel Y en coordonnée de grille
 * 
 * @param pixel_y Coordonnée Y en pixels
 * @return int Coordonnée Y de la grille
 */
int	pixel_to_grid_y(int pixel_y)
{
	return (pixel_y / GRID_SIZE);
}
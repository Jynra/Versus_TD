/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:08:22 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 02:10:27 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# define _USE_MATH_DEFINES  /* Pour accéder à M_PI et autres constantes */
# include <math.h>

/* Si M_PI n'est toujours pas défini, le définir manuellement */
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

/* Fonctions mathématiques */
float	distance(float x1, float y1, float x2, float y2);
float	lerp(float a, float b, float t);
float	clamp(float value, float min, float max);
float	rad_to_deg(float rad);
float	deg_to_rad(float deg);
float	vector_length(float x, float y);
void	normalize_vector(float *x, float *y);
int		random_int(int min, int max);
float	random_float(float min, float max);
int		grid_to_pixel_x(int grid_x);
int		grid_to_pixel_y(int grid_y);
int		pixel_to_grid_x(int pixel_x);
int		pixel_to_grid_y(int pixel_y);

#endif
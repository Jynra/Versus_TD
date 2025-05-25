/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:02:00 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

float	math_distance(t_vector2 a, t_vector2 b)
{
	float	dx;
	float	dy;

	dx = b.x - a.x;
	dy = b.y - a.y;
	return (sqrtf(dx * dx + dy * dy));
}

float	math_distance_squared(t_vector2 a, t_vector2 b)
{
	float	dx;
	float	dy;

	dx = b.x - a.x;
	dy = b.y - a.y;
	return (dx * dx + dy * dy);
}

t_vector2	math_normalize(t_vector2 vec)
{
	float		magnitude;
	t_vector2	result;

	magnitude = math_magnitude(vec);
	if (magnitude == 0.0f)
		return (vec2_zero());
	result.x = vec.x / magnitude;
	result.y = vec.y / magnitude;
	return (result);
}

float	math_magnitude(t_vector2 vec)
{
	return (sqrtf(vec.x * vec.x + vec.y * vec.y));
}

float	math_magnitude_squared(t_vector2 vec)
{
	return (vec.x * vec.x + vec.y * vec.y);
}

t_vector2	math_rotate(t_vector2 vec, float angle)
{
	t_vector2	result;
	float		cos_a;
	float		sin_a;

	cos_a = cosf(angle);
	sin_a = sinf(angle);
	result.x = vec.x * cos_a - vec.y * sin_a;
	result.y = vec.x * sin_a + vec.y * cos_a;
	return (result);
}

float	math_angle_between(t_vector2 a, t_vector2 b)
{
	return (atan2f(b.y - a.y, b.x - a.x));
}

float	math_lerp(float a, float b, float t)
{
	return (a + (b - a) * t);
}

float	math_clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

int	math_clamp_int(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

float	math_deg_to_rad(float degrees)
{
	return (degrees * PI / 180.0f);
}

float	math_rad_to_deg(float radians)
{
	return (radians * 180.0f / PI);
}

t_vector2	vec2_add(t_vector2 a, t_vector2 b)
{
	t_vector2	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

t_vector2	vec2_subtract(t_vector2 a, t_vector2 b)
{
	t_vector2	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

t_vector2	vec2_multiply(t_vector2 vec, float scalar)
{
	t_vector2	result;

	result.x = vec.x * scalar;
	result.y = vec.y * scalar;
	return (result);
}

t_vector2	vec2_divide(t_vector2 vec, float scalar)
{
	t_vector2	result;

	if (scalar == 0.0f)
		return (vec2_zero());
	result.x = vec.x / scalar;
	result.y = vec.y / scalar;
	return (result);
}

t_vector2	vec2_zero(void)
{
	t_vector2	result;

	result.x = 0.0f;
	result.y = 0.0f;
	return (result);
}

t_vector2	vec2_one(void)
{
	t_vector2	result;

	result.x = 1.0f;
	result.y = 1.0f;
	return (result);
}

t_vector2	vec2_create(float x, float y)
{
	t_vector2	result;

	result.x = x;
	result.y = y;
	return (result);
}

bool	vec2_equals(t_vector2 a, t_vector2 b)
{
	const float	epsilon = 0.0001f;

	return (fabsf(a.x - b.x) < epsilon && fabsf(a.y - b.y) < epsilon);
}

t_color	color_create(int r, int g, int b, int a)
{
	t_color	result;

	result.r = math_clamp_int(r, 0, 255);
	result.g = math_clamp_int(g, 0, 255);
	result.b = math_clamp_int(b, 0, 255);
	result.a = math_clamp_int(a, 0, 255);
	return (result);
}

t_color	color_white(void)
{
	return (color_create(255, 255, 255, 255));
}

t_color	color_black(void)
{
	return (color_create(0, 0, 0, 255));
}

t_color	color_red(void)
{
	return (color_create(255, 0, 0, 255));
}

t_color	color_green(void)
{
	return (color_create(0, 255, 0, 255));
}

t_color	color_blue(void)
{
	return (color_create(0, 0, 255, 255));
}

t_color	color_yellow(void)
{
	return (color_create(255, 255, 0, 255));
}

t_color	color_gray(void)
{
	return (color_create(128, 128, 128, 255));
}

t_color	color_lerp(t_color a, t_color b, float t)
{
	t_color	result;

	result.r = (int)math_lerp((float)a.r, (float)b.r, t);
	result.g = (int)math_lerp((float)a.g, (float)b.g, t);
	result.b = (int)math_lerp((float)a.b, (float)b.b, t);
	result.a = (int)math_lerp((float)a.a, (float)b.a, t);
	return (result);
}

SDL_Color	color_to_sdl(t_color color)
{
	SDL_Color	result;

	result.r = (Uint8)color.r;
	result.g = (Uint8)color.g;
	result.b = (Uint8)color.b;
	result.a = (Uint8)color.a;
	return (result);
}

t_vector2	world_to_grid(t_vector2 world_pos)
{
	t_vector2	result;

	result.x = (float)world_to_grid_x(world_pos.x);
	result.y = (float)world_to_grid_y(world_pos.y);
	return (result);
}

t_vector2	grid_to_world(int grid_x, int grid_y)
{
	t_vector2	result;

	result.x = grid_to_world_x(grid_x);
	result.y = grid_to_world_y(grid_y);
	return (result);
}

bool	is_valid_grid_pos(int x, int y)
{
	return (x >= 0 && x < GRID_COLS && y >= 0 && y < GRID_ROWS);
}

int	world_to_grid_x(float world_x)
{
	return ((int)(world_x / GRID_SIZE));
}

int	world_to_grid_y(float world_y)
{
	return ((int)(world_y / GRID_SIZE));
}

float	grid_to_world_x(int grid_x)
{
	return ((float)grid_x * GRID_SIZE + GRID_SIZE * 0.5f);
}

float	grid_to_world_y(int grid_y)
{
	return ((float)grid_y * GRID_SIZE + GRID_SIZE * 0.5f);
}
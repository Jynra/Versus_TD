/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 12:29:30 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "types.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include <time.h>

/* Math Utilities */
float		math_distance(t_vector2 a, t_vector2 b);
float		math_distance_squared(t_vector2 a, t_vector2 b);
t_vector2	math_normalize(t_vector2 vec);
float		math_magnitude(t_vector2 vec);
float		math_magnitude_squared(t_vector2 vec);
t_vector2	math_rotate(t_vector2 vec, float angle);
float		math_angle_between(t_vector2 a, t_vector2 b);
float		math_lerp(float a, float b, float t);
float		math_clamp(float value, float min, float max);
int			math_clamp_int(int value, int min, int max);
float		math_deg_to_rad(float degrees);
float		math_rad_to_deg(float radians);

/* Vector Operations */
t_vector2	vec2_add(t_vector2 a, t_vector2 b);
t_vector2	vec2_subtract(t_vector2 a, t_vector2 b);
t_vector2	vec2_multiply(t_vector2 vec, float scalar);
t_vector2	vec2_divide(t_vector2 vec, float scalar);
t_vector2	vec2_zero(void);
t_vector2	vec2_one(void);
t_vector2	vec2_create(float x, float y);
bool		vec2_equals(t_vector2 a, t_vector2 b);

/* Color Utilities */
t_color		color_create(int r, int g, int b, int a);
t_color		color_white(void);
t_color		color_black(void);
t_color		color_red(void);
t_color		color_green(void);
t_color		color_blue(void);
t_color		color_yellow(void);
t_color		color_gray(void);
t_color		color_lerp(t_color a, t_color b, float t);
SDL_Color	color_to_sdl(t_color color);

/* Rectangle Utilities */
t_rect		rect_create(int x, int y, int w, int h);
bool		rect_contains_point(t_rect rect, t_vector2 point);
bool		rect_intersects(t_rect a, t_rect b);
t_rect		rect_intersection(t_rect a, t_rect b);
t_vector2	rect_center(t_rect rect);

/* Memory Management */
void		*memory_alloc(size_t size);
void		memory_free(void *ptr);
void		*memory_realloc(void *ptr, size_t size);
void		memory_set(void *ptr, int value, size_t size);
void		memory_copy(void *dest, const void *src, size_t size);
int			memory_compare(const void *a, const void *b, size_t size);

/* File Operations */
bool		file_exists(const char *filename);
char		*file_read_text(const char *filename);
bool		file_write_text(const char *filename, const char *content);
size_t		file_get_size(const char *filename);
bool		file_copy(const char *src, const char *dest);
bool		file_delete(const char *filename);

/* String Utilities */
int			string_length(const char *str);
char		*string_copy(char *dest, const char *src);
char		*string_concat(char *dest, const char *src);
int			string_compare(const char *a, const char *b);
char		*string_find(const char *haystack, const char *needle);
void		string_to_lower(char *str);
void		string_to_upper(char *str);
bool		string_starts_with(const char *str, const char *prefix);
bool		string_ends_with(const char *str, const char *suffix);

/* Random Number Generation */
void		random_seed(unsigned int seed);
int			random_int(int min, int max);
float		random_float(float min, float max);
bool		random_chance(float probability);
t_vector2	random_vector2(float min_x, float max_x, float min_y, float max_y);
t_color		random_color(void);

/* Debug Utilities */
void		debug_log(const char *format, ...);
void		debug_error(const char *format, ...);
void		debug_warning(const char *format, ...);
void		debug_info(const char *format, ...);
void		debug_print_vector2(const char *name, t_vector2 vec);
void		debug_print_rect(const char *name, t_rect rect);
void		debug_print_color(const char *name, t_color color);

/* Time Utilities */
float		time_get_delta(void);
Uint32		time_get_ticks(void);
void		time_delay(Uint32 ms);
float		time_get_fps(void);

/* Grid/World Conversion */
t_vector2	world_to_grid(t_vector2 world_pos);
t_vector2	grid_to_world(int grid_x, int grid_y);
bool		is_valid_grid_pos(int x, int y);
int			world_to_grid_x(float world_x);
int			world_to_grid_y(float world_y);
float		grid_to_world_x(int grid_x);
float		grid_to_world_y(int grid_y);

/* Pathfinding Utilities */
float		pathfind_heuristic(t_vector2 a, t_vector2 b);
bool		pathfind_is_walkable(t_game *game, int x, int y);
t_vector2	pathfind_get_direction(t_vector2 from, t_vector2 to);

/* Performance Utilities */
void		perf_start_timer(const char *name);
void		perf_end_timer(const char *name);
void		perf_print_stats(void);
void		perf_reset_stats(void);

/* Validation Utilities */
bool		validate_pointer(const void *ptr);
bool		validate_range_int(int value, int min, int max);
bool		validate_range_float(float value, float min, float max);
bool		validate_string(const char *str);

#endif
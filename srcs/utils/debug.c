/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 13:25:18 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdarg.h>
#include <time.h>

static void	debug_print_time(void);
static void	debug_print_level(const char *level, const char *color);

void	debug_log(const char *format, ...)
{
	va_list	args;

	debug_print_time();
	debug_print_level("LOG", "\033[0;37m");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\033[0m\n");
	fflush(stdout);
}

void	debug_error(const char *format, ...)
{
	va_list	args;

	debug_print_time();
	debug_print_level("ERROR", "\033[0;31m");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\033[0m\n");
	fflush(stderr);
}

void	debug_warning(const char *format, ...)
{
	va_list	args;

	debug_print_time();
	debug_print_level("WARN", "\033[0;33m");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\033[0m\n");
	fflush(stdout);
}

void	debug_info(const char *format, ...)
{
	va_list	args;

	debug_print_time();
	debug_print_level("INFO", "\033[0;32m");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\033[0m\n");
	fflush(stdout);
}

void	debug_print_vector2(const char *name, t_vector2 vec)
{
	debug_info("%s: (%.2f, %.2f)", name, vec.x, vec.y);
}

void	debug_print_rect(const char *name, t_rect rect)
{
	debug_info("%s: (%d, %d, %d, %d)", name, rect.x, rect.y, rect.w, rect.h);
}

void	debug_print_color(const char *name, t_color color)
{
	debug_info("%s: (%d, %d, %d, %d)", name, color.r, color.g, color.b, 
		color.a);
}

static void	debug_print_time(void)
{
	time_t		raw_time;
	struct tm	*time_info;
	char		time_str[20];

	time(&raw_time);
	time_info = localtime(&raw_time);
	strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);
	printf("[%s] ", time_str);
}

static void	debug_print_level(const char *level, const char *color)
{
	printf("%s[%-5s]\033[0m ", color, level);
}

float	time_get_delta(void)
{
	static Uint32	last_time = 0;
	Uint32			current_time;
	float			delta;

	current_time = SDL_GetTicks();
	if (last_time == 0)
		last_time = current_time;
	delta = (current_time - last_time) / 1000.0f;
	last_time = current_time;
	return (delta);
}

Uint32	time_get_ticks(void)
{
	return (SDL_GetTicks());
}

void	time_delay(Uint32 ms)
{
	SDL_Delay(ms);
}

float	time_get_fps(void)
{
	static Uint32	last_time = 0;
	static int		frame_count = 0;
	static float	fps = 0.0f;
	Uint32			current_time;

	frame_count++;
	current_time = SDL_GetTicks();
	if (current_time - last_time >= 1000)
	{
		fps = frame_count * 1000.0f / (current_time - last_time);
		frame_count = 0;
		last_time = current_time;
	}
	return (fps);
}

bool	validate_pointer(const void *ptr)
{
	if (!ptr)
	{
		printf("ERROR: Null pointer detected\n");
		return (false);
	}
	return (true);
}

bool	validate_range_int(int value, int min, int max)
{
	if (value < min || value > max)
	{
		printf("WARN: Value %d out of range [%d, %d]\n", value, min, max);
		return (false);
	}
	return (true);
}

bool	validate_range_float(float value, float min, float max)
{
	if (value < min || value > max)
	{
		printf("WARN: Value %.2f out of range [%.2f, %.2f]\n", value, min, max);
		return (false);
	}
	return (true);
}

bool	validate_string(const char *str)
{
	if (!str)
	{
		printf("ERROR: Null string detected\n");
		return (false);
	}
	if (string_length(str) == 0)
	{
		printf("WARN: Empty string detected\n");
		return (false);
	}
	return (true);
}

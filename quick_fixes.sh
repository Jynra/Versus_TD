#!/bin/bash

# Script de corrections rapides pour la compilation

echo "🔧 Application des corrections rapides..."

# 1. Corriger les includes manquants dans debug.c
cat > srcs/utils/debug_fixed.c << 'EOF'
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
EOF

# 2. Créer un systems/basic_systems.c minimal qui compile
mkdir -p srcs/systems
cat > srcs/systems/basic_systems.c << 'EOF'
#include "systems.h"

void	input_init(t_input *input)
{
	int	i;

	if (!input)
		return ;
	input->mouse_pos = vec2_zero();
	input->mouse_world_pos = vec2_zero();
	input->mouse_left_down = false;
	input->mouse_right_down = false;
	input->mouse_left_clicked = false;
	input->mouse_right_clicked = false;
	i = 0;
	while (i < SDL_NUM_SCANCODES)
	{
		input->keys[i] = false;
		input->keys_pressed[i] = false;
		i++;
	}
}

void	input_update(t_input *input)
{
	int	i;

	if (!input)
		return ;
	input->mouse_left_clicked = false;
	input->mouse_right_clicked = false;
	i = 0;
	while (i < SDL_NUM_SCANCODES)
	{
		input->keys_pressed[i] = false;
		i++;
	}
}

void	input_handle_event(t_input *input, SDL_Event *event)
{
	if (!input || !event)
		return ;
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
		{
			input->mouse_left_down = true;
			input->mouse_left_clicked = true;
		}
		else if (event->button.button == SDL_BUTTON_RIGHT)
		{
			input->mouse_right_down = true;
			input->mouse_right_clicked = true;
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		if (event->button.button == SDL_BUTTON_LEFT)
			input->mouse_left_down = false;
		else if (event->button.button == SDL_BUTTON_RIGHT)
			input->mouse_right_down = false;
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		input->mouse_pos.x = (float)event->motion.x;
		input->mouse_pos.y = (float)event->motion.y;
		input->mouse_world_pos = input->mouse_pos;
	}
	else if (event->type == SDL_KEYDOWN && !event->key.repeat)
	{
		input->keys[event->key.keysym.scancode] = true;
		input->keys_pressed[event->key.keysym.scancode] = true;
	}
	else if (event->type == SDL_KEYUP)
		input->keys[event->key.keysym.scancode] = false;
}

void	effects_init(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = 0;
	while (i < MAX_PARTICLES)
	{
		game->particles[i].active = false;
		i++;
	}
}

void	waves_init(t_wave *wave)
{
	int	i;

	if (!wave)
		return ;
	wave->number = 1;
	wave->enemies_total = WAVE_BASE_ENEMIES;
	wave->enemies_spawned = 0;
	wave->enemies_alive = 0;
	wave->spawn_delay = WAVE_SPAWN_DELAY_BASE;
	wave->spawn_timer = 0.0f;
	wave->prep_timer = 0.0f;
	wave->active = false;
	wave->completed = false;
	wave->preparing = true;
	wave->current_spawn_type = 0;
	i = 0;
	while (i < 4)
	{
		wave->enemy_types[i] = ENEMY_BASIC;
		wave->enemy_counts[i] = 0;
		i++;
	}
	wave->enemy_types[0] = ENEMY_BASIC;
	wave->enemy_counts[0] = wave->enemies_total;
}

void	notifications_init(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = 0;
	while (i < MAX_NOTIFICATIONS)
	{
		game->notifications[i].active = false;
		i++;
	}
}

void	ui_init(t_game *game)
{
	int	i;

	if (!game)
		return ;
	i = 0;
	while (i < 4)
	{
		game->tower_buttons[i].rect = rect_create(
			10 + i * (UI_BUTTON_WIDTH + UI_BUTTON_PADDING),
			GAME_AREA_HEIGHT + UI_BUTTON_PADDING,
			UI_BUTTON_WIDTH,
			UI_BUTTON_HEIGHT
		);
		game->tower_buttons[i].type = i + 1;
		game->tower_buttons[i].enabled = true;
		game->tower_buttons[i].pressed = false;
		game->tower_buttons[i].hovered = false;
		game->tower_buttons[i].color = color_gray();
		i++;
	}
	string_copy(game->tower_buttons[0].text, "Basic");
	string_copy(game->tower_buttons[1].text, "Sniper");
	string_copy(game->tower_buttons[2].text, "Cannon");
	string_copy(game->tower_buttons[3].text, "Freeze");
}
EOF

echo "✅ Corrections appliquées"
echo "📝 Remplacez maintenant:"
echo "   mv srcs/utils/debug_fixed.c srcs/utils/debug.c"
echo "   Ajoutez srcs/systems/basic_systems.c au Makefile"
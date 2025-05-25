/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   effects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 18:30:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 19:18:55 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "systems.h"
#include "utils.h"
#include "game.h"

/* Forward declarations */
static void	particle_init(t_particle *particle);
static void	particle_update(t_particle *particle, float delta_time);
static void	particle_render(t_particle *particle, t_game *game);
static void	create_particle(t_game *game, t_vector2 pos, t_vector2 vel,
				int type, t_color color, float lifetime);

void	effects_init(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < MAX_PARTICLES)
	{
		particle_init(&game->particles[i]);
		i++;
	}
	debug_info("Effects system initialized with %d particle slots", 
		MAX_PARTICLES);
}

void	effects_update(t_game *game)
{
	int	i;
	int	active_count;

	if (!validate_pointer(game))
		return ;
	active_count = 0;
	i = 0;
	while (i < MAX_PARTICLES)
	{
		if (game->particles[i].active)
		{
			particle_update(&game->particles[i], game->delta_time);
			active_count++;
		}
		i++;
	}
	game->active_particles = active_count;
}

void	effects_render(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < MAX_PARTICLES)
	{
		if (game->particles[i].active)
			particle_render(&game->particles[i], game);
		i++;
	}
}

void	effects_create_explosion(t_game *game, t_vector2 pos, int intensity)
{
	t_vector2	particle_vel;
	int			particle_count;
	int			i;
	float		angle;
	float		speed;
	t_color		explosion_color;

	if (!validate_pointer(game))
		return ;
	particle_count = math_clamp_int(intensity * 3, 5, 20);
	explosion_color = color_create(255, 200, 100, 255);
	
	i = 0;
	while (i < particle_count)
	{
		angle = random_float(0, TWO_PI);
		speed = random_float(PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX);
		particle_vel.x = cosf(angle) * speed;
		particle_vel.y = sinf(angle) * speed;
		create_particle(game, pos, particle_vel, PARTICLE_EXPLOSION, 
			explosion_color, random_float(PARTICLE_LIFETIME_MIN, PARTICLE_LIFETIME_MAX));
		i++;
	}
	debug_log("Explosion created: %d particles at (%.1f, %.1f)", 
		particle_count, pos.x, pos.y);
}

void	effects_create_blood(t_game *game, t_vector2 pos)
{
	t_vector2	particle_vel;
	int			particle_count;
	int			i;
	float		angle;
	float		speed;
	t_color		blood_color;

	if (!validate_pointer(game))
		return ;
	particle_count = random_int(3, 8);
	blood_color = color_create(255, 0, 0, 255);
	
	i = 0;
	while (i < particle_count)
	{
		angle = random_float(-HALF_PI - 0.5f, -HALF_PI + 0.5f);
		speed = random_float(30.0f, 80.0f);
		particle_vel.x = cosf(angle) * speed;
		particle_vel.y = sinf(angle) * speed;
		create_particle(game, pos, particle_vel, PARTICLE_BLOOD, 
			blood_color, random_float(0.5f, 1.5f));
		i++;
	}
}

void	effects_create_smoke(t_game *game, t_vector2 pos)
{
	t_vector2	particle_vel;
	int			particle_count;
	int			i;
	t_color		smoke_color;

	if (!validate_pointer(game))
		return ;
	particle_count = random_int(2, 5);
	smoke_color = color_create(200, 200, 200, 150);
	
	i = 0;
	while (i < particle_count)
	{
		particle_vel.x = random_float(-20.0f, 20.0f);
		particle_vel.y = random_float(-30.0f, -10.0f);
		create_particle(game, pos, particle_vel, PARTICLE_SMOKE, 
			smoke_color, random_float(1.0f, 2.5f));
		i++;
	}
}

void	effects_create_sparks(t_game *game, t_vector2 pos, int count)
{
	t_vector2	particle_vel;
	int			i;
	float		angle;
	float		speed;
	t_color		spark_color;

	if (!validate_pointer(game))
		return ;
	count = math_clamp_int(count, 1, 15);
	spark_color = color_create(255, 255, 150, 255);
	
	i = 0;
	while (i < count)
	{
		angle = random_float(0, TWO_PI);
		speed = random_float(40.0f, 120.0f);
		particle_vel.x = cosf(angle) * speed;
		particle_vel.y = sinf(angle) * speed;
		create_particle(game, pos, particle_vel, PARTICLE_SPARK, 
			spark_color, random_float(0.3f, 1.0f));
		i++;
	}
}

void	effects_create_freeze_aura(t_game *game, t_vector2 pos, float radius)
{
	t_vector2	particle_pos;
	t_vector2	particle_vel;
	int			particle_count;
	int			i;
	float		angle;
	float		distance;
	t_color		freeze_color;

	if (!validate_pointer(game))
		return ;
	particle_count = (int)(radius * 0.3f);
	particle_count = math_clamp_int(particle_count, 5, 25);
	freeze_color = color_create(150, 200, 255, 200);
	
	i = 0;
	while (i < particle_count)
	{
		angle = random_float(0, TWO_PI);
		distance = random_float(radius * 0.3f, radius * 0.8f);
		particle_pos.x = pos.x + cosf(angle) * distance;
		particle_pos.y = pos.y + sinf(angle) * distance;
		particle_vel.x = random_float(-10.0f, 10.0f);
		particle_vel.y = random_float(-20.0f, -5.0f);
		create_particle(game, particle_pos, particle_vel, PARTICLE_SPARK, 
			freeze_color, random_float(0.8f, 1.5f));
		i++;
	}
}

int	effects_find_free_particle(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return (-1);
	i = 0;
	while (i < MAX_PARTICLES)
	{
		if (!game->particles[i].active)
			return (i);
		i++;
	}
	return (-1);
}

void	effects_cleanup_particles(t_game *game)
{
	int	i;
	int	cleaned;

	if (!validate_pointer(game))
		return ;
	cleaned = 0;
	i = 0;
	while (i < MAX_PARTICLES)
	{
		if (game->particles[i].active && game->particles[i].life <= 0.0f)
		{
			game->particles[i].active = false;
			cleaned++;
		}
		i++;
	}
	if (cleaned > 0)
		debug_log("Cleaned up %d expired particles", cleaned);
}

static void	particle_init(t_particle *particle)
{
	if (!validate_pointer(particle))
		return ;
	particle->pos = vec2_zero();
	particle->vel = vec2_zero();
	particle->life = 0.0f;
	particle->max_life = 1.0f;
	particle->size = 1.0f;
	particle->start_size = 1.0f;
	particle->color = color_white();
	particle->start_color = color_white();
	particle->type = PARTICLE_EXPLOSION;
	particle->active = false;
	particle->gravity = 0.0f;
}

static void	particle_update(t_particle *particle, float delta_time)
{
	float	life_ratio;
	float	size_progress;
	float	alpha_progress;

	if (!validate_pointer(particle) || !particle->active)
		return ;
	particle->life -= delta_time;
	if (particle->life <= 0.0f)
	{
		particle->active = false;
		return ;
	}
	particle->vel.y += particle->gravity * delta_time;
	particle->pos = vec2_add(particle->pos, 
		vec2_multiply(particle->vel, delta_time));
	life_ratio = particle->life / particle->max_life;
	size_progress = 1.0f - life_ratio;
	particle->size = math_lerp(particle->start_size, 
		particle->start_size * 0.1f, size_progress);
	alpha_progress = 1.0f - life_ratio;
	particle->color = color_lerp(particle->start_color, 
		color_create(particle->start_color.r, particle->start_color.g, 
		particle->start_color.b, 0), alpha_progress);
}

static void	particle_render(t_particle *particle, t_game *game)
{
	if (!validate_pointer(particle) || !validate_pointer(game))
		return ;
	if (!particle->active || particle->size <= 0.1f)
		return ;
	if (particle->color.a <= 5)
		return ;
	render_circle(game, particle->pos, particle->size, particle->color);
}

static void	create_particle(t_game *game, t_vector2 pos, t_vector2 vel,
		int type, t_color color, float lifetime)
{
	int			particle_id;
	t_particle	*particle;

	particle_id = effects_find_free_particle(game);
	if (particle_id == -1)
		return ;
	particle = &game->particles[particle_id];
	particle->pos = pos;
	particle->vel = vel;
	particle->life = lifetime;
	particle->max_life = lifetime;
	particle->size = random_float(PARTICLE_SIZE_MIN, PARTICLE_SIZE_MAX);
	particle->start_size = particle->size;
	particle->color = color;
	particle->start_color = color;
	particle->type = type;
	particle->gravity = 20.0f;
	particle->active = true;
}

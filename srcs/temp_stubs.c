/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_stubs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 20:03:43 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

/* ========================================================================== */
/* PHASE 3 - IMPLEMENTED FUNCTIONS (No longer stubs)                         */
/* ========================================================================== */

/* Entity functions - Phase 3 - ACTIVE */
void	entities_update_all(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	
	/* Update all active enemies */
	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].base.active)
			enemy_update(&game->enemies[i], game);
		i++;
	}
	
	/* Update all active towers */
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
			tower_update(&game->towers[i], game);
		i++;
	}
	
	/* Update all active projectiles */
	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (game->projectiles[i].base.active)
			projectile_update(&game->projectiles[i], game);
		i++;
	}
	
	/* Update spawner system */
	spawner_update(game);
}

void	entities_render_all(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	
	/* Render all active enemies */
	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].base.active)
			enemy_render(&game->enemies[i], game);
		i++;
	}
	
	/* Render all active towers */
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
			tower_render(&game->towers[i], game);
		i++;
	}
	
	/* Render all active projectiles */
	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (game->projectiles[i].base.active)
			projectile_render(&game->projectiles[i], game);
		i++;
	}
}

/* Entity management functions */
int	entities_find_free_enemy(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return (-1);
	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (!game->enemies[i].base.active)
			return (i);
		i++;
	}
	return (-1);
}

int	entities_count_active_enemies(t_game *game)
{
	int	count;
	int	i;

	if (!validate_pointer(game))
		return (0);
	count = 0;
	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].base.active)
			count++;
		i++;
	}
	return (count);
}

int	entities_count_active_towers(t_game *game)
{
	int	count;
	int	i;

	if (!validate_pointer(game))
		return (0);
	count = 0;
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
			count++;
		i++;
	}
	return (count);
}

int	entities_find_free_tower(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return (-1);
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (!game->towers[i].base.active)
			return (i);
		i++;
	}
	return (-1);
}

int	entities_find_free_projectile(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return (-1);
	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (!game->projectiles[i].base.active)
			return (i);
		i++;
	}
	return (-1);
}

/* Base entity functions */
void	entity_init(t_entity *entity)
{
	if (!validate_pointer(entity))
		return ;
	entity->pos = vec2_zero();
	entity->vel = vec2_zero();
	entity->radius = 10.0f;
	entity->health = 100;
	entity->max_health = 100;
	entity->type = ENTITY_NONE;
	entity->active = false;
	entity->rotation = 0.0f;
}

/* Tower placement and management - NOW IMPLEMENTED IN TOWER.C */
bool	spawn_tower(t_game *game, t_vector2 pos, int type)
{
	int			tower_id;
	int			tower_cost;
	int			grid_x;
	int			grid_y;

	if (!validate_pointer(game))
		return (false);
	if (!game_is_position_valid(game, pos))
		return (false);
	if (type == TOWER_BASIC)
		tower_cost = TOWER_BASIC_COST;
	else if (type == TOWER_SNIPER)
		tower_cost = TOWER_SNIPER_COST;
	else if (type == TOWER_CANNON)
		tower_cost = TOWER_CANNON_COST;
	else if (type == TOWER_FREEZE)
		tower_cost = TOWER_FREEZE_COST;
	else
		return (false);
	if (game->money < tower_cost)
		return (false);
	tower_id = entities_find_free_tower(game);
	if (tower_id == -1)
		return (false);
	tower_create(&game->towers[tower_id], pos, type);
	game_spend_money(game, tower_cost);
	grid_x = world_to_grid_x(pos.x);
	grid_y = world_to_grid_y(pos.y);
	if (is_valid_grid_pos(grid_x, grid_y))
	{
		game->grid[grid_y][grid_x].occupied = true;
		game->grid[grid_y][grid_x].tower_id = tower_id;
	}
	debug_info("Tower placed: type=%d, cost=%d", type, tower_cost);
	return (true);
}

t_tower	*get_tower_at_position(t_game *game, t_vector2 pos)
{
	int		i;
	float	distance;

	if (!validate_pointer(game))
		return (NULL);
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].base.active)
		{
			distance = math_distance(game->towers[i].base.pos, pos);
			if (distance <= game->towers[i].base.radius)
				return (&game->towers[i]);
		}
		i++;
	}
	return (NULL);
}

void	select_tower(t_game *game, int tower_id)
{
	if (!validate_pointer(game) || tower_id < 0 || tower_id >= MAX_TOWERS)
		return ;
	deselect_all_towers(game);
	if (game->towers[tower_id].base.active)
	{
		game->towers[tower_id].selected = true;
		game->selected_tower_id = tower_id;
	}
}

void	deselect_all_towers(t_game *game)
{
	int	i;

	if (!validate_pointer(game))
		return ;
	i = 0;
	while (i < MAX_TOWERS)
	{
		game->towers[i].selected = false;
		i++;
	}
	game->selected_tower_id = -1;
}

/* ========================================================================== */
/* EFFECTS SYSTEM STUBS - For stable build compatibility                     */
/* ========================================================================== */

void	effects_create_explosion(t_game *game, t_vector2 pos, int intensity)
{
	(void)game;
	(void)pos;
	(void)intensity;
	/* Stub - no visual effects in stable build */
}

void	effects_create_blood(t_game *game, t_vector2 pos)
{
	(void)game;
	(void)pos;
	/* Stub - no visual effects in stable build */
}

void	effects_create_smoke(t_game *game, t_vector2 pos)
{
	(void)game;
	(void)pos;
	/* Stub - no visual effects in stable build */
}

void	effects_create_sparks(t_game *game, t_vector2 pos, int count)
{
	(void)game;
	(void)pos;
	(void)count;
	/* Stub - no visual effects in stable build */
}

void	effects_create_freeze_aura(t_game *game, t_vector2 pos, float radius)
{
	(void)game;
	(void)pos;
	(void)radius;
	/* Stub - no visual effects in stable build */
}

/* ========================================================================== */
/* TEMPORARY STUBS FOR NOT YET IMPLEMENTED FUNCTIONS (Phase 4)               */
/* ========================================================================== */

/* Wave system functions - Using spawner system */
void	waves_update(t_wave *wave, t_game *game)
{
	/* Wave updates are now handled by spawner_update() */
	(void)wave;
	(void)game;
}

/* Effects system functions */
void	effects_update(t_game *game)
{
	(void)game;
	/* TODO: Implement effects updates in Phase 4 */
}

void	effects_render(t_game *game)
{
	(void)game;
	/* TODO: Implement effects rendering in Phase 4 */
}

/* Notification system functions */
void	notifications_update(t_game *game)
{
	(void)game;
	/* TODO: Implement notifications updates in Phase 4 */
}

void	notifications_render(t_game *game)
{
	(void)game;
	/* TODO: Implement notifications rendering in Phase 4 */
}

/* UI system functions */
void	ui_update(t_game *game)
{
	(void)game;
	/* TODO: Implement UI updates in Phase 4 */
}

void	ui_render(t_game *game)
{
	int		i;
	t_color	button_color;
	t_color	text_color;
	t_vector2	text_pos;

	if (!validate_pointer(game))
		return ;
	
	/* Simple tower button rendering */
	i = 0;
	while (i < 4)
	{
		button_color = game->tower_buttons[i].enabled ? 
			color_create(100, 100, 150, 255) : color_create(50, 50, 50, 255);
		text_color = color_white();
		render_rect(game, game->tower_buttons[i].rect, button_color);
		text_pos.x = game->tower_buttons[i].rect.x + 5;
		text_pos.y = game->tower_buttons[i].rect.y + 5;
		if (game->font_small)
			render_text(game, game->tower_buttons[i].text, text_pos, 
				text_color, game->font_small);
		i++;
	}
	
	/* Game stats display */
	if (game->show_debug)
	{
		char stats_text[256];
		sprintf(stats_text, "Wave: %d | Enemies: %d | Money: %d | Lives: %d", 
			game->current_wave.number, 
			entities_count_active_enemies(game),
			game->money, 
			game->lives);
		text_pos = vec2_create(10, GAME_AREA_HEIGHT + 50);
		if (game->font_small)
			render_text(game, stats_text, text_pos, color_white(), game->font_small);
	}
}
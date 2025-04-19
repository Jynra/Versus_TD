/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 16:05:42 by student           #+#    #+#             */
/*   Updated: 2025/04/19 02:12:06 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/game.h"

/* Prototypes des fonctions auxiliaires */
void	set_tower_properties(t_game *game, t_tower *tower, int grid_x, int grid_y,
			int tower_cost);
void	shift_targets(int *target_indices, float *target_distances,
			int insert_index, int targets_found);
void	shift_targets_insert(int *target_indices, float *target_distances,
			int insert_index, int targets_to_find);
void	find_targets(t_game *game, t_tower *tower, int targets_to_find,
			int *target_indices);
void	fire_at_target(t_game *game, t_tower *tower, int target_index);

void	spawn_enemy(t_game *game)
{
	int	i;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (!game->enemies[i].active)
		{
			game->enemies[i].active = true;
			game->enemies[i].x = game->path[0].x;
			game->enemies[i].y = game->path[0].y;
			game->enemies[i].width = 30;
			game->enemies[i].height = 30;
			game->enemies[i].base_speed = ENEMY_BASIC_SPEED;
			game->enemies[i].speed = ENEMY_BASIC_SPEED;
			game->enemies[i].health = ENEMY_BASIC_HEALTH;
			game->enemies[i].max_health = ENEMY_BASIC_HEALTH;
			game->enemies[i].path_index = 0;
			game->enemies[i].slowed = false;
			game->enemies[i].slow_timer = 0.0f;
			break ;
		}
		i++;
	}
}

void	set_tower_properties(t_game *game, t_tower *tower, int grid_x, int grid_y,
		int tower_cost)
{
	tower->active = true;
	tower->x = grid_x * GRID_SIZE + GRID_SIZE / 2;
	tower->y = grid_y * GRID_SIZE + GRID_SIZE / 2;
	tower->width = 30;
	tower->height = 30;
	tower->cost = tower_cost;
	if (game->selected_tower_type == TOWER_TYPE_BASIC)
	{
		tower->damage = TOWER_BASIC_DAMAGE;
		tower->range = TOWER_BASIC_RANGE;
		tower->fire_rate = TOWER_BASIC_RATE;
	}
	else if (game->selected_tower_type == TOWER_TYPE_SLOW)
	{
		tower->damage = TOWER_SLOW_DAMAGE;
		tower->range = TOWER_SLOW_RANGE;
		tower->fire_rate = TOWER_SLOW_RATE;
	}
	else if (game->selected_tower_type == TOWER_TYPE_MULTI)
	{
		tower->damage = TOWER_MULTI_DAMAGE;
		tower->range = TOWER_MULTI_RANGE;
		tower->fire_rate = TOWER_MULTI_RATE;
	}
	tower->fire_timer = 0.0f;
}

void	place_tower(t_game *game, int grid_x, int grid_y)
{
	int	tower_cost;
	int	i;

	if (grid_x < 0 || grid_x >= GRID_COLS || grid_y < 0 || grid_y >= GRID_ROWS)
		return ;
	if (game->grid[grid_y][grid_x] != TERRAIN_BUILDABLE)
		return ;
	tower_cost = 0;
	if (game->selected_tower_type == TOWER_TYPE_BASIC)
		tower_cost = COST_TOWER_BASIC;
	else if (game->selected_tower_type == TOWER_TYPE_SLOW)
		tower_cost = COST_TOWER_SLOW;
	else if (game->selected_tower_type == TOWER_TYPE_MULTI)
		tower_cost = COST_TOWER_MULTI;
	else
		return ;
	if (game->money < tower_cost)
		return ;
	i = 0;
	while (i < MAX_TOWERS)
	{
		if (!game->towers[i].active)
		{
			set_tower_properties(game, &game->towers[i], grid_x, grid_y, tower_cost);
			game->money -= tower_cost;
			game->grid[grid_y][grid_x] = TERRAIN_OCCUPIED;
			break ;
		}
		i++;
	}
}

void	shift_targets(int *target_indices, float *target_distances,
		int insert_index, int targets_found)
{
	int	k;

	k = targets_found - 1;
	while (k >= insert_index)
	{
		target_indices[k + 1] = target_indices[k];
		target_distances[k + 1] = target_distances[k];
		k--;
	}
}

void	shift_targets_insert(int *target_indices, float *target_distances,
		int insert_index, int targets_to_find)
{
	int	k;

	k = targets_to_find - 1;
	while (k > insert_index)
	{
		target_indices[k] = target_indices[k - 1];
		target_distances[k] = target_distances[k - 1];
		k--;
	}
}

void	find_targets(t_game *game, t_tower *tower, int targets_to_find,
		int *target_indices)
{
	int		i;
	int		j;
	int		insert_index;
	float	dist;
	float	target_distances[TOWER_MULTI_TARGETS];
	int		targets_found;

	targets_found = 0;
	i = 0;
	while (i < TOWER_MULTI_TARGETS)
	{
		target_indices[i] = -1;
		target_distances[i] = 0;
		i++;
	}
	j = 0;
	while (j < MAX_ENEMIES)
	{
		if (game->enemies[j].active)
		{
			dist = distance(tower->x, tower->y,
					game->enemies[j].x, game->enemies[j].y);
			if (dist < tower->range)
			{
				insert_index = targets_found;
				while (insert_index > 0 && dist < target_distances[insert_index - 1])
					insert_index--;
				if (insert_index < targets_found && targets_found < targets_to_find)
					shift_targets(target_indices, target_distances, insert_index,
							targets_found);
				if (targets_found < targets_to_find)
				{
					target_indices[insert_index] = j;
					target_distances[insert_index] = dist;
					targets_found++;
				}
				else if (insert_index < targets_to_find)
				{
					shift_targets_insert(target_indices, target_distances,
							insert_index, targets_to_find);
					target_indices[insert_index] = j;
					target_distances[insert_index] = dist;
				}
			}
		}
		j++;
	}
}

void	fire_at_target(t_game *game, t_tower *tower, int target_index)
{
	int		j;
	float	dx;
	float	dy;
	float	length;

	j = 0;
	while (j < MAX_PROJECTILES)
	{
		if (!game->projectiles[j].active)
		{
			game->projectiles[j].active = true;
			game->projectiles[j].x = tower->x;
			game->projectiles[j].y = tower->y;
			game->projectiles[j].damage = tower->damage;
			game->projectiles[j].speed = PROJECTILE_SPEED;
			dx = game->enemies[target_index].x - tower->x;
			dy = game->enemies[target_index].y - tower->y;
			length = sqrt(dx * dx + dy * dy);
			game->projectiles[j].dx = dx / length * game->projectiles[j].speed;
			game->projectiles[j].dy = dy / length * game->projectiles[j].speed;
			break ;
		}
		j++;
	}
}

void	update_towers(t_game *game)
{
	int	i;
	int	targets_to_find;
	int	target_indices[TOWER_MULTI_TARGETS];
	int	t;
	int	targets_found;

	i = 0;
	while (i < MAX_TOWERS)
	{
		if (game->towers[i].active)
		{
			game->towers[i].fire_timer += 1.0f / FPS;
			if (game->towers[i].fire_timer >= 1.0f / game->towers[i].fire_rate)
			{
				targets_to_find = 1;
				if (game->towers[i].damage == TOWER_MULTI_DAMAGE)
					targets_to_find = TOWER_MULTI_TARGETS;
				find_targets(game, &game->towers[i], targets_to_find, target_indices);
				targets_found = 0;
				t = 0;
				while (t < targets_to_find && target_indices[t] != -1)
				{
					fire_at_target(game, &game->towers[i], target_indices[t]);
					targets_found++;
					t++;
				}
				if (targets_found > 0)
					game->towers[i].fire_timer = 0.0f;
			}
		}
		i++;
	}
}

void	update_enemies(t_game *game)
{
	int		i;
	float	target_x;
	float	target_y;
	float	dx;
	float	dy;
	float	length;

	i = 0;
	while (i < MAX_ENEMIES)
	{
		if (game->enemies[i].active)
		{
			if (game->enemies[i].slowed)
			{
				game->enemies[i].slow_timer -= 1.0f / FPS;
				if (game->enemies[i].slow_timer <= 0)
				{
					game->enemies[i].slowed = false;
					game->enemies[i].speed = game->enemies[i].base_speed;
				}
			}
			target_x = game->path[game->enemies[i].path_index + 1].x;
			target_y = game->path[game->enemies[i].path_index + 1].y;
			dx = target_x - game->enemies[i].x;
			dy = target_y - game->enemies[i].y;
			length = sqrt(dx * dx + dy * dy);
			if (length < game->enemies[i].speed)
			{
				game->enemies[i].path_index++;
				if (game->enemies[i].path_index >= game->path_length - 1)
				{
					game->enemies[i].active = false;
					game->lives--;
					if (game->lives <= 0)
						printf("Game Over!\n");
				}
			}
			else
			{
				game->enemies[i].x += dx / length * game->enemies[i].speed;
				game->enemies[i].y += dy / length * game->enemies[i].speed;
			}
			if (game->enemies[i].health <= 0)
			{
				game->enemies[i].active = false;
				game->money += ENEMY_REWARD;
				game->score += ENEMY_SCORE;
			}
		}
		i++;
	}
}

void	update_projectiles(t_game *game)
{
	int		i;
	int		j;
	float	dist;

	i = 0;
	while (i < MAX_PROJECTILES)
	{
		if (game->projectiles[i].active)
		{
			game->projectiles[i].x += game->projectiles[i].dx;
			game->projectiles[i].y += game->projectiles[i].dy;
			if (game->projectiles[i].x < 0 || game->projectiles[i].x > WINDOW_WIDTH
				|| game->projectiles[i].y < 0
				|| game->projectiles[i].y > WINDOW_HEIGHT)
			{
				game->projectiles[i].active = false;
				i++;
				continue ;
			}
			j = 0;
			while (j < MAX_ENEMIES)
			{
				if (game->enemies[j].active)
				{
					dist = distance(
							game->projectiles[i].x, game->projectiles[i].y,
							game->enemies[j].x, game->enemies[j].y);
					if (dist < game->enemies[j].width / 2)
					{
						game->enemies[j].health -= game->projectiles[i].damage;
						if (game->projectiles[i].damage == TOWER_SLOW_DAMAGE)
						{
							game->enemies[j].slowed = true;
							game->enemies[j].slow_timer = TOWER_SLOW_DURATION;
							game->enemies[j].speed = game->enemies[j].base_speed
								* TOWER_SLOW_EFFECT;
						}
						game->projectiles[i].active = false;
						break ;
					}
				}
				j++;
			}
		}
		i++;
	}
}
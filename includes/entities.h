/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entities.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 17:42:11 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENTITIES_H
# define ENTITIES_H

# include "types.h"

/* Base Entity Functions */
void	entity_init(t_entity *entity);
void	entity_update(t_entity *entity, t_game *game);
bool	entity_is_alive(t_entity *entity);
float	entity_distance(t_entity *a, t_entity *b);
bool	entity_collision(t_entity *a, t_entity *b);
void	entity_take_damage(t_entity *entity, int damage);
void	entity_heal(t_entity *entity, int amount);

/* Tower Functions */
void	tower_init(t_tower *tower);
void	tower_create(t_tower *tower, t_vector2 pos, int type);
void	tower_update(t_tower *tower, t_game *game);
void	tower_render(t_tower *tower, t_game *game);
void	tower_fire(t_tower *tower, t_game *game);
void	tower_upgrade(t_tower *tower);
bool	tower_can_upgrade(t_tower *tower, t_game *game);
int		tower_get_upgrade_cost(t_tower *tower);
void	tower_set_stats(t_tower *tower, int type);
t_enemy	*tower_find_target(t_tower *tower, t_game *game);
bool	tower_in_range(t_tower *tower, t_enemy *enemy);
void	tower_destroy(t_tower *tower);

/* Enemy Functions */
void	enemy_init(t_enemy *enemy);
void	enemy_create(t_enemy *enemy, int type);
void	enemy_update(t_enemy *enemy, t_game *game);
void	enemy_render(t_enemy *enemy, t_game *game);
void	enemy_move(t_enemy *enemy, t_game *game);
void	enemy_take_damage(t_enemy *enemy, int damage, t_game *game);
void	enemy_apply_slow(t_enemy *enemy, float duration, float factor);
void	enemy_set_stats(t_enemy *enemy, int type);
bool	enemy_reached_end(t_enemy *enemy, t_game *game);
void	enemy_destroy(t_enemy *enemy, t_game *game);
t_vector2	enemy_get_next_waypoint(t_enemy *enemy, t_game *game);

/* Projectile Functions */
void	projectile_init(t_projectile *projectile);
void	projectile_create(t_projectile *proj, t_vector2 pos, t_vector2 target,
			int damage);
void	projectile_update(t_projectile *projectile, t_game *game);
void	projectile_render(t_projectile *projectile, t_game *game);
bool	projectile_hit_target(t_projectile *projectile, t_enemy *enemy);
void	projectile_explode(t_projectile *projectile, t_game *game);
void	projectile_destroy(t_projectile *projectile);

/* Spawner Functions - Phase 3 NEW */
void	spawner_update(t_game *game);
void	spawner_force_next_wave(t_game *game);
int		spawner_get_enemies_remaining(t_game *game);

/* Entity Management Functions */
int		entities_find_free_tower(t_game *game);
int		entities_find_free_enemy(t_game *game);
int		entities_find_free_projectile(t_game *game);
void	entities_update_all(t_game *game);
void	entities_render_all(t_game *game);
void	entities_cleanup_inactive(t_game *game);
int		entities_count_active_enemies(t_game *game);
int		entities_count_active_towers(t_game *game);

/* Entity Spawning Functions */
bool	spawn_tower(t_game *game, t_vector2 pos, int type);
bool	spawn_enemy(t_game *game, int type);
bool	spawn_projectile(t_game *game, t_vector2 pos, t_vector2 target,
			int damage);

/* Entity Selection and Interaction */
t_tower	*get_tower_at_position(t_game *game, t_vector2 pos);
t_enemy	*get_nearest_enemy(t_game *game, t_vector2 pos, float max_range);
void	select_tower(t_game *game, int tower_id);
void	deselect_all_towers(t_game *game);

#endif
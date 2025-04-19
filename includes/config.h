/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 01:35:35 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/19 01:35:36 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

/* Window and performance constants */
# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 680
# define FPS 60
# define FRAME_DELAY (1000 / FPS)

/* Interface constants */
# define TOOLBAR_HEIGHT 80
# define GAME_AREA_HEIGHT (WINDOW_HEIGHT - TOOLBAR_HEIGHT)
# define TOWER_BUTTON_SIZE 60
# define TOWER_BUTTON_PADDING 10
# define MAX_TOWER_TYPES 3

/* Entity limits */
# define MAX_TOWERS 50
# define MAX_ENEMIES 100
# define MAX_PROJECTILES 200

/* Grid constants */
# define GRID_SIZE 40
# define GRID_COLS (WINDOW_WIDTH / GRID_SIZE)
# define GRID_ROWS (GAME_AREA_HEIGHT / GRID_SIZE)

/* Tower types */
# define TOWER_TYPE_BASIC 1
# define TOWER_TYPE_SLOW 2
# define TOWER_TYPE_MULTI 3

/* Terrain types */
# define TERRAIN_PATH 0
# define TERRAIN_BUILDABLE 1
# define TERRAIN_OCCUPIED 2

/* Tower costs */
# define COST_TOWER_BASIC 20
# define COST_TOWER_SLOW 40
# define COST_TOWER_MULTI 60

/* Tower stats - basic tower */
# define TOWER_BASIC_DAMAGE 20
# define TOWER_BASIC_RANGE 100.0f
# define TOWER_BASIC_RATE 1.0f

/* Tower stats - slow tower */
# define TOWER_SLOW_DAMAGE 10
# define TOWER_SLOW_RANGE 120.0f
# define TOWER_SLOW_RATE 0.8f
# define TOWER_SLOW_EFFECT 0.5f
# define TOWER_SLOW_DURATION 3.0f

/* Tower stats - multi tower */
# define TOWER_MULTI_DAMAGE 15
# define TOWER_MULTI_RANGE 150.0f
# define TOWER_MULTI_RATE 0.7f
# define TOWER_MULTI_TARGETS 2

/* Enemy stats */
# define ENEMY_BASIC_HEALTH 100
# define ENEMY_BASIC_SPEED 1.0f
# define ENEMY_REWARD 10
# define ENEMY_SCORE 100

/* Projectile stats */
# define PROJECTILE_SPEED 5.0f

/* Game parameters */
# define STARTING_MONEY 100
# define STARTING_LIVES 20

#endif
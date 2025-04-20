/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 11:12:22 by ellucas           #+#    #+#             */
/*   Updated: 2025/04/20 13:39:07 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

/* Window and performance constants */
# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600
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
# define MAX_PATH_POINTS 20

/* Grid constants */
# define GRID_SIZE 40
# define GRID_COLS (WINDOW_WIDTH / GRID_SIZE)
# define GRID_ROWS (GAME_AREA_HEIGHT / GRID_SIZE)

/* Tower types */
# define TOWER_TYPE_BASIC 1
# define TOWER_TYPE_SLOW 2
# define TOWER_TYPE_MULTI 3

/* Enemy types */
# define ENEMY_TYPE_BASIC 1
# define ENEMY_TYPE_FAST 2
# define ENEMY_TYPE_BOSS 3

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

/* Enemy stats - basic enemy */
# define ENEMY_BASIC_HEALTH 100
# define ENEMY_BASIC_SPEED 1.0f
# define ENEMY_BASIC_REWARD 10
# define ENEMY_BASIC_SCORE 100

/* Enemy stats - fast enemy */
# define ENEMY_FAST_HEALTH 60
# define ENEMY_FAST_SPEED 1.8f
# define ENEMY_FAST_REWARD 15
# define ENEMY_FAST_SCORE 150

/* Enemy stats - boss enemy */
# define ENEMY_BOSS_HEALTH 300
# define ENEMY_BOSS_SPEED 0.6f
# define ENEMY_BOSS_REWARD 50
# define ENEMY_BOSS_SCORE 500

/* Projectile stats */
# define PROJECTILE_SPEED 5.0f
# define PROJECTILE_BASIC_SIZE 4
# define PROJECTILE_SLOW_SIZE 5
# define PROJECTILE_MULTI_SIZE 3

/* Game parameters */
# define STARTING_MONEY 100
# define STARTING_LIVES 20
# define WAVE_DELAY 10.0f  /* Temps entre chaque vague en secondes */

/* Resource paths */
# define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
# define TEXTURE_COUNT 11
# define FONT_COUNT 3
# define TEXTURES_PATH "assets/textures/"
# define SOUNDS_PATH "assets/sounds/"

/* Debug mode */
# define DEBUG_MODE 0

#endif
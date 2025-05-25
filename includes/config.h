/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 12:28:03 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

/* Window and Display Settings */
# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 768
# define WINDOW_TITLE "Versus TD - Clean Edition"
# define FPS 60
# define FRAME_DELAY (1000 / FPS)

/* Game Area Layout */
# define TOOLBAR_HEIGHT 80
# define GAME_AREA_WIDTH WINDOW_WIDTH
# define GAME_AREA_HEIGHT (WINDOW_HEIGHT - TOOLBAR_HEIGHT)

/* Grid System */
# define GRID_SIZE 32
# define GRID_COLS (GAME_AREA_WIDTH / GRID_SIZE)
# define GRID_ROWS (GAME_AREA_HEIGHT / GRID_SIZE)

/* Entity Limits */
# define MAX_TOWERS 50
# define MAX_ENEMIES 100
# define MAX_PROJECTILES 200
# define MAX_PARTICLES 300
# define MAX_NOTIFICATIONS 8
# define MAX_PATH_POINTS 20

/* Game States */
# define STATE_MENU 0
# define STATE_PLAYING 1
# define STATE_PAUSED 2
# define STATE_GAME_OVER 3
# define STATE_VICTORY 4

/* Entity Types */
# define ENTITY_NONE 0
# define ENTITY_TOWER 1
# define ENTITY_ENEMY 2
# define ENTITY_PROJECTILE 3

/* Tower Types */
# define TOWER_BASIC 1
# define TOWER_SNIPER 2
# define TOWER_CANNON 3
# define TOWER_FREEZE 4

/* Tower Stats - Basic */
# define TOWER_BASIC_DAMAGE 25
# define TOWER_BASIC_RANGE 80.0f
# define TOWER_BASIC_FIRE_RATE 1.5f
# define TOWER_BASIC_COST 20

/* Tower Stats - Sniper */
# define TOWER_SNIPER_DAMAGE 80
# define TOWER_SNIPER_RANGE 150.0f
# define TOWER_SNIPER_FIRE_RATE 0.8f
# define TOWER_SNIPER_COST 50

/* Tower Stats - Cannon */
# define TOWER_CANNON_DAMAGE 60
# define TOWER_CANNON_RANGE 100.0f
# define TOWER_CANNON_FIRE_RATE 0.6f
# define TOWER_CANNON_COST 80

/* Tower Stats - Freeze */
# define TOWER_FREEZE_DAMAGE 15
# define TOWER_FREEZE_RANGE 90.0f
# define TOWER_FREEZE_FIRE_RATE 1.2f
# define TOWER_FREEZE_COST 40
# define TOWER_FREEZE_DURATION 3.0f
# define TOWER_FREEZE_SLOW_FACTOR 0.3f

/* Enemy Types */
# define ENEMY_BASIC 1
# define ENEMY_FAST 2
# define ENEMY_TANK 3
# define ENEMY_FLYING 4

/* Enemy Stats - Basic */
# define ENEMY_BASIC_HEALTH 100
# define ENEMY_BASIC_SPEED 30.0f
# define ENEMY_BASIC_REWARD 10
# define ENEMY_BASIC_SCORE 50

/* Enemy Stats - Fast */
# define ENEMY_FAST_HEALTH 60
# define ENEMY_FAST_SPEED 60.0f
# define ENEMY_FAST_REWARD 15
# define ENEMY_FAST_SCORE 75

/* Enemy Stats - Tank */
# define ENEMY_TANK_HEALTH 300
# define ENEMY_TANK_SPEED 15.0f
# define ENEMY_TANK_REWARD 30
# define ENEMY_TANK_SCORE 150

/* Enemy Stats - Flying */
# define ENEMY_FLYING_HEALTH 80
# define ENEMY_FLYING_SPEED 45.0f
# define ENEMY_FLYING_REWARD 20
# define ENEMY_FLYING_SCORE 100

/* Projectile Stats */
# define PROJECTILE_SPEED 200.0f
# define PROJECTILE_SIZE 4
# define PROJECTILE_LIFETIME 5.0f

/* Particle Types */
# define PARTICLE_EXPLOSION 1
# define PARTICLE_SMOKE 2
# define PARTICLE_SPARK 3
# define PARTICLE_BLOOD 4

/* Particle Settings */
# define PARTICLE_LIFETIME_MIN 0.5f
# define PARTICLE_LIFETIME_MAX 2.0f
# define PARTICLE_SPEED_MIN 20.0f
# define PARTICLE_SPEED_MAX 80.0f
# define PARTICLE_SIZE_MIN 2.0f
# define PARTICLE_SIZE_MAX 8.0f

/* Notification Types */
# define NOTIF_INFO 1
# define NOTIF_SUCCESS 2
# define NOTIF_WARNING 3
# define NOTIF_ERROR 4
# define NOTIF_DURATION 3.0f

/* Wave System */
# define MAX_WAVES 30
# define WAVE_PREP_TIME 5.0f
# define WAVE_BASE_ENEMIES 8
# define WAVE_ENEMY_INCREMENT 3
# define WAVE_SPAWN_DELAY_BASE 1.5f
# define WAVE_SPAWN_DELAY_MIN 0.3f

/* Game Balance */
# define STARTING_MONEY 100
# define STARTING_LIVES 20
# define TOWER_UPGRADE_COST_MULTIPLIER 1.5f
# define TOWER_MAX_LEVEL 3

/* Colors (RGB) */
# define COLOR_WHITE_R 255
# define COLOR_WHITE_G 255
# define COLOR_WHITE_B 255

# define COLOR_BLACK_R 0
# define COLOR_BLACK_G 0
# define COLOR_BLACK_B 0

# define COLOR_RED_R 255
# define COLOR_RED_G 50
# define COLOR_RED_B 50

# define COLOR_GREEN_R 50
# define COLOR_GREEN_G 255
# define COLOR_GREEN_B 50

# define COLOR_BLUE_R 50
# define COLOR_BLUE_G 50
# define COLOR_BLUE_B 255

# define COLOR_YELLOW_R 255
# define COLOR_YELLOW_G 255
# define COLOR_YELLOW_B 50

# define COLOR_GRAY_R 128
# define COLOR_GRAY_G 128
# define COLOR_GRAY_B 128

# define COLOR_DARK_GRAY_R 64
# define COLOR_DARK_GRAY_G 64
# define COLOR_DARK_GRAY_B 64

/* UI Layout */
# define UI_BUTTON_WIDTH 60
# define UI_BUTTON_HEIGHT 60
# define UI_BUTTON_PADDING 10
# define UI_TEXT_PADDING 5

/* Terrain Types */
# define TERRAIN_GRASS 0
# define TERRAIN_PATH 1
# define TERRAIN_BLOCKED 2

/* Input Settings */
# define MOUSE_BUTTON_LEFT 1
# define MOUSE_BUTTON_RIGHT 3

/* Debug Settings */
# define DEBUG_SHOW_RANGES 1
# define DEBUG_SHOW_PATHS 1
# define DEBUG_SHOW_GRID 0
# define DEBUG_SHOW_FPS 1

/* File Paths */
# define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
# define FONT_SIZE_SMALL 12
# define FONT_SIZE_MEDIUM 16
# define FONT_SIZE_LARGE 24

/* Math Constants */
# define PI 3.14159265359f
# define TWO_PI 6.28318530718f
# define HALF_PI 1.57079632679f

/* Utility Macros */
# define MIN(a, b) ((a) < (b) ? (a) : (b))
# define MAX(a, b) ((a) > (b) ? (a) : (b))
# define CLAMP(x, min, max) (MIN(MAX(x, min), max))
# define ABS(x) ((x) < 0 ? -(x) : (x))

#endif
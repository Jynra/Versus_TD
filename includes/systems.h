/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   systems.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellucas <ellucas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:00:00 by jynra             #+#    #+#             */
/*   Updated: 2025/05/25 18:11:41 by ellucas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSTEMS_H
# define SYSTEMS_H

# include "types.h"

/* Input System */
void	input_init(t_input *input);
void	input_update(t_input *input);
void	input_handle_event(t_input *input, SDL_Event *event, t_game *game);
bool	input_is_key_pressed(t_input *input, int scancode);
bool	input_is_key_down(t_input *input, int scancode);
bool	input_mouse_clicked(t_input *input, int button);
bool	input_mouse_down(t_input *input, int button);
t_vector2	input_get_mouse_pos(t_input *input);
t_vector2	input_screen_to_world(t_input *input, t_vector2 screen_pos);

/* Physics System */
void	physics_update(t_game *game);
bool	physics_circle_collision(t_vector2 pos1, float r1, t_vector2 pos2,
			float r2);
bool	physics_point_in_circle(t_vector2 point, t_vector2 center, float radius);
bool	physics_point_in_rect(t_vector2 point, t_rect rect);
float	physics_distance(t_vector2 a, t_vector2 b);
t_vector2	physics_normalize(t_vector2 vec);
t_vector2	physics_rotate(t_vector2 vec, float angle);

/* Wave System */
void	waves_init(t_wave *wave);
void	waves_update(t_wave *wave, t_game *game);
void	waves_start_next(t_wave *wave, t_game *game);
void	waves_spawn_enemy(t_wave *wave, t_game *game);
bool	waves_is_complete(t_wave *wave, t_game *game);
bool	waves_all_enemies_dead(t_game *game);
void	waves_calculate_spawn_data(t_wave *wave);
int		waves_get_enemy_type_for_wave(int wave_number, int spawn_index);

/* Effects System */
void	effects_init(t_game *game);
void	effects_update(t_game *game);
void	effects_render(t_game *game);
void	effects_create_explosion(t_game *game, t_vector2 pos, int intensity);
void	effects_create_blood(t_game *game, t_vector2 pos);
void	effects_create_smoke(t_game *game, t_vector2 pos);
void	effects_create_sparks(t_game *game, t_vector2 pos, int count);
int		effects_find_free_particle(t_game *game);
void	effects_cleanup_particles(t_game *game);

/* UI System */
void	ui_init(t_game *game);
void	ui_update(t_game *game);
void	ui_render(t_game *game);
void	ui_render_hud(t_game *game);
void	ui_render_tower_buttons(t_game *game);
void	ui_render_tower_info(t_game *game);
void	ui_render_wave_info(t_game *game);
void	ui_handle_click(t_game *game, t_vector2 pos);
bool	ui_button_clicked(t_button *button, t_vector2 pos);
void	ui_button_update(t_button *button, t_vector2 mouse_pos);

/* Notification System */
void	notifications_init(t_game *game);
void	notifications_update(t_game *game);
void	notifications_render(t_game *game);
void	notifications_add(t_game *game, const char *text, int type);
int		notifications_find_free(t_game *game);
void	notifications_cleanup(t_game *game);

/* Upgrade System */
void	upgrades_init(t_game *game);
bool	upgrades_tower(t_game *game, int tower_id);
int		upgrades_get_cost(t_tower *tower);
void	upgrades_apply_stats(t_tower *tower);
bool	upgrades_can_afford(t_game *game, t_tower *tower);

/* Render System */
void	render_init(t_game *game);
void	render_clear(t_game *game);
void	render_present(t_game *game);
void	render_grid(t_game *game);
void	render_path(t_game *game);
void	render_ranges(t_game *game);
void	render_circle(t_game *game, t_vector2 center, float radius, t_color color);
void	render_rect(t_game *game, t_rect rect, t_color color);
void	render_line(t_game *game, t_vector2 start, t_vector2 end, t_color color);
void	render_text(t_game *game, const char *text, t_vector2 pos, t_color color,
			TTF_Font *font);

#endif
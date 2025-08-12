/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:50:44 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 13:21:05 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           KEYBOARD INPUT HANDLING                         */
/* ************************************************************************** */

/**
 * @brief Process keyboard input events and execute corresponding actions
 * 
 * This function handles all keyboard input for game control:
 * - Movement: WASD keys for forward/backward/strafe movement
 * - Rotation: Arrow keys for looking left/right
 * - System: ESC key for clean program exit
 * 
 * Movement calculations:
 * - W/S: Move along player direction vector (forward/backward)
 * - A/D: Move along camera plane vector (strafe left/right)
 * 
 * Rotation behavior varies by player orientation to maintain intuitive controls.
 * 
 * @param keycode The key code of the pressed key
 * @param game_map Pointer to the main game structure
 * @return 0 (required for MLX hook compatibility)
 */
int	handle_keyboard_input(int keycode, t_game_map *game_map)
{
	if (keycode == KEY_ESC)
		clean_exit_program(game_map);
	else if (keycode == KEY_W)
		move_player_with_collision(game_map, 
			game_map->player_dir_x * game_map->movement_speed,
			game_map->player_dir_y * game_map->movement_speed, '+');
	else if (keycode == KEY_S)
		move_player_with_collision(game_map,
			game_map->player_dir_x * game_map->movement_speed,
			game_map->player_dir_y * game_map->movement_speed, '-');
	else if (keycode == KEY_A)
		move_player_with_collision(game_map,
			game_map->camera_plane_x * game_map->movement_speed,
			game_map->camera_plane_y * game_map->movement_speed, '-');
	else if (keycode == KEY_D)
		move_player_with_collision(game_map,
			game_map->camera_plane_x * game_map->movement_speed,
			game_map->camera_plane_y * game_map->movement_speed, '+');
	else if ((keycode == KEY_LEFT && (game_map->player_direction == 'N' || game_map->player_direction == 'S')) ||
			 (keycode == KEY_RIGHT && (game_map->player_direction == 'E' || game_map->player_direction == 'W')))
		rotate_player_view(game_map, -game_map->rotation_speed);
	else if ((keycode == KEY_LEFT && (game_map->player_direction == 'E' || game_map->player_direction == 'W')) ||
			 (keycode == KEY_RIGHT && (game_map->player_direction == 'N' || game_map->player_direction == 'S')))
		rotate_player_view(game_map, game_map->rotation_speed);
	return (0);
}

/* ************************************************************************** */
/*                           MOUSE INPUT HANDLING                            */
/* ************************************************************************** */

/**
 * @brief Handle mouse movement for view rotation (bonus feature)
 * 
 * This function implements mouse-look functionality by detecting when
 * the mouse cursor is near the window edges and rotating the view accordingly.
 * This provides intuitive FPS-style controls.
 * 
 * Mouse rotation zones:
 * - Right edge: Rotate right (clockwise)
 * - Left edge: Rotate left (counter-clockwise)
 * - Center area: No rotation
 * 
 * Rotation speed is reduced compared to keyboard rotation for smoother control.
 * The behavior is adjusted based on player orientation to maintain consistency.
 * 
 * @param mouse_x X coordinate of mouse cursor
 * @param mouse_y Y coordinate of mouse cursor (unused but required by MLX)
 * @param game_map Pointer to the main game structure
 * @return 0 (required for MLX hook compatibility)
 */
int	handle_mouse_rotation(int mouse_x, int mouse_y, t_game_map *game_map)
{
	double	mouse_rotation_speed;

	(void)mouse_y;	
	mouse_rotation_speed = game_map->rotation_speed / 3.0;
	if (game_map->player_direction == 'N' || game_map->player_direction == 'S')
	{
		if (mouse_x > (int)(game_map->display_width / 1.2))
			rotate_player_view(game_map, mouse_rotation_speed);
		else if (mouse_x < game_map->display_width / 6)
			rotate_player_view(game_map, -mouse_rotation_speed);
	}
	else
	{
		if (mouse_x > (int)(game_map->display_width / 1.2))
			rotate_player_view(game_map, -mouse_rotation_speed);
		else if (mouse_x < game_map->display_width / 6)
			rotate_player_view(game_map, mouse_rotation_speed);
	}
	return (0);
}

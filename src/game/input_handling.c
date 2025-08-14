/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:50:44 by gansari           #+#    #+#             */
/*   Updated: 2025/08/14 15:25:50 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	handle_key_press(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		clean_exit_program(game);
	else if (keycode == KEY_W)
		game->player.key_w = 1;
	else if (keycode == KEY_S)
		game->player.key_s = 1;
	else if (keycode == KEY_A)
		game->player.key_a = 1;
	else if (keycode == KEY_D)
		game->player.key_d = 1;
	else if (keycode == KEY_LEFT)
		game->player.key_left = 1;
	else if (keycode == KEY_RIGHT)
		game->player.key_right = 1;
	return (0);
}

/**
 * @brief Handle key release events
 */
int	handle_key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->player.key_w = 0;
	else if (keycode == KEY_S)
		game->player.key_s = 0;
	else if (keycode == KEY_A)
		game->player.key_a = 0;
	else if (keycode == KEY_D)
		game->player.key_d = 0;
	else if (keycode == KEY_LEFT)
		game->player.key_left = 0;
	else if (keycode == KEY_RIGHT)
		game->player.key_right = 0;
	return (0);
}

/**
 * @brief Process all active movement inputs each frame
 */
void	process_movement_input(t_game *game)
{
	double	move_x = 0.0;
	double	move_y = 0.0;
	
	// Forward/Backward movement (relative to player's view direction)
	if (game->player.key_w)
	{
		move_x += game->player.dir_x * game->player.move_speed;
		move_y += game->player.dir_y * game->player.move_speed;
	}
	if (game->player.key_s)
	{
		move_x -= game->player.dir_x * game->player.move_speed;
		move_y -= game->player.dir_y * game->player.move_speed;
	}
	
	// Strafe left/right (relative to player's view direction)
	if (game->player.key_a)
	{
		move_x -= game->player.plane_x * game->player.move_speed;
		move_y -= game->player.plane_y * game->player.move_speed;
	}
	if (game->player.key_d)
	{
		move_x += game->player.plane_x * game->player.move_speed;
		move_y += game->player.plane_y * game->player.move_speed;
	}
	
	// Apply movement with collision detection
	if (move_x != 0.0 || move_y != 0.0)
		move_player_with_collision(game, move_x, move_y);
	
	// Handle rotation
	if (game->player.key_left)
		rotate_player_view(game, -game->player.rotate_speed);
	if (game->player.key_right)
		rotate_player_view(game, game->player.rotate_speed);
}

int	handle_keyboard_input(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		clean_exit_program(game);
	else if (keycode == KEY_W)
		move_player_with_collision(game, 
			game->player.dir_x * game->player.move_speed,
			game->player.dir_y * game->player.move_speed);
	else if (keycode == KEY_S)
		move_player_with_collision(game,
			game->player.dir_x * game->player.move_speed,
			game->player.dir_y * game->player.move_speed);
	else if (keycode == KEY_A)
		move_player_with_collision(game,
			game->player.plane_x * game->player.move_speed,
			game->player.plane_y * game->player.move_speed);
	else if (keycode == KEY_D)
		move_player_with_collision(game,
			game->player.plane_x * game->player.move_speed,
			game->player.plane_y * game->player.move_speed);
	else if ((keycode == KEY_LEFT && (game->player.initial_dir == 'N' || game->player.initial_dir == 'S')) ||
			 (keycode == KEY_RIGHT && (game->player.initial_dir == 'E' || game->player.initial_dir == 'W')))
		rotate_player_view(game, -game->player.rotate_speed);
	else if ((keycode == KEY_LEFT && (game->player.initial_dir == 'E' || game->player.initial_dir == 'W')) ||
			 (keycode == KEY_RIGHT && (game->player.initial_dir == 'N' || game->player.initial_dir == 'S')))
		rotate_player_view(game, game->player.rotate_speed);
	return (0);
}

#ifdef BONUS
int	handle_mouse_rotation(int mouse_x, int mouse_y, t_game *game)
{
	double	mouse_rotation_speed;

	(void)mouse_y;	
	mouse_rotation_speed = game->player.rotate_speed / 3.0;
	
	if (game->player.initial_dir == 'N' || game->player.initial_dir == 'S')
	{
		if (mouse_x > (int)(game->mlx.width / 1.2))
			rotate_player_view(game, mouse_rotation_speed);
		else if (mouse_x < game->mlx.width / 6)
			rotate_player_view(game, -mouse_rotation_speed);
	}
	else
	{
		if (mouse_x > (int)(game->mlx.width / 1.2))
			rotate_player_view(game, -mouse_rotation_speed);
		else if (mouse_x < game->mlx.width / 6)
			rotate_player_view(game, mouse_rotation_speed);
	}
	return (0);
}
#endif

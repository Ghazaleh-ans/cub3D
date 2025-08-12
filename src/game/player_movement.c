/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 17:11:12 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           PLAYER MOVEMENT WITH COLLISION                  */
/* ************************************************************************** */

/**
 * @brief Move player with collision detection and minimap updates
 * 
 * This function handles player movement with comprehensive collision detection.
 * Independent axis checking allows sliding along walls.
 */
void	move_player_with_collision(t_game *game, double delta_x, 
		double delta_y, char direction_sign)
{
	int	previous_grid_x;
	int	previous_grid_y;
	int	new_grid_x;
	int	new_grid_y;

	previous_grid_x = (int)game->player.pos_x;
	previous_grid_y = (int)game->player.pos_y;
	
	if (direction_sign == '+')
	{
		new_grid_x = (int)(game->player.pos_x + delta_x);
		if (game->map.grid[(int)game->player.pos_y][new_grid_x] != '1')
			game->player.pos_x += delta_x;
			
		new_grid_y = (int)(game->player.pos_y + delta_y);
		if (game->map.grid[new_grid_y][(int)game->player.pos_x] != '1')
			game->player.pos_y += delta_y;
	}
	else if (direction_sign == '-')
	{
		new_grid_x = (int)(game->player.pos_x - delta_x);
		if (game->map.grid[(int)game->player.pos_y][new_grid_x] != '1')
			game->player.pos_x -= delta_x;
			
		new_grid_y = (int)(game->player.pos_y - delta_y);
		if (game->map.grid[new_grid_y][(int)game->player.pos_x] != '1')
			game->player.pos_y -= delta_y;
	}
	
	#ifdef BONUS
	if ((int)game->player.pos_x != previous_grid_x || 
		(int)game->player.pos_y != previous_grid_y)
	{
		update_minimap_player_position(game, previous_grid_x, previous_grid_y);
	}
	#else
	(void)previous_grid_x;
	(void)previous_grid_y;
	#endif
}

/* ************************************************************************** */
/*                           PLAYER VIEW ROTATION                            */
/* ************************************************************************** */

/**
 * @brief Rotate player view direction and camera plane
 * 
 * This function implements smooth view rotation using rotation matrices.
 * Both the player direction vector and camera plane must be rotated together.
 */
void	rotate_player_view(t_game *game, double rotation_speed)
{
	double	temp_dir_x;
	double	temp_plane_x;
	double	cos_rotation;
	double	sin_rotation;

	cos_rotation = cos(rotation_speed);
	sin_rotation = sin(rotation_speed);
	
	/* Rotate direction vector */
	temp_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos_rotation - 
		game->player.dir_y * sin_rotation;
	game->player.dir_y = temp_dir_x * sin_rotation + 
		game->player.dir_y * cos_rotation;
	
	/* Rotate camera plane */
	temp_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos_rotation - 
		game->player.plane_y * sin_rotation;
	game->player.plane_y = temp_plane_x * sin_rotation + 
		game->player.plane_y * cos_rotation;
}

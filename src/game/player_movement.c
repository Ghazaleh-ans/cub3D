/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:51:14 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           PLAYER MOVEMENT WITH COLLISION                  */
/* ************************************************************************** */

/**
 * @brief Move player with collision detection and minimap updates
 * 
 * This function handles player movement with comprehensive collision detection:
 * 1. Calculates new position based on movement vector and direction
 * 2. Checks for wall collisions on X and Y axes independently
 * 3. Updates player position only if movement is valid
 * 4. Updates minimap display when player changes grid position
 * 
 * Independent axis checking allows sliding along walls:
 * - Player can move horizontally even if blocked vertically
 * - Player can move vertically even if blocked horizontally
 * 
 * @param game_map Pointer to the main game structure
 * @param delta_x X component of movement vector
 * @param delta_y Y component of movement vector
 * @param direction_sign '+' for positive movement, '-' for negative movement
 */
void	move_player_with_collision(t_game_map *game_map, double delta_x, 
		double delta_y, char direction_sign)
{
	int	previous_grid_x;
	int	previous_grid_y;
	int	new_grid_x;
	int	new_grid_y;

	/* Store previous grid position for minimap updates */
	previous_grid_x = (int)game_map->player_pos_x;
	previous_grid_y = (int)game_map->player_pos_y;
	
	/* Calculate movement based on direction sign */
	if (direction_sign == '+')
	{
		/* Forward/rightward movement */
		/* Check X-axis collision */
		new_grid_x = (int)(game_map->player_pos_x + delta_x);
		if (game_map->map_grid[(int)game_map->player_pos_y][new_grid_x] != '1')
			game_map->player_pos_x += delta_x;
		
		/* Check Y-axis collision */
		new_grid_y = (int)(game_map->player_pos_y + delta_y);
		if (game_map->map_grid[new_grid_y][(int)game_map->player_pos_x] != '1')
			game_map->player_pos_y += delta_y;
	}
	else if (direction_sign == '-')
	{
		/* Backward/leftward movement */
		/* Check X-axis collision */
		new_grid_x = (int)(game_map->player_pos_x - delta_x);
		if (game_map->map_grid[(int)game_map->player_pos_y][new_grid_x] != '1')
			game_map->player_pos_x -= delta_x;
		
		/* Check Y-axis collision */
		new_grid_y = (int)(game_map->player_pos_y - delta_y);
		if (game_map->map_grid[new_grid_y][(int)game_map->player_pos_x] != '1')
			game_map->player_pos_y -= delta_y;
	}
	
	/* Update minimap if player changed grid position */
	if ((int)game_map->player_pos_x != previous_grid_x || 
		(int)game_map->player_pos_y != previous_grid_y)
	{
		update_minimap_player_position(game_map, previous_grid_x, previous_grid_y);
	}
}

/* ************************************************************************** */
/*                           PLAYER VIEW ROTATION                            */
/* ************************************************************************** */

/**
 * @brief Rotate player view direction and camera plane
 * 
 * This function implements smooth view rotation using rotation matrices.
 * Both the player direction vector and camera plane must be rotated
 * together to maintain proper perspective projection.
 * 
 * Rotation mathematics:
 * - Uses 2D rotation matrix: [cos(θ) -sin(θ); sin(θ) cos(θ)]
 * - Positive angles rotate counter-clockwise
 * - Negative angles rotate clockwise
 * - Both direction and plane vectors are rotated by the same angle
 * 
 * The camera plane determines the field of view and must remain
 * perpendicular to the direction vector for correct rendering.
 * 
 * @param game_map Pointer to the main game structure
 * @param rotation_speed Angular speed of rotation (positive = counter-clockwise)
 */
void	rotate_player_view(t_game_map *game_map, double rotation_speed)
{
	double	temp_dir_x;
	double	temp_plane_x;
	double	cos_rotation;
	double	sin_rotation;

	/* Pre-calculate trigonometric values for efficiency */
	cos_rotation = cos(rotation_speed);
	sin_rotation = sin(rotation_speed);
	
	/* Store original direction X component */
	temp_dir_x = game_map->player_dir_x;
	
	/* Rotate direction vector using rotation matrix */
	game_map->player_dir_x = game_map->player_dir_x * cos_rotation - 
		game_map->player_dir_y * sin_rotation;
	game_map->player_dir_y = temp_dir_x * sin_rotation + 
		game_map->player_dir_y * cos_rotation;
	
	/* Store original camera plane X component */
	temp_plane_x = game_map->camera_plane_x;
	
	/* Rotate camera plane vector using the same rotation matrix */
	game_map->camera_plane_x = game_map->camera_plane_x * cos_rotation - 
		game_map->camera_plane_y * sin_rotation;
	game_map->camera_plane_y = temp_plane_x * sin_rotation + 
		game_map->camera_plane_y * cos_rotation;
}
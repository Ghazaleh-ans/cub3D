/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/22 16:46:22 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

/**
 * @brief Check if a specific map position contains a wall
 * 
 * @param game Game structure containing the map
 * @param x X coordinate to check
 * @param y Y coordinate to check  
 * @return 1 if position is a wall or out of bounds, 0 if free
 */
static int	is_wall_at_position(t_game *game, int x, int y)
{
	if (x < 0 || y < 0 || x >= game->map.width || y >= game->map.height)
		return (1);
	return (game->map.grid[y][x] == '1');
}

/**
 * @brief Enhanced collision detection with proper diagonal movement handling
 * 
 * DIAGONAL COLLISION IMPROVEMENTS:
 * This function now properly handles diagonal movement by checking all collision
 * scenarios including corner cases. It prevents "sliding" behavior when the
 * player tries to move diagonally into a corner where they shouldn't be able to.
 * 
 * Collision Priority:
 * 1. Check diagonal destination - if blocked, prevent entire movement
 * 2. Check individual axes - allow partial movement if possible
 * 3. Implement smart corner collision detection
 * 
 * @param game Game structure containing player and map data
 * @param delta_x X-axis movement delta
 * @param delta_y Y-axis movement delta
 */
void	move_player_with_collision(t_game *game, double delta_x, double delta_y)
{
	int		previous_grid_x;
	int		previous_grid_y;
	double	new_x;
	double	new_y;
	int		x_blocked;
	int		y_blocked;
	int		diagonal_blocked;

	previous_grid_x = (int)game->player.pos_x;
	previous_grid_y = (int)game->player.pos_y;
	new_x = game->player.pos_x + delta_x;
	new_y = game->player.pos_y + delta_y;
	
	/* Check collision states for all movement directions */
	x_blocked = is_wall_at_position(game, (int)new_x, (int)game->player.pos_y);
	y_blocked = is_wall_at_position(game, (int)game->player.pos_x, (int)new_y);
	diagonal_blocked = is_wall_at_position(game, (int)new_x, (int)new_y);
	
	/* DIAGONAL COLLISION LOGIC: Enhanced corner handling */
	if (delta_x != 0.0 && delta_y != 0.0)  /* Diagonal movement detected */
	{
		/* Case 1: Diagonal destination is blocked (corner case) */
		if (diagonal_blocked)
		{
			/* Allow sliding along walls only if one axis is free */
			if (!x_blocked && y_blocked)
				game->player.pos_x = new_x;  /* Slide horizontally */
			else if (x_blocked && !y_blocked)
				game->player.pos_y = new_y;  /* Slide vertically */
			/* If both axes blocked, no movement allowed */
		}
		/* Case 2: Diagonal destination is free */
		else
		{
			/* Allow movement in any free direction */
			if (!x_blocked)
				game->player.pos_x = new_x;
			if (!y_blocked)
				game->player.pos_y = new_y;
		}
	}
	/* STRAIGHT MOVEMENT: Simpler collision handling */
	else
	{
		if (!x_blocked && delta_x != 0.0)
			game->player.pos_x = new_x;
		if (!y_blocked && delta_y != 0.0)
			game->player.pos_y = new_y;
	}
	
	/* Safety check: ensure player never ends up inside a wall */
	if (is_wall_at_position(game, (int)game->player.pos_x, (int)game->player.pos_y))
	{
		game->player.pos_x -= delta_x;
		game->player.pos_y -= delta_y;
	}
	
	/* Update minimap if player moved to a different grid cell */
	if ((int)game->player.pos_x != previous_grid_x
		|| (int)game->player.pos_y != previous_grid_y)
	{
		update_minimap_player_position(game, previous_grid_x, previous_grid_y);
	}
}

void	rotate_player_view(t_game *game, double rotation_speed)
{
	double	temp_dir_x;
	double	temp_plane_x;
	double	cos_rotation;
	double	sin_rotation;

	cos_rotation = cos(rotation_speed);
	sin_rotation = sin(rotation_speed);
	temp_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos_rotation
		- game->player.dir_y * sin_rotation;
	game->player.dir_y = temp_dir_x * sin_rotation
		+ game->player.dir_y * cos_rotation;
	temp_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos_rotation
		- game->player.plane_y * sin_rotation;
	game->player.plane_y = temp_plane_x * sin_rotation
		+ game->player.plane_y * cos_rotation;
}

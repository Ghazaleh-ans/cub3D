/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/14 15:29:29 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	move_player_with_collision(t_game *game, double delta_x, double delta_y)
{
	int	previous_grid_x;
	int	previous_grid_y;
	double	new_x;
	double	new_y;

	previous_grid_x = (int)game->player.pos_x;
	previous_grid_y = (int)game->player.pos_y;
	
	// Try to move in X direction
	new_x = game->player.pos_x + delta_x;
	if (game->map.grid[(int)game->player.pos_y][(int)new_x] != '1')
		game->player.pos_x = new_x;
	
	// Try to move in Y direction
	new_y = game->player.pos_y + delta_y;
	if (game->map.grid[(int)new_y][(int)game->player.pos_x] != '1')
		game->player.pos_y = new_y;
	
	#ifdef BONUS
	// Update minimap if player moved to a different grid cell
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

/**
 * @brief Rotate player view direction and camera plane
 */
void	rotate_player_view(t_game *game, double rotation_speed)
{
	double	temp_dir_x;
	double	temp_plane_x;
	double	cos_rotation;
	double	sin_rotation;

	cos_rotation = cos(rotation_speed);
	sin_rotation = sin(rotation_speed);
	
	// Rotate direction vector
	temp_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos_rotation - 
		game->player.dir_y * sin_rotation;
	game->player.dir_y = temp_dir_x * sin_rotation + 
		game->player.dir_y * cos_rotation;
	
	// Rotate camera plane
	temp_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos_rotation - 
		game->player.plane_y * sin_rotation;
	game->player.plane_y = temp_plane_x * sin_rotation + 
		game->player.plane_y * cos_rotation;
}

// #ifdef BONUS
// static void	handle_minimap_update(t_game *game, int prev_x, int prev_y)
// {
// 	int	current_x;
// 	int	current_y;

// 	current_x = (int)game->player.pos_x;
// 	current_y = (int)game->player.pos_y;
// 	if (current_x != prev_x || current_y != prev_y)
// 		update_minimap_player_position(game, prev_x, prev_y);
// }
// #endif


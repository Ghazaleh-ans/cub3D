/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/13 16:17:10 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	move_player_positive(t_game *game, double delta_x, double delta_y)
{
	int	new_grid_x;
	int	new_grid_y;

	new_grid_x = (int)(game->player.pos_x + delta_x);
	if (game->map.grid[(int)game->player.pos_y][new_grid_x] != '1')
		game->player.pos_x += delta_x;
	new_grid_y = (int)(game->player.pos_y + delta_y);
	if (game->map.grid[new_grid_y][(int)game->player.pos_x] != '1')
		game->player.pos_y += delta_y;
}

static void	move_player_negative(t_game *game, double delta_x, double delta_y)
{
	int	new_grid_x;
	int	new_grid_y;

	new_grid_x = (int)(game->player.pos_x - delta_x);
	if (game->map.grid[(int)game->player.pos_y][new_grid_x] != '1')
		game->player.pos_x -= delta_x;
	new_grid_y = (int)(game->player.pos_y - delta_y);
	if (game->map.grid[new_grid_y][(int)game->player.pos_x] != '1')
		game->player.pos_y -= delta_y;
}

#ifdef BONUS
static void	handle_minimap_update(t_game *game, int prev_x, int prev_y)
{
	int	current_x;
	int	current_y;

	current_x = (int)game->player.pos_x;
	current_y = (int)game->player.pos_y;
	if (current_x != prev_x || current_y != prev_y)
		update_minimap_player_position(game, prev_x, prev_y);
}
#endif

void	move_player_with_collision(t_game *game, double delta_x,
		double delta_y, char direction_sign)
{
	int	previous_grid_x;
	int	previous_grid_y;

	previous_grid_x = (int)game->player.pos_x;
	previous_grid_y = (int)game->player.pos_y;
	if (direction_sign == '+')
		move_player_positive(game, delta_x, delta_y);
	else if (direction_sign == '-')
		move_player_negative(game, delta_x, delta_y);
	#ifdef BONUS
	handle_minimap_update(game, previous_grid_x, previous_grid_y);
	#else
	(void)previous_grid_x;
	(void)previous_grid_y;
	#endif
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
	game->player.dir_x = game->player.dir_x * cos_rotation - 
		game->player.dir_y * sin_rotation;
	game->player.dir_y = temp_dir_x * sin_rotation + 
		game->player.dir_y * cos_rotation;
	temp_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos_rotation - 
		game->player.plane_y * sin_rotation;
	game->player.plane_y = temp_plane_x * sin_rotation + 
		game->player.plane_y * cos_rotation;
}

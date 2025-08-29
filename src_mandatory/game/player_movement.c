/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/29 17:19:15 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_mandatory/cub3d.h"

static int	is_wall_at_position(t_game *game, int x, int y)
{
	if (x < 0 || y < 0 || x >= game->map.width || y >= game->map.height)
		return (1);
	return (game->map.grid[y][x] == '1');
}

void	move_player_with_collision(t_game *game, double delta_x, double delta_y)
{
	double	new_x;
	double	new_y;
	int		x_blocked;
	int		y_blocked;
	int		diagonal_blocked;

	new_x = game->player.pos_x + delta_x;
	new_y = game->player.pos_y + delta_y;
	x_blocked = is_wall_at_position(game, (int)new_x, (int)game->player.pos_y);
	y_blocked = is_wall_at_position(game, (int)game->player.pos_x, (int)new_y);
	diagonal_blocked = is_wall_at_position(game, (int)new_x, (int)new_y);
	if (delta_x != 0.0 && delta_y != 0.0)
	{
		if (diagonal_blocked)
		{
			if (!x_blocked && y_blocked)
				game->player.pos_x = new_x;
			else if (x_blocked && !y_blocked)
				game->player.pos_y = new_y;
		}
		else
		{
			if (!x_blocked)
				game->player.pos_x = new_x;
			if (!y_blocked)
				game->player.pos_y = new_y;
		}
	}
	else
	{
		if (!x_blocked && delta_x != 0.0)
			game->player.pos_x = new_x;
		if (!y_blocked && delta_y != 0.0)
			game->player.pos_y = new_y;
	}
	if (is_wall_at_position(game, (int)game->player.pos_x, (int)game->player.pos_y))
	{
		game->player.pos_x -= delta_x;
		game->player.pos_y -= delta_y;
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

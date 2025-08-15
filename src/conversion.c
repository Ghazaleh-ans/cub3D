/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:00:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 17:02:34 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * @brief Convert parsing data to rendering format
 * 
 * This function bridges the gap between the new parsing system
 * and the original rendering system by copying/converting data.
 */
void	convert_parsing_to_rendering(t_game *game)
{
	/* Copy player position from new format to rendering format */
	game->player.pos_x = game->player.x;
	game->player.pos_y = game->player.y;
	game->player.initial_dir = game->player.direction;
	
	/* Copy texture paths to rendering structure */
	if (game->textures_paths[NORTH_TEX])
		game->textures.north.path = ft_strdup(game->textures_paths[NORTH_TEX]);
	if (game->textures_paths[SOUTH_TEX])
		game->textures.south.path = ft_strdup(game->textures_paths[SOUTH_TEX]);
	if (game->textures_paths[EAST_TEX])
		game->textures.east.path = ft_strdup(game->textures_paths[EAST_TEX]);
	if (game->textures_paths[WEST_TEX])
		game->textures.west.path = ft_strdup(game->textures_paths[WEST_TEX]);
}
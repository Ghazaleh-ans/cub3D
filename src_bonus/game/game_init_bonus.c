/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:47:34 by gansari           #+#    #+#             */
/*   Updated: 2025/08/22 15:01:57 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/cub3d.h"

static void	copy_minimap_base(t_game *game)
{
	int	total_bytes;

	total_bytes = game->map.height * MINIMAP_SCALE 
		* game->textures.minimap_base.line_length;
	ft_memcpy(game->textures.minimap_base.data,
		game->textures.minimap.data, total_bytes);
}

void	restore_minimap_from_base(t_game *game)
{
	int	total_bytes;

	total_bytes = game->map.height * MINIMAP_SCALE 
		* game->textures.minimap_base.line_length;
	ft_memcpy(game->textures.minimap.data,
		game->textures.minimap_base.data, total_bytes);
}

void	init_minimap_system(t_game *game)
{
	int	minimap_width;
	int	minimap_height;

	minimap_width = game->map.width * MINIMAP_SCALE;
	minimap_height = game->map.height * MINIMAP_SCALE;
	game->textures.minimap.mlx_ptr = mlx_new_image(game->mlx.instance,
			minimap_width, minimap_height);
	if (!game->textures.minimap.mlx_ptr)
		handle_game_error(game, "Error\nFailed to create minimap\n");
	game->textures.minimap.data
		= mlx_get_data_addr(game->textures.minimap.mlx_ptr,
			&game->textures.minimap.bits_per_pixel,
			&game->textures.minimap.line_length,
			&game->textures.minimap.endian);
	if (!game->textures.minimap.data)
		handle_game_error(game, "Error\nFailed to initialize minimap data\n");
	game->textures.minimap_base.mlx_ptr = mlx_new_image(game->mlx.instance,
			minimap_width, minimap_height);
	if (!game->textures.minimap_base.mlx_ptr)
		handle_game_error(game, "Error\nFailed to create minimap base\n");
	game->textures.minimap_base.data
		= mlx_get_data_addr(game->textures.minimap_base.mlx_ptr,
			&game->textures.minimap_base.bits_per_pixel,
			&game->textures.minimap_base.line_length,
			&game->textures.minimap_base.endian);
	if (!game->textures.minimap_base.data)
		handle_game_error(game, "Error\nFailed to initialize minimap base\n");
	draw_minimap_display(game);
	copy_minimap_base(game);
}

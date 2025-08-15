/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:56 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 17:03:07 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * @brief Main render frame function (compatible with both systems)
 * Note: render_background_colors is now in drawing.c
 */
int	render_frame(t_game *game)
{
	process_movement_input(game);
	render_background_colors(game);
	execute_raycasting(game);
	mlx_put_image_to_window(game->mlx.instance, game->mlx.window, 
		game->textures.screen.mlx_ptr, 0, 0);
	#ifdef BONUS
	mlx_put_image_to_window(game->mlx.instance, game->mlx.window,
		game->textures.minimap.mlx_ptr, MINIMAP_OFFSET, MINIMAP_OFFSET);
	#endif
	return (0);
}
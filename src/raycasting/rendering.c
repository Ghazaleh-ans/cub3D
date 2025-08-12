/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:56 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 13:14:33 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           MAIN RENDERING FUNCTION                         */
/* ************************************************************************** */

/**
 * @brief Main rendering function called every frame
 * 
 * This function orchestrates the complete rendering pipeline:
 * 1. Clear the screen with floor and ceiling colors
 * 2. Execute ray-casting to render 3D walls
 * 3. Display the rendered frame to the window
 * 4. Overlay the minimap for navigation aid (bonus only)
 * 
 * The rendering order is critical:
 * - Background first (floor/ceiling)
 * - Walls second (3D geometry)
 * - UI elements last (minimap, bonus only)
 * 
 * This function is called by MLX in the main loop for continuous rendering.
 * 
 * @param game_map Pointer to the main game structure
 * @return 0 (required for MLX loop compatibility)
 */
int	render_frame(t_game_map *game_map)
{
	render_background_colors(game_map);
	execute_raycasting(game_map);
	mlx_put_image_to_window(game_map->mlx_instance, game_map->mlx_window, 
		game_map->texture_images[4].mlx_image_ptr, 0, 0);
	#ifdef BONUS
	mlx_put_image_to_window(game_map->mlx_instance, game_map->mlx_window,
		game_map->minimap_image.mlx_image_ptr, MINIMAP_OFFSET, MINIMAP_OFFSET);
	#endif
	return (0);
}

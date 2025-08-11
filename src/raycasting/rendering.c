/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:51:56 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:51:57 by gansari          ###   ########.fr       */
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
 * 4. Overlay the minimap for navigation aid
 * 
 * The rendering order is critical:
 * - Background first (floor/ceiling)
 * - Walls second (3D geometry)
 * - UI elements last (minimap)
 * 
 * This function is called by MLX in the main loop for continuous rendering.
 * 
 * @param game_map Pointer to the main game structure
 * @return 0 (required for MLX loop compatibility)
 */
int	render_frame(t_game_map *game_map)
{
	/* Clear screen with floor and ceiling colors */
	render_background_colors(game_map);
	
	/* Execute ray-casting to render 3D world */
	execute_raycasting(game_map);
	
	/* Display the rendered frame to the window */
	mlx_put_image_to_window(game_map->mlx_instance, game_map->mlx_window, 
		game_map->texture_images[4].mlx_image_ptr, 0, 0);
	
	/* Overlay minimap in the top-left corner */
	mlx_put_image_to_window(game_map->mlx_instance, game_map->mlx_window,
		game_map->minimap_image.mlx_image_ptr, MINIMAP_OFFSET, MINIMAP_OFFSET);
	
	return (0);
}
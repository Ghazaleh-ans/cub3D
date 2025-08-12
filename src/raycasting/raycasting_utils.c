/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:53:26 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 13:15:11 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           TEXTURE SAMPLING                                */
/* ************************************************************************** */

/**
 * @brief Get color value from texture at specified coordinates
 * 
 * This function samples a color from a texture image at the given coordinates.
 * It directly accesses the image buffer for maximum performance during
 * real-time rendering.
 * 
 * The function performs:
 * 1. Calculates the exact pixel address in the texture buffer
 * 2. Reads the 32-bit color value from that address
 * 3. Returns the color for immediate use in rendering
 * 
 * No bounds checking is performed here for performance reasons - the caller
 * must ensure coordinates are within texture bounds (0 to TEXTURE_SIZE-1).
 * 
 * @param game_map Pointer to the main game structure
 * @param texture_x X coordinate in texture (0 to TEXTURE_SIZE-1)
 * @param texture_y Y coordinate in texture (0 to TEXTURE_SIZE-1) 
 * @param texture_index Index of texture to sample from (0-3 for directional textures)
 * @return 32-bit ARGB color value from the texture
 */
int	get_texture_color(t_game_map *game_map, int texture_x, int texture_y, int texture_index)
{
	char	*pixel_address;

	pixel_address = game_map->texture_images[texture_index].image_buffer +
		(texture_y * game_map->texture_images[texture_index].line_length +
		texture_x * (game_map->texture_images[texture_index].bits_per_pixel / 8));
	return (*(int *)pixel_address);
}
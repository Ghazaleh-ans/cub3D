/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_game_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:30:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 17:13:32 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * @brief Debug version of texture loading with detailed error reporting
 */
static void	load_texture_image_debug(t_game *game, t_image *texture, char *texture_name)
{
	if (!texture->path)
	{
		printf("ERROR: %s texture path is NULL\n", texture_name);
		handle_game_error(game, "Error\nTexture path is NULL\n");
		return;
	}
	
	printf("Loading %s texture from: '%s'\n", texture_name, texture->path);
	
	/* Check if file exists and is readable */
	int fd = open(texture->path, O_RDONLY);
	if (fd < 0)
	{
		printf("ERROR: Cannot open %s texture file: '%s'\n", texture_name, texture->path);
		printf("Error details: %s\n", strerror(errno));
		handle_game_error(game, "Error\nCannot open texture file\n");
		return;
	}
	close(fd);
	
	texture->mlx_ptr = mlx_xpm_file_to_image(game->mlx.instance, 
		texture->path, &texture->width, &texture->height);
	if (!texture->mlx_ptr)
	{
		printf("ERROR: Failed to load %s texture: '%s'\n", texture_name, texture->path);
		printf("MLX returned NULL - check if file is valid XPM format\n");
		handle_game_error(game, "Error\nFailed to load texture file\n");
		return;
	}
	
	printf("SUCCESS: Loaded %s texture (%dx%d)\n", texture_name, texture->width, texture->height);
		
	texture->data = mlx_get_data_addr(texture->mlx_ptr,
		&texture->bits_per_pixel, &texture->line_length, &texture->endian);
	if (!texture->data)
	{
		printf("ERROR: Failed to get %s texture data address\n", texture_name);
		handle_game_error(game, "Error\nFailed to get texture data address\n");
		return;
	}
}

/**
 * @brief Debug version showing which textures are being loaded
 */
void	init_mlx_images_debug(t_game *game)
{
	printf("\n=== TEXTURE LOADING DEBUG ===\n");
	
	/* Print parsed texture paths */
	printf("Parsed texture paths:\n");
	printf("  NORTH: %s\n", game->textures_paths[NORTH_TEX] ? game->textures_paths[NORTH_TEX] : "NULL");
	printf("  SOUTH: %s\n", game->textures_paths[SOUTH_TEX] ? game->textures_paths[SOUTH_TEX] : "NULL");
	printf("  WEST:  %s\n", game->textures_paths[WEST_TEX] ? game->textures_paths[WEST_TEX] : "NULL");
	printf("  EAST:  %s\n", game->textures_paths[EAST_TEX] ? game->textures_paths[EAST_TEX] : "NULL");
	
	/* Print converted texture paths */
	printf("\nConverted texture paths:\n");
	printf("  NORTH: %s\n", game->textures.north.path ? game->textures.north.path : "NULL");
	printf("  SOUTH: %s\n", game->textures.south.path ? game->textures.south.path : "NULL");
	printf("  WEST:  %s\n", game->textures.west.path ? game->textures.west.path : "NULL");
	printf("  EAST:  %s\n", game->textures.east.path ? game->textures.east.path : "NULL");
	
	printf("\nLoading textures...\n");
	
	load_texture_image_debug(game, &game->textures.north, "NORTH");
	load_texture_image_debug(game, &game->textures.south, "SOUTH");
	load_texture_image_debug(game, &game->textures.east, "EAST");
	load_texture_image_debug(game, &game->textures.west, "WEST");
	
	printf("\nCreating screen buffer...\n");
	game->textures.screen.mlx_ptr = mlx_new_image(game->mlx.instance,
		game->mlx.width, game->mlx.height);
	if (!game->textures.screen.mlx_ptr)
		handle_game_error(game, "Error\nFailed to create display buffer\n");
		
	game->textures.screen.data = mlx_get_data_addr(game->textures.screen.mlx_ptr,
		&game->textures.screen.bits_per_pixel, &game->textures.screen.line_length,
		&game->textures.screen.endian);
	if (!game->textures.screen.data)
		handle_game_error(game, "Error\nFailed to get screen buffer data address\n");
		
	printf("SUCCESS: Screen buffer created\n");
	printf("=== TEXTURE LOADING COMPLETE ===\n\n");
}
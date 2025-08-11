/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:19:37 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:19:39 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/* ************************************************************************** */
/*                           MLX IMAGE CLEANUP                               */
/* ************************************************************************** */

/**
 * @brief Safely destroy all MLX images to prevent memory leaks
 * 
 * This function destroys all MLX images that were created during the
 * program execution. It includes proper NULL checking to avoid
 * attempting to destroy already freed or uninitialized images.
 * 
 * The function handles:
 * - All texture images (North, South, West, East textures)
 * - Main display buffer image
 * - Minimap image
 * 
 * @param game_map Pointer to the main game structure
 */
void	destroy_mlx_images(t_game_map *game_map)
{
	int	texture_index;

	/* Only proceed if MLX instance exists */
	if (!game_map->mlx_instance)
		return ;
	
	/* Destroy all texture images */
	texture_index = 0;
	while (texture_index < 5)
	{
		if (game_map->texture_images[texture_index].mlx_image_ptr)
		{
			mlx_destroy_image(game_map->mlx_instance, 
				game_map->texture_images[texture_index].mlx_image_ptr);
			game_map->texture_images[texture_index].mlx_image_ptr = NULL;
		}
		texture_index++;
	}
	
	/* Destroy minimap image */
	if (game_map->minimap_image.mlx_image_ptr)
	{
		mlx_destroy_image(game_map->mlx_instance, game_map->minimap_image.mlx_image_ptr);
		game_map->minimap_image.mlx_image_ptr = NULL;
	}
}

/* ************************************************************************** */
/*                          TEXTURE PATH CLEANUP                            */
/* ************************************************************************** */

/**
 * @brief Free all allocated texture paths
 * 
 * This function frees the memory allocated for texture file paths.
 * Only the first 4 images (directional textures) have paths,
 * as the 5th image is the display buffer which doesn't have a file path.
 * 
 * The function includes NULL checking to avoid double-free errors.
 * 
 * @param game_map Pointer to the main game structure
 */
void	free_texture_paths(t_game_map *game_map)
{
	int	texture_index;

	texture_index = 0;
	
	/* Free paths for directional textures only (indices 0-3) */
	while (texture_index < 4)
	{
		if (game_map->texture_images[texture_index].texture_path)
		{
			free(game_map->texture_images[texture_index].texture_path);
			game_map->texture_images[texture_index].texture_path = NULL;
		}
		texture_index++;
	}
}

/* ************************************************************************** */
/*                           STRING ARRAY CLEANUP                           */
/* ************************************************************************** */

/**
 * @brief Free a dynamically allocated array of strings
 * 
 * This function properly frees a NULL-terminated array of strings,
 * such as the map grid. It first frees each individual string,
 * then frees the array itself.
 * 
 * The function handles:
 * - NULL array pointers (safe to call with NULL)
 * - Arrays with NULL string elements
 * - Proper sequential freeing to avoid memory leaks
 * 
 * @param string_array Pointer to the array of strings to free
 */
void	free_string_array(char **string_array)
{
	int	string_index;

	/* Return early if array is NULL */
	if (!string_array)
		return ;
	
	string_index = 0;
	
	/* Free each individual string */
	while (string_array[string_index])
	{
		free(string_array[string_index]);
		string_array[string_index] = NULL;  /* Set to NULL for safety */
		string_index++;
	}
	
	/* Free the array itself */
	free(string_array);
}

/* ************************************************************************** */
/*                           PARSING BUFFER CLEANUP                         */
/* ************************************************************************** */

/**
 * @brief Free parsing-related buffers and temporary data
 * 
 * This function cleans up temporary buffers used during map file parsing.
 * These buffers are typically allocated during the parsing process and
 * need to be freed before program exit or error handling.
 * 
 * @param game_map Pointer to the main game structure
 */
static void	free_parsing_buffers(t_game_map *game_map)
{
	/* Free current line buffer if allocated */
	if (game_map->current_line)
	{
		free(game_map->current_line);
		game_map->current_line = NULL;
	}
	
	/* Free map data buffer if allocated */
	if (game_map->map_data_buffer)
	{
		free(game_map->map_data_buffer);
		game_map->map_data_buffer = NULL;
	}
}

/* ************************************************************************** */
/*                            MLX CLEANUP                                   */
/* ************************************************************************** */

/**
 * @brief Properly shutdown MLX and free related resources
 * 
 * This function handles the proper shutdown sequence for MLX:
 * 1. Destroy the window (if it exists)
 * 2. Destroy the display connection
 * 3. Free the MLX instance
 * 
 * The order is important to avoid crashes or memory leaks.
 * 
 * @param game_map Pointer to the main game structure
 */
static void	cleanup_mlx_resources(t_game_map *game_map)
{
	/* Destroy window if it exists */
	if (game_map->mlx_window && game_map->mlx_instance)
	{
		mlx_destroy_window(game_map->mlx_instance, game_map->mlx_window);
		game_map->mlx_window = NULL;
	}
	
	/* Destroy display and free MLX instance */
	if (game_map->mlx_instance)
	{
		mlx_destroy_display(game_map->mlx_instance);
		free(game_map->mlx_instance);
		game_map->mlx_instance = NULL;
	}
}

/* ************************************************************************** */
/*                            COMPLETE CLEANUP                              */
/* ************************************************************************** */

/**
 * @brief Comprehensive cleanup and program exit function
 * 
 * This function performs a complete cleanup of all allocated resources
 * and exits the program gracefully. It's designed to be called from
 * MLX event handlers (like window close or ESC key press) or error conditions.
 * 
 * The cleanup order is important:
 * 1. Free texture paths (before destroying images)
 * 2. Free string arrays (map data)
 * 3. Free parsing buffers
 * 4. Destroy MLX images
 * 5. Cleanup MLX resources
 * 6. Exit the program
 * 
 * @param game_map Pointer to the main game structure
 * @return Always returns 0 (for MLX hook compatibility)
 */
int	clean_exit_program(t_game_map *game_map)
{
	/* Phase 1: Free texture-related memory */
	free_texture_paths(game_map);
	
	/* Phase 2: Free map and parsing data */
	free_string_array(game_map->map_grid);
	free_parsing_buffers(game_map);
	
	/* Phase 3: Cleanup MLX resources */
	destroy_mlx_images(game_map);
	cleanup_mlx_resources(game_map);
	
	/* Phase 4: Exit program successfully */
	exit(EXIT_SUCCESS);
	
	/* This return is never reached but required for MLX hook compatibility */
	return (0);
}

/* ************************************************************************** */
/*                           ERROR CLEANUP                                  */
/* ************************************************************************** */

/**
 * @brief Cleanup function for error conditions during parsing
 * 
 * This function is specifically designed for cleanup during parsing errors,
 * before MLX resources have been allocated. It only cleans up parsing-related
 * memory and texture paths.
 * 
 * @param game_map Pointer to the main game structure
 */
void	cleanup_parsing_error(t_game_map *game_map)
{
	/* Free map data if allocated */
	free_string_array(game_map->map_grid);
	
	/* Free parsing buffers */
	free_parsing_buffers(game_map);
	
	/* Free texture paths if any were allocated */
	free_texture_paths(game_map);
}

/**
 * @brief Cleanup function for error conditions during game initialization
 * 
 * This function handles cleanup when errors occur after MLX initialization
 * but before the main game loop starts. It performs more comprehensive
 * cleanup including MLX resources.
 * 
 * @param game_map Pointer to the main game structure
 */
void	cleanup_initialization_error(t_game_map *game_map)
{
	/* Free all parsing and game data */
	cleanup_parsing_error(game_map);
	
	/* Clean up any MLX resources that may have been allocated */
	destroy_mlx_images(game_map);
	cleanup_mlx_resources(game_map);
}
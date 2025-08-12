/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:19:37 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 17:12:19 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/* ************************************************************************** */
/*                           MLX IMAGE CLEANUP                               */
/* ************************************************************************** */

/**
 * @brief Safely destroy all MLX images to prevent memory leaks
 */
void	destroy_mlx_images(t_game *game)
{
	if (!game->mlx.instance)
		return ;
		
	/* Destroy individual texture images */
	if (game->textures.north.mlx_ptr)
	{
		mlx_destroy_image(game->mlx.instance, game->textures.north.mlx_ptr);
		game->textures.north.mlx_ptr = NULL;
	}
	if (game->textures.south.mlx_ptr)
	{
		mlx_destroy_image(game->mlx.instance, game->textures.south.mlx_ptr);
		game->textures.south.mlx_ptr = NULL;
	}
	if (game->textures.east.mlx_ptr)
	{
		mlx_destroy_image(game->mlx.instance, game->textures.east.mlx_ptr);
		game->textures.east.mlx_ptr = NULL;
	}
	if (game->textures.west.mlx_ptr)
	{
		mlx_destroy_image(game->mlx.instance, game->textures.west.mlx_ptr);
		game->textures.west.mlx_ptr = NULL;
	}
	if (game->textures.screen.mlx_ptr)
	{
		mlx_destroy_image(game->mlx.instance, game->textures.screen.mlx_ptr);
		game->textures.screen.mlx_ptr = NULL;
	}
	
	#ifdef BONUS
	if (game->textures.minimap.mlx_ptr)
	{
		mlx_destroy_image(game->mlx.instance, game->textures.minimap.mlx_ptr);
		game->textures.minimap.mlx_ptr = NULL;
	}
	#endif
}

/* ************************************************************************** */
/*                          TEXTURE PATH CLEANUP                            */
/* ************************************************************************** */

/**
 * @brief Free all allocated texture paths
 */
void	free_texture_paths(t_game *game)
{
	if (game->textures.north.path)
	{
		free(game->textures.north.path);
		game->textures.north.path = NULL;
	}
	if (game->textures.south.path)
	{
		free(game->textures.south.path);
		game->textures.south.path = NULL;
	}
	if (game->textures.east.path)
	{
		free(game->textures.east.path);
		game->textures.east.path = NULL;
	}
	if (game->textures.west.path)
	{
		free(game->textures.west.path);
		game->textures.west.path = NULL;
	}
}

/* ************************************************************************** */
/*                           STRING ARRAY CLEANUP                           */
/* ************************************************************************** */

/**
 * @brief Free a dynamically allocated array of strings
 */
void	free_string_array(char **string_array)
{
	int	string_index;

	if (!string_array)
		return ;
	string_index = 0;
	while (string_array[string_index])
	{
		free(string_array[string_index]);
		string_array[string_index] = NULL;
		string_index++;
	}
	free(string_array);
}

/* ************************************************************************** */
/*                           PARSING BUFFER CLEANUP                         */
/* ************************************************************************** */

/**
 * @brief Free parsing-related buffers and temporary data
 */
static void	free_parsing_buffers(t_game *game)
{
	if (game->map.current_line)
	{
		free(game->map.current_line);
		game->map.current_line = NULL;
	}
	
	if (game->map.data_buffer)
	{
		free(game->map.data_buffer);
		game->map.data_buffer = NULL;
	}
}

/* ************************************************************************** */
/*                            MLX CLEANUP                                   */
/* ************************************************************************** */

/**
 * @brief Properly shutdown MLX and free related resources
 */
static void	cleanup_mlx_resources(t_game *game)
{
	if (game->mlx.window && game->mlx.instance)
	{
		mlx_destroy_window(game->mlx.instance, game->mlx.window);
		game->mlx.window = NULL;
	}
	
	if (game->mlx.instance)
	{
		mlx_destroy_display(game->mlx.instance);
		free(game->mlx.instance);
		game->mlx.instance = NULL;
	}
}

/* ************************************************************************** */
/*                            COMPLETE CLEANUP                              */
/* ************************************************************************** */

/**
 * @brief Comprehensive cleanup and program exit function
 */
int	clean_exit_program(t_game *game)
{
	/* Phase 1: Free texture-related memory */
	free_texture_paths(game);
	
	/* Phase 2: Free map and parsing data */
	free_string_array(game->map.grid);
	free_parsing_buffers(game);
	
	/* Phase 3: Cleanup MLX resources */
	destroy_mlx_images(game);
	cleanup_mlx_resources(game);
	
	/* Phase 4: Exit program successfully */
	exit(EXIT_SUCCESS);
	
	return (0);
}

/* ************************************************************************** */
/*                           ERROR CLEANUP                                  */
/* ************************************************************************** */

/**
 * @brief Cleanup function for error conditions during parsing
 */
void	cleanup_parsing_error(t_game *game)
{
	free_string_array(game->map.grid);
	free_parsing_buffers(game);
	free_texture_paths(game);
}

/**
 * @brief Cleanup function for error conditions during game initialization
 */
void	cleanup_initialization_error(t_game *game)
{
	cleanup_parsing_error(game);
	destroy_mlx_images(game);
	cleanup_mlx_resources(game);
}

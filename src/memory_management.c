/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:19:37 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 17:00:49 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * @brief Free texture paths (adapted for new parsing system)
 */
void	free_texture_paths(t_game *game)
{
	int i;
	
	/* Free new parsing system texture paths */
	i = 0;
	while (i < TEX_COUNT)
	{
		if (game->textures_paths[i])
		{
			free(game->textures_paths[i]);
			game->textures_paths[i] = NULL;
		}
		i++;
	}
	
	/* Free original rendering system texture paths */
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

/**
 * @brief Free string array wrapper for compatibility
 */
void	free_string_array(char **string_array)
{
	free_double_ptr(string_array);
}

/**
 * @brief Free parsing buffers (adapted for new system)
 */
void	free_parsing_buffers(t_game *game)
{
	if (game->current_line)
	{
		free(game->current_line);
		game->current_line = NULL;
	}
	if (game->data_buffer)
	{
		free(game->data_buffer);
		game->data_buffer = NULL;
	}
}

/**
 * @brief Clean exit program (adapted for new parsing system)
 */
int	clean_exit_program(t_game *game)
{
	free_texture_paths(game);
	if (game->map)
		free_string_array(game->map);
	free_parsing_buffers(game);
	destroy_mlx_images(game);
	cleanup_mlx_resources(game);
	exit(EXIT_SUCCESS);
	return (0);
}

/**
 * @brief Cleanup parsing error (adapted for new system)
 */
void	cleanup_parsing_error(t_game *game)
{
	if (game->map)
		free_string_array(game->map);
	free_parsing_buffers(game);
	free_texture_paths(game);
}

/**
 * @brief Cleanup initialization error (adapted for new system)
 */
void	cleanup_initialization_error(t_game *game)
{
	cleanup_parsing_error(game);
	destroy_mlx_images(game);
	cleanup_mlx_resources(game);
}
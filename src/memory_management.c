/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:19:37 by gansari           #+#    #+#             */
/*   Updated: 2025/08/13 12:56:47 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	destroy_mlx_images(t_game *game)
{
	if (!game->mlx.instance)
		return ;
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

int	clean_exit_program(t_game *game)
{
	free_texture_paths(game);
	free_string_array(game->map.grid);
	free_parsing_buffers(game);
	destroy_mlx_images(game);
	cleanup_mlx_resources(game);
	exit(EXIT_SUCCESS);
	return (0);
}

void	cleanup_parsing_error(t_game *game)
{
	free_string_array(game->map.grid);
	free_parsing_buffers(game);
	free_texture_paths(game);
}

void	cleanup_initialization_error(t_game *game)
{
	cleanup_parsing_error(game);
	destroy_mlx_images(game);
	cleanup_mlx_resources(game);
}

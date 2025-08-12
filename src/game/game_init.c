/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:49:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 17:52:49 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                            ERROR HANDLING                                 */
/* ************************************************************************** */

/**
 * @brief Handle game initialization errors with proper cleanup
 */
void	handle_game_error(t_game *game, char *error_message)
{
	ft_printf("%s", error_message);
	free_texture_paths(game);
	free_string_array(game->map.grid);
	destroy_mlx_images(game);
	if (game->mlx.window && game->mlx.instance)
		mlx_destroy_window(game->mlx.instance, game->mlx.window);
	if (game->mlx.instance)
	{
		mlx_destroy_display(game->mlx.instance);
		free(game->mlx.instance);
	}
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                           GAME STATISTICS SETUP                           */
/* ************************************************************************** */

/**
 * @brief Initialize game-specific settings and player camera
 */
void	init_game_settings(t_game *game)
{
	/* Set movement and display parameters */
	game->player.move_speed = MOVE_SPEED;
	game->player.rotate_speed = ROTATION_SPEED;
	game->mlx.width = DEFAULT_WIDTH;
	game->mlx.height = DEFAULT_HEIGHT;
	
	/* Set camera plane based on initial player direction */
	if (game->player.initial_dir == 'N')
	{
		game->player.plane_x = 0.66;
		game->player.plane_y = 0.0;
	}
	else if (game->player.initial_dir == 'S')
	{
		game->player.plane_x = -0.66;
		game->player.plane_y = 0.0;
	}
	else if (game->player.initial_dir == 'W')
	{
		game->player.plane_x = 0.0;
		game->player.plane_y = 0.66;
	}
	else if (game->player.initial_dir == 'E')
	{
		game->player.plane_x = 0.0;
		game->player.plane_y = -0.66;
	}
}

/* ************************************************************************** */
/*                           MLX IMAGE INITIALIZATION                        */
/* ************************************************************************** */

/**
 * @brief Load a single texture image
 */
static void	load_texture_image(t_game *game, t_image *texture)
{
	if (!texture->path)
		return;
		
	texture->mlx_ptr = mlx_xpm_file_to_image(game->mlx.instance, 
		texture->path, &texture->width, &texture->height);
	if (!texture->mlx_ptr)
		handle_game_error(game, "Error\nFailed to load texture file\n");
		
	texture->data = mlx_get_data_addr(texture->mlx_ptr,
		&texture->bits_per_pixel, &texture->line_length, &texture->endian);
	if (!texture->data)
		handle_game_error(game, "Error\nFailed to get texture data address\n");
}

/**
 * @brief Initialize all MLX images including textures and display buffer
 */
void	init_mlx_images(t_game *game)
{
	/* Load wall textures */
	load_texture_image(game, &game->textures.north);
	load_texture_image(game, &game->textures.south);
	load_texture_image(game, &game->textures.east);
	load_texture_image(game, &game->textures.west);
	
	/* Create main display buffer */
	game->textures.screen.mlx_ptr = mlx_new_image(game->mlx.instance,
		game->mlx.width, game->mlx.height);
	if (!game->textures.screen.mlx_ptr)
		handle_game_error(game, "Error\nFailed to create display buffer\n");
		
	game->textures.screen.data = mlx_get_data_addr(game->textures.screen.mlx_ptr,
		&game->textures.screen.bits_per_pixel, &game->textures.screen.line_length,
		&game->textures.screen.endian);
	if (!game->textures.screen.data)
		handle_game_error(game, "Error\nFailed to get screen buffer data address\n");
}

/* ************************************************************************** */
/*                           MINIMAP INITIALIZATION (BONUS)                 */
/* ************************************************************************** */

#ifdef BONUS
/**
 * @brief Initialize the minimap display system
 */
static void	init_minimap_system(t_game *game)
{
	int	minimap_width;
	int	minimap_height;

	minimap_width = game->map.width * MINIMAP_SCALE;
	minimap_height = game->map.height * MINIMAP_SCALE;
	
	game->textures.minimap.mlx_ptr = mlx_new_image(game->mlx.instance,
		minimap_width, minimap_height);
	if (!game->textures.minimap.mlx_ptr)
		handle_game_error(game, "Error\nFailed to create minimap\n");
		
	game->textures.minimap.data = mlx_get_data_addr(game->textures.minimap.mlx_ptr,
		&game->textures.minimap.bits_per_pixel, &game->textures.minimap.line_length,
		&game->textures.minimap.endian);
	if (!game->textures.minimap.data)
		handle_game_error(game, "Error\nFailed to initialize minimap data\n");
		
	draw_minimap_display(game);
}
#endif

/* ************************************************************************** */
/*                           MAIN INITIALIZATION FUNCTION                    */
/* ************************************************************************** */

/**
 * @brief Main game engine initialization function
 */
int	init_game_engine(t_game *game)
{
	/* Initialize game settings and player camera */
	init_game_settings(game);
	
	/* Create MLX instance and window */
	game->mlx.instance = mlx_init();
	if (!game->mlx.instance)
		handle_game_error(game, "Error\nFailed to initialize MLX\n");
		
	game->mlx.window = mlx_new_window(game->mlx.instance, 
		game->mlx.width, game->mlx.height, "cub3D");
	if (!game->mlx.window)
		handle_game_error(game, "Error\nFailed to create game window\n");
	
	/* Load textures and create display buffer */
	init_mlx_images(game);
	
	#ifdef BONUS
	/* Initialize minimap system */
	init_minimap_system(game);
	#endif
	
	/* Set up event handlers and start main loop */
	mlx_loop_hook(game->mlx.instance, &render_frame, game);
	mlx_hook(game->mlx.window, 2, 1L << 0, handle_keyboard_input, game);
	#ifdef BONUS
	mlx_hook(game->mlx.window, 6, 1L << 6, handle_mouse_rotation, game);
	#endif
	mlx_hook(game->mlx.window, 17, 1L << 0, clean_exit_program, game);
	
	/* Start the game loop */
	mlx_loop(game->mlx.instance);
	
	return (0);
}

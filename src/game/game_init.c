/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:49:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:49:02 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                            ERROR HANDLING                                 */
/* ************************************************************************** */

/**
 * @brief Handle game initialization errors with proper cleanup
 * 
 * This function provides centralized error handling for game initialization
 * failures. It performs comprehensive cleanup of all allocated resources
 * including MLX resources, textures, map data, and memory allocations.
 * 
 * The cleanup order is critical to prevent crashes:
 * 1. Display error message
 * 2. Free texture paths and map data
 * 3. Destroy MLX images
 * 4. Destroy MLX window and display
 * 5. Exit program
 * 
 * @param game_map Pointer to the main game structure
 * @param error_message Descriptive error message to display
 */
void	handle_game_error(t_game_map *game_map, char *error_message)
{
	/* Display error message */
	ft_printf("%s", error_message);
	
	/* Free game data */
	free_texture_paths(game_map);
	free_string_array(game_map->map_grid);
	
	/* Clean up MLX resources */
	destroy_mlx_images(game_map);
	
	/* Destroy window if it exists */
	if (game_map->mlx_window && game_map->mlx_instance)
		mlx_destroy_window(game_map->mlx_instance, game_map->mlx_window);
	
	/* Clean up MLX instance */
	if (game_map->mlx_instance)
	{
		mlx_destroy_display(game_map->mlx_instance);
		free(game_map->mlx_instance);
	}
	
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                           GAME STATISTICS SETUP                           */
/* ************************************************************************** */

/**
 * @brief Initialize game-specific settings and player camera
 * 
 * This function sets up the core game parameters:
 * - Movement and rotation speeds for responsive controls
 * - Display dimensions for window size
 * - Camera plane orientation based on initial player direction
 * 
 * The camera plane determines the field of view (FOV):
 * - Plane length of 0.66 creates approximately 60-degree FOV
 * - Plane is perpendicular to player direction vector
 * - Plane orientation depends on initial player facing direction
 * 
 * @param game_map Pointer to the main game structure
 */
void	initialize_game_statistics(t_game_map *game_map)
{
	/* Set movement parameters for smooth gameplay */
	game_map->movement_speed = MOVE_SPEED;
	game_map->rotation_speed = ROTATION_SPEED;
	
	/* Set display dimensions */
	game_map->display_width = DEFAULT_WIDTH;
	game_map->display_height = DEFAULT_HEIGHT;
	
	/* Initialize camera plane based on player direction */
	if (game_map->player_direction == 'N')
	{
		game_map->camera_plane_x = 0.66;   /* Plane perpendicular to North direction */
		game_map->camera_plane_y = 0.0;
	}
	else if (game_map->player_direction == 'S')
	{
		game_map->camera_plane_x = -0.66;  /* Plane perpendicular to South direction */
		game_map->camera_plane_y = 0.0;
	}
	else if (game_map->player_direction == 'W')
	{
		game_map->camera_plane_x = 0.0;
		game_map->camera_plane_y = 0.66;   /* Plane perpendicular to West direction */
	}
	else if (game_map->player_direction == 'E')
	{
		game_map->camera_plane_x = 0.0;
		game_map->camera_plane_y = -0.66;  /* Plane perpendicular to East direction */
	}
}

/* ************************************************************************** */
/*                           MLX IMAGE INITIALIZATION                        */
/* ************************************************************************** */

/**
 * @brief Initialize all MLX images including textures and display buffer
 * 
 * This function loads and initializes all graphical assets:
 * 1. Load wall textures from XPM files (North, South, West, East)
 * 2. Create main display buffer for rendering
 * 3. Initialize image data addresses for direct pixel access
 * 4. Validate all operations for error handling
 * 
 * The texture loading order corresponds to the direction enum:
 * - Index 0: North texture
 * - Index 1: South texture  
 * - Index 2: West texture
 * - Index 3: East texture
 * - Index 4: Display buffer
 * 
 * @param game_map Pointer to the main game structure
 */
void	initialize_mlx_images(t_game_map *game_map)
{
	int	texture_index;

	texture_index = 0;
	
	/* Load wall textures from XPM files */
	while (texture_index < 4)
	{
		game_map->texture_images[texture_index].mlx_image_ptr = 
			mlx_xpm_file_to_image(game_map->mlx_instance, 
				game_map->texture_images[texture_index].texture_path,
				&game_map->texture_images[texture_index].image_width, 
				&game_map->texture_images[texture_index].image_height);
		
		if (!game_map->texture_images[texture_index].mlx_image_ptr)
			handle_game_error(game_map, "Error\nFailed to load texture file\n");
		
		texture_index++;
	}
	
	/* Create main display buffer */
	game_map->texture_images[4].mlx_image_ptr = mlx_new_image(game_map->mlx_instance,
		game_map->display_width, game_map->display_height);
	if (!game_map->texture_images[4].mlx_image_ptr)
		handle_game_error(game_map, "Error\nFailed to create display buffer\n");
	
	/* Initialize image data addresses for all images */
	texture_index = 0;
	while (texture_index < 5)
	{
		game_map->texture_images[texture_index].image_buffer = 
			mlx_get_data_addr(game_map->texture_images[texture_index].mlx_image_ptr,
				&game_map->texture_images[texture_index].bits_per_pixel,
				&game_map->texture_images[texture_index].line_length,
				&game_map->texture_images[texture_index].endianness);
		
		if (!game_map->texture_images[texture_index].image_buffer)
			handle_game_error(game_map, "Error\nFailed to get image data address\n");
		
		texture_index++;
	}
}

/* ************************************************************************** */
/*                           MINIMAP INITIALIZATION                          */
/* ************************************************************************** */

/**
 * @brief Initialize the minimap display system
 * 
 * This function creates and sets up the minimap functionality:
 * 1. Calculate minimap dimensions based on map size and scale
 * 2. Create MLX image for minimap rendering
 * 3. Initialize image data address for pixel access
 * 4. Draw initial minimap content
 * 
 * The minimap provides real-time navigation aid by showing:
 * - Map layout with walls and open spaces
 * - Player position and orientation
 * - Scaled representation for easy viewing
 * 
 * @param game_map Pointer to the main game structure
 */
static void	initialize_minimap_system(t_game_map *game_map)
{
	int	minimap_width;
	int	minimap_height;

	/* Calculate minimap dimensions */
	minimap_width = game_map->map_width * MINIMAP_SCALE;
	minimap_height = game_map->map_height * MINIMAP_SCALE;
	
	/* Create minimap image */
	game_map->minimap_image.mlx_image_ptr = mlx_new_image(game_map->mlx_instance,
		minimap_width, minimap_height);
	if (!game_map->minimap_image.mlx_image_ptr)
		handle_game_error(game_map, "Error\nFailed to create minimap\n");
	
	/* Initialize minimap image data address */
	game_map->minimap_image.image_buffer = 
		mlx_get_data_addr(game_map->minimap_image.mlx_image_ptr,
			&game_map->minimap_image.bits_per_pixel,
			&game_map->minimap_image.line_length,
			&game_map->minimap_image.endianness);
	
	if (!game_map->minimap_image.image_buffer)
		handle_game_error(game_map, "Error\nFailed to initialize minimap data\n");
	
	/* Draw initial minimap content */
	draw_minimap_display(game_map);
}

/* ************************************************************************** */
/*                           MAIN INITIALIZATION FUNCTION                    */
/* ************************************************************************** */

/**
 * @brief Main game engine initialization function
 * 
 * This function orchestrates the complete game initialization process:
 * 1. Initialize game statistics and camera settings
 * 2. Create MLX instance and window
 * 3. Load textures and create display buffer
 * 4. Initialize minimap system
 * 5. Set up event handlers for input and rendering
 * 6. Start the main game loop
 * 
 * The initialization follows a strict order to ensure all dependencies
 * are properly established before starting the game loop.
 * 
 * @param game_map Pointer to the main game structure
 * @return 0 on successful initialization and game loop start
 */
int	initialize_game_engine(t_game_map *game_map)
{
	/* Initialize game settings and camera */
	initialize_game_statistics(game_map);
	
	/* Create MLX instance */
	game_map->mlx_instance = mlx_init();
	if (!game_map->mlx_instance)
		handle_game_error(game_map, "Error\nFailed to initialize MLX\n");
	
	/* Create game window */
	game_map->mlx_window = mlx_new_window(game_map->mlx_instance, 
		game_map->display_width, game_map->display_height, "cub3D");
	if (!game_map->mlx_window)
		handle_game_error(game_map, "Error\nFailed to create game window\n");
	
	/* Initialize all images and textures */
	initialize_mlx_images(game_map);
	
	/* Initialize minimap system */
	initialize_minimap_system(game_map);
	
	/* Set up event handlers */
	mlx_loop_hook(game_map->mlx_instance, &render_frame, game_map);
	mlx_hook(game_map->mlx_window, 2, 1L << 0, handle_keyboard_input, game_map);
	mlx_hook(game_map->mlx_window, 6, 1L << 6, handle_mouse_rotation, game_map);
	mlx_hook(game_map->mlx_window, 17, 1L << 0, clean_exit_program, game_map);
	
	/* Start the main game loop */
	mlx_loop(game_map->mlx_instance);
	
	return (0);
}
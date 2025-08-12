/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:30:20 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 11:48:14 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/* ************************************************************************** */
/*                           FILE VALIDATION                                 */
/* ************************************************************************** */

/**
 * @brief Validates that the file has the correct .cub extension
 * 
 * Checks the last 4 characters of the filename to ensure it ends with ".cub".
 * This function performs a simple string comparison starting from the end
 * of the filename.
 * 
 * @param filename The filename to validate
 * @return 1 if extension is valid (.cub), 0 otherwise
 */
static int	validate_file_extension(char *filename)
{
	int	filename_length;
	int	extension_index;

	filename_length = ft_strlen(filename);
	
	/* Check if filename is long enough to contain ".cub" */
	if (filename_length < 4)
		return (0);
	
	/* Start checking from 4 characters before the end */
	extension_index = 0;
	filename_length -= 4;
	
	/* Validate each character of the extension */
	while (filename[filename_length] != '\0')
	{
		if (extension_index == 0 && filename[filename_length] != '.')
			return (0);
		if (extension_index == 1 && filename[filename_length] != 'c')
			return (0);
		if (extension_index == 2 && filename[filename_length] != 'u')
			return (0);
		if (extension_index == 3 && filename[filename_length] != 'b')
			return (0);
		filename_length++;
		extension_index++;
	}
	return (1);
}

/**
 * @brief Validates that the input file exists and is readable
 * 
 * This function performs comprehensive file validation:
 * 1. Checks file extension (.cub)
 * 2. Attempts to open the file
 * 3. Verifies the file is not empty
 * 4. Ensures the file is readable
 * 
 * @param filename Path to the file to validate
 * @return 1 if file is valid and readable, 0 otherwise
 */
int	validate_input_file(char *filename)
{
	int		file_descriptor;
	int		read_result;
	char	test_buffer[1];

	/* First, validate the file extension */
	if (!validate_file_extension(filename))
	{
		ft_printf("Error\nInvalid file extension. Expected: .cub\n");
		return (0);
	}
	
	/* Attempt to open the file for reading */
	file_descriptor = open(filename, O_RDONLY);
	if (file_descriptor < 0)
	{
		ft_printf("Error\nCannot open file: %s\n", filename);
		return (0);
	}
	
	/* Try to read one byte to check if file is empty or readable */
	read_result = read(file_descriptor, test_buffer, 1);
	close(file_descriptor);
	
	/* Handle different read results */
	if (read_result <= 0)
	{
		if (read_result == 0)
			ft_printf("Error\nFile is empty: %s\n", filename);
		else
			ft_printf("Error\nCannot read file: %s\n", filename);
		return (0);
	}
	
	return (1);
}

/* ************************************************************************** */
/*                         STRUCTURE INITIALIZATION                          */
/* ************************************************************************** */

/**
 * @brief Initializes all members of the game structure to safe default values
 * 
 * This function sets all pointers to NULL and numeric values to appropriate
 * defaults. This prevents undefined behavior and makes debugging easier.
 * RGB values are initialized to -1 to indicate they haven't been set yet.
 * 
 * @param game_map Pointer to the main game structure to initialize
 */
void	initialize_game_structure(t_game_map *game_map)
{
	int	texture_index;

	/* Initialize MLX pointers */
	game_map->mlx_instance = NULL;
	game_map->mlx_window = NULL;
	
	/* Initialize map data */
	game_map->map_grid = NULL;
	game_map->map_width = 0;
	game_map->map_height = 0;
	
	/* Initialize RGB color arrays to -1 (unset indicator) */
	game_map->floor_color_rgb[0] = -1;
	game_map->floor_color_rgb[1] = -1;
	game_map->floor_color_rgb[2] = -1;
	game_map->ceiling_color_rgb[0] = -1;
	game_map->ceiling_color_rgb[1] = -1;
	game_map->ceiling_color_rgb[2] = -1;
	
	/* Initialize parsing buffers */
	game_map->current_line = NULL;
	game_map->map_data_buffer = NULL;
	
	/* Initialize all texture images */
	texture_index = 0;
	while (texture_index < 5)
	{
		game_map->texture_images[texture_index].mlx_image_ptr = NULL;
		game_map->texture_images[texture_index].texture_path = NULL;
		texture_index++;
	}
	#ifdef BONUS
	/* Initialize minimap */
	game_map->minimap_image.mlx_image_ptr = NULL;
	#endif
}

/* ************************************************************************** */
/*                              MAIN FUNCTION                               */
/* ************************************************************************** */

/**
 * @brief Main function - entry point of the program
 * 
 * This function coordinates the entire program execution:
 * 1. Validates command line arguments
 * 2. Initializes the game structure
 * 3. Validates the input file
 * 4. Parses the map configuration
 * 5. Initializes the game engine
 * 6. Starts the main game loop
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, error code on failure
 */
int	main(int argc, char **argv)
{
	t_game_map	game_map;
	int			file_descriptor;

	/* Initialize the game structure to safe defaults */
	initialize_game_structure(&game_map);
	
	/* Validate command line arguments */
	if (argc != 2)
	{
		ft_printf("Error\nUsage: %s <map_file.cub>\n", argv[0]);
		ft_printf("Expected exactly one argument: path to .cub file\n");
		return (1);
	}
	
	/* Validate the input file */
	if (!validate_input_file(argv[1]))
		return (2);
	
	/* Open the validated file for parsing */
	file_descriptor = open(argv[1], O_RDONLY);
	if (file_descriptor < 0)
	{
		ft_printf("Error\nFailed to open file for parsing: %s\n", argv[1]);
		return (3);
	}
	
	/* Parse the map file */
	if (!parse_map_file(&game_map, file_descriptor))
	{
		ft_printf("Error\nMap parsing failed\n");
		free_string_array(game_map.map_grid);
		close(file_descriptor);
		return (4);
	}
	
	/* Close the file descriptor as we're done reading */
	close(file_descriptor);
	
	/* Initialize the game engine (MLX, textures, etc.) */
	if (initialize_game_engine(&game_map) != 0)
	{
		ft_printf("Error\nGame engine initialization failed\n");
		free_string_array(game_map.map_grid);
		return (5);
	}
	
	/* 
	 * If we reach this point, the game should be running
	 * The MLX loop will handle the game execution and the program
	 * will exit through the clean_exit_program function
	 */
	return (0);
}
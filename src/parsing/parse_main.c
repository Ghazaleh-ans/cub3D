/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:45:35 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:45:38 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           MAP VALIDATION                                  */
/* ************************************************************************** */

/**
 * @brief Validate that the parsed map meets all requirements
 * 
 * This function performs comprehensive map validation including:
 * - Wall enclosure verification (map must be surrounded by walls)
 * - Player position validation (exactly one player must exist)
 * - Character validation (only allowed characters in map)
 * 
 * @param game_map Pointer to the main game structure
 * @return 1 if map is valid, 0 if validation fails
 */
static int	validate_parsed_map(t_game_map *game_map)
{
	if (!validate_map_walls(game_map))
	{
		ft_printf("Error\nMap is not properly enclosed by walls\n");
		return (0);
	}
	return (1);
}

/**
 * @brief Handle parsing errors with proper cleanup and messaging
 * 
 * This function provides centralized error handling for all parsing operations.
 * It displays the error message, performs necessary cleanup, and exits the program.
 * The cleanup includes freeing any allocated memory to prevent memory leaks.
 * 
 * @param game_map Pointer to the main game structure
 * @param error_message Descriptive error message to display to user
 */
void	handle_parsing_error(t_game_map *game_map, char *error_message)
{
	/* Display error message to user */
	ft_printf("%s", error_message);
	
	/* Perform comprehensive cleanup */
	if (game_map->map_grid)
		free_string_array(game_map->map_grid);
	if (game_map->current_line)
		free(game_map->current_line);
	if (game_map->map_data_buffer)
		free(game_map->map_data_buffer);
	
	/* Free any texture paths that were allocated */
	free_texture_paths(game_map);
	
	/* Exit with failure status */
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                           MAP SIZE NORMALIZATION                          */
/* ************************************************************************** */

/**
 * @brief Normalize map to rectangular format for easier processing
 * 
 * This function ensures all map lines have the same width by:
 * 1. Calculating map dimensions and finding player position
 * 2. Resizing shorter lines to match the longest line
 * 3. Padding with spaces to form a perfect rectangle
 * 
 * This normalization simplifies boundary checking and rendering algorithms.
 * 
 * @param game_map Pointer to the main game structure
 */
static void	normalize_map_dimensions(t_game_map *game_map)
{
	int	row_index;
	int	col_index;

	row_index = 0;
	col_index = 0;
	
	/* Calculate map dimensions and locate player */
	calculate_map_dimensions(game_map, game_map->map_grid, row_index, col_index);
	
	/* Validate that exactly one player was found */
	if (game_map->map_height == 0 || game_map->map_width == 0)
		handle_parsing_error(game_map, "Error\nMap must contain exactly one player (N/S/E/W)\n");
	
	/* Resize all lines to have uniform width */
	row_index = 0;
	while (game_map->map_grid[row_index])
	{
		if (get_string_length_no_newline(game_map->map_grid[row_index]) < game_map->map_width)
		{
			game_map->map_grid[row_index] = resize_string_to_size(
				game_map->map_grid[row_index], game_map->map_width);
			if (!game_map->map_grid[row_index])
				handle_parsing_error(game_map, "Error\nMemory allocation failed during map normalization\n");
		}
		row_index++;
	}
}

/* ************************************************************************** */
/*                           MAP FILE READING                                */
/* ************************************************************************** */

/**
 * @brief Read and parse the entire map configuration file
 * 
 * This function orchestrates the complete file parsing process:
 * 1. Read file line by line using get_next_line
 * 2. Parse configuration elements (textures, colors)
 * 3. Accumulate map data into a buffer
 * 4. Convert buffer to 2D map array
 * 5. Normalize map dimensions
 * 
 * The function handles both configuration data and map data, switching
 * parsing modes when the map section is encountered.
 * 
 * @param file_descriptor Open file descriptor for the .cub file
 * @param game_map Pointer to the main game structure
 */
void	read_and_parse_map_file(int file_descriptor, t_game_map *game_map)
{
	char	*current_line;
	int		parsing_map_data;

	parsing_map_data = 0;
	
	/* Read file line by line until EOF */
	while (1)
	{
		current_line = get_next_line(file_descriptor);
		if (!current_line)
			break;
		
		/* Convert empty lines to spaces for proper map parsing */
		if (current_line[0] == '\n')
			current_line[0] = ' ';
		
		/* Determine parsing mode based on current content */
		if (!parsing_map_data)
		{
			/* Still parsing configuration elements */
			parsing_map_data = extract_map_statistics(game_map, current_line);
		}
		
		if (parsing_map_data)
		{
			/* Now parsing map data */
			/* Validate that map doesn't contain separator character */
			if (ft_strchr(current_line, '/'))
			{
				free(current_line);
				handle_parsing_error(game_map, "Error\nInvalid character '/' found in map data\n");
			}
			
			/* Accumulate map data with separator */
			game_map->map_data_buffer = join_strings_with_separator(
				game_map->map_data_buffer, current_line);
		}
		
		free(current_line);
	}
	
	/* Convert accumulated map data to 2D array */
	game_map->map_grid = ft_split(game_map->map_data_buffer, '/');
	free(game_map->map_data_buffer);
	game_map->map_data_buffer = NULL;
	
	/* Normalize map to rectangular format */
	normalize_map_dimensions(game_map);
}

/* ************************************************************************** */
/*                           MAIN PARSING FUNCTION                           */
/* ************************************************************************** */

/**
 * @brief Main map file parsing function
 * 
 * This is the primary entry point for map file parsing. It coordinates
 * the entire parsing process and performs final validation.
 * 
 * The parsing process includes:
 * 1. Reading and parsing the configuration file
 * 2. Validating the parsed map structure
 * 3. Ensuring all required elements are present
 * 4. Performing final error checking
 * 
 * @param game_map Pointer to the main game structure
 * @param file_descriptor Open file descriptor for the .cub file
 * @return 1 if parsing successful, 0 if any validation fails
 */
int	parse_map_file(t_game_map *game_map, int file_descriptor)
{
	/* Read and parse the entire file */
	read_and_parse_map_file(file_descriptor, game_map);
	
	/* Validate the parsed map structure */
	if (!validate_parsed_map(game_map))
		return (0);
	
	/* Close file descriptor as we're done reading */
	close(file_descriptor);
	
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_statistics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:46:39 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:46:41 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           PLAYER INITIALIZATION                           */
/* ************************************************************************** */

/**
 * @brief Set player position and initial direction vectors
 * 
 * This function initializes the player's starting state based on the
 * direction character found in the map (N/S/E/W). It sets:
 * - Player position coordinates (converted to floating point)
 * - Direction vector components for movement
 * - Initial facing direction for rendering
 * 
 * The direction vectors are normalized unit vectors:
 * - North: (0, -1) - negative Y because Y increases downward
 * - South: (0, 1)  - positive Y
 * - East:  (1, 0)  - positive X
 * - West:  (-1, 0) - negative X
 * 
 * @param game_map Pointer to the main game structure
 * @param direction Direction character from map (N/S/E/W)
 * @param row Player's row position in the map grid
 * @param col Player's column position in the map grid
 */
void	set_player_position_and_direction(t_game_map *game_map, char direction, 
		int row, int col)
{
	/* Set player position (convert to floating point for smooth movement) */
	game_map->player_pos_y = (double)row;
	game_map->player_pos_x = (double)col;
	game_map->player_direction = direction;
	
	/* Set direction vector based on initial facing direction */
	if (direction == 'N')
	{
		game_map->player_dir_x = 0.0;
		game_map->player_dir_y = -1.0;  /* North is negative Y */
	}
	else if (direction == 'S')
	{
		game_map->player_dir_x = 0.0;
		game_map->player_dir_y = 1.0;   /* South is positive Y */
	}
	else if (direction == 'W')
	{
		game_map->player_dir_x = -1.0;  /* West is negative X */
		game_map->player_dir_y = 0.0;
	}
	else if (direction == 'E')
	{
		game_map->player_dir_x = 1.0;   /* East is positive X */
		game_map->player_dir_y = 0.0;
	}
}

/* ************************************************************************** */
/*                           MAP DIMENSION CALCULATION                       */
/* ************************************************************************** */

/**
 * @brief Calculate map dimensions and locate player position
 * 
 * This function performs a comprehensive analysis of the map array:
 * 1. Iterates through all map cells
 * 2. Finds the maximum width (longest line)
 * 3. Counts the total height (number of lines)
 * 4. Locates player position and direction
 * 5. Validates that exactly one player exists
 * 
 * The function ensures map integrity by checking for:
 * - Exactly one player position marker (N/S/E/W)
 * - Valid map dimensions (non-zero width and height)
 * 
 * @param game_map Pointer to the main game structure
 * @param map_array 2D string array representing the map
 * @param start_row Starting row index (typically 0)
 * @param start_col Starting column index (typically 0)
 */
void	calculate_map_dimensions(t_game_map *game_map, char **map_array, 
		int start_row, int start_col)
{
	int	current_row;
	int	current_col;
	int	max_width;
	int	player_count;

	current_row = start_row;
	max_width = 0;
	player_count = 0;
	
	/* Iterate through all rows of the map */
	while (map_array[current_row])
	{
		current_col = start_col;
		
		/* Check each character in the current row */
		while (map_array[current_row][current_col])
		{
			/* Check if this character is a player position marker */
			if (is_character_valid(map_array[current_row][current_col], "NSEW"))
			{
				set_player_position_and_direction(game_map, 
					map_array[current_row][current_col], current_row, current_col);
				player_count++;
			}
			current_col++;
		}
		
		/* Update maximum width if current row is longer */
		if (current_col > max_width)
			max_width = current_col;
		
		current_row++;
	}
	
	/* Set map dimensions only if exactly one player was found */
	if (player_count == 1)
	{
		game_map->map_width = max_width;
		game_map->map_height = current_row;
	}
	else
	{
		/* Invalid player count - reset dimensions to indicate error */
		game_map->map_width = 0;
		game_map->map_height = 0;
	}
}

/* ************************************************************************** */
/*                           CONFIGURATION VALIDATION                        */
/* ************************************************************************** */

/**
 * @brief Validate that all required configuration elements are present
 * 
 * This function performs final validation to ensure all mandatory
 * configuration elements have been properly parsed:
 * - All four directional textures (North, South, East, West)
 * - Floor color RGB values
 * - Ceiling color RGB values
 * 
 * Missing elements will cause a parsing error with descriptive message.
 * 
 * @param game_map Pointer to the main game structure
 */
static void	validate_configuration_completeness(t_game_map *game_map)
{
	int	color_index;

	/* Check that all texture paths are present */
	if (!game_map->texture_images[0].texture_path || 
		!game_map->texture_images[1].texture_path ||
		!game_map->texture_images[2].texture_path || 
		!game_map->texture_images[3].texture_path)
	{
		handle_parsing_error(game_map, "Error\nMissing texture definitions (NO/SO/WE/EA)\n");
	}
	
	/* Check that all color values are set (initialized to -1) */
	color_index = 0;
	while (color_index < 3)
	{
		if (game_map->floor_color_rgb[color_index] == -1 || 
			game_map->ceiling_color_rgb[color_index] == -1)
		{
			handle_parsing_error(game_map, "Error\nMissing color definitions (F/C)\n");
		}
		color_index++;
	}
}

/* ************************************************************************** */
/*                           MAIN STATISTICS FUNCTION                        */
/* ************************************************************************** */

/**
 * @brief Extract configuration data from a single line
 * 
 * This function processes individual lines from the configuration file,
 * identifying and parsing different types of configuration elements:
 * - Texture definitions (NO, SO, WE, EA)
 * - Color definitions (F, C)
 * - Empty lines (ignored)
 * - Map data detection
 * 
 * The function uses a static counter to track how many configuration
 * elements have been processed. Once all 6 elements are found, it
 * validates completeness and signals that map data parsing should begin.
 * 
 * @param game_map Pointer to the main game structure
 * @param config_line Line from configuration file to process
 * @return 1 if ready to parse map data, 0 if still parsing configuration
 */
int	extract_map_statistics(t_game_map *game_map, char *config_line)
{
	static int	elements_processed = 0;

	/* Continue processing configuration elements until we have all 6 */
	while (elements_processed < 6)
	{
		/* Skip empty lines */
		if (config_line[0] == ' ' && !config_line[1])
			return (0);
		
		/* Parse texture definitions */
		if (!ft_strncmp(config_line, "NO ", 3))
			extract_texture_path(game_map, &game_map->texture_images[0].texture_path, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "SO ", 3))
			extract_texture_path(game_map, &game_map->texture_images[1].texture_path, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "WE ", 3))
			extract_texture_path(game_map, &game_map->texture_images[2].texture_path, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "EA ", 3))
			extract_texture_path(game_map, &game_map->texture_images[3].texture_path, 
				ft_split(config_line, ' '));
		
		/* Parse color definitions */
		else if (!ft_strncmp(config_line, "F ", 2))
			extract_rgb_colors(game_map, game_map->floor_color_rgb, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "C ", 2))
			extract_rgb_colors(game_map, game_map->ceiling_color_rgb, 
				ft_split(config_line, ' '));
		
		/* Unknown configuration element */
		else
			handle_parsing_error(game_map, "Error\nInvalid configuration element\n");
		
		elements_processed++;
		return (0);
	}
	
	/* All configuration elements processed - validate completeness */
	validate_configuration_completeness(game_map);
	
	/* Signal that we're ready to parse map data */
	return (1);
}
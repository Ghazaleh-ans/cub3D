/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:48:01 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:48:03 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           COLUMN VALIDATION                               */
/* ************************************************************************** */

/**
 * @brief Validate a single column of the map for proper wall enclosure
 * 
 * This function checks that a vertical column of the map is properly
 * enclosed by walls. It performs the following validations:
 * 1. Top and bottom boundaries must be walls or spaces
 * 2. All characters must be valid map characters
 * 3. Space regions must be properly enclosed by walls
 * 4. No open spaces can be adjacent to map boundaries
 * 
 * The algorithm handles spaces by ensuring they are enclosed:
 * - Any space must have walls above and below the space region
 * - Transitions from wall to space and back to walkable areas are invalid
 * 
 * @param map_array 2D array representing the complete map
 * @param start_row Starting row for validation (typically 0)
 * @param col_index Column index to validate
 * @param total_rows Total number of rows in the map
 * @return 1 if column is properly enclosed, 0 if validation fails
 */
int	validate_map_column(char **map_array, int start_row, int col_index, int total_rows)
{
	int	current_row;

	current_row = start_row;
	
	/* Validate each cell in the column */
	while (current_row < total_rows)
	{
		/* First row must be wall or space (boundary) */
		if (current_row == 0 && !is_character_valid(map_array[current_row][col_index], "1 "))
			return (0);
		
		/* Validate character types */
		if (!is_character_valid(map_array[current_row][col_index], "10NSEW "))
			return (0);
		
		/* Handle space regions - ensure proper enclosure */
		if (map_array[current_row][col_index] == ' ')
		{
			/* Check that space region is properly bounded above */
			if (current_row > 0 && map_array[current_row - 1][col_index] != '1')
				return (0);
			
			/* Skip through entire space region */
			while (current_row < total_rows && map_array[current_row][col_index] == ' ')
				current_row++;
			
			/* Check that space region is properly bounded below */
			if (current_row < total_rows && map_array[current_row][col_index] != '1')
				return (0);
		}
		
		if (current_row < total_rows)
			current_row++;
	}
	
	/* Last row must be wall or space (boundary) */
	if (!is_character_valid(map_array[current_row - 1][col_index], "1 "))
		return (0);
	
	return (1);
}

/**
 * @brief Validate all vertical columns of the map
 * 
 * This function iterates through every column of the map and validates
 * that each column is properly enclosed by walls. It ensures that the
 * entire map boundary is secure and no walkable areas extend to the edges.
 * 
 * @param game_map Pointer to the main game structure
 * @param start_row Starting row for validation (typically 0)
 * @param start_col Starting column for validation (typically 0)
 * @return 1 if all columns are valid, 0 if any column fails validation
 */
int	validate_vertical_walls(t_game_map *game_map, int start_row, int start_col)
{
	int	current_col;

	current_col = start_col;
	
	/* Check each column of the map */
	while (current_col < game_map->map_width)
	{
		if (!validate_map_column(game_map->map_grid, start_row, current_col, game_map->map_height))
			return (0);
		current_col++;
	}
	
	return (1);
}

/* ************************************************************************** */
/*                             ROW VALIDATION                                */
/* ************************************************************************** */

/**
 * @brief Validate a single row of the map for proper wall enclosure
 * 
 * This function checks that a horizontal row of the map is properly
 * enclosed by walls. It performs similar validation to column checking:
 * 1. Left and right boundaries must be walls or spaces
 * 2. All characters must be valid map characters
 * 3. Space regions must be properly enclosed by walls
 * 4. Invalid characters trigger parsing errors
 * 
 * @param game_map Pointer to the main game structure (for error handling)
 * @param line_to_check String representing the map row to validate
 * @param start_col Starting column index for validation
 * @return 1 if row is properly enclosed, 0 if validation fails
 */
int	validate_map_line(t_game_map *game_map, char *line_to_check, int start_col)
{
	int	current_col;

	current_col = start_col;
	
	/* Validate each character in the row */
	while (line_to_check[current_col])
	{
		/* First column must be wall or space (boundary) */
		if (current_col == 0 && !is_character_valid(line_to_check[current_col], "1 "))
			return (0);
		
		/* Validate character types - invalid characters cause parsing error */
		if (!is_character_valid(line_to_check[current_col], "10NSEW "))
			handle_parsing_error(game_map, "Error\nInvalid character found in map\n");
		
		/* Handle space regions - ensure proper enclosure */
		if (line_to_check[current_col] == ' ')
		{
			/* Check that space region is properly bounded to the left */
			if (current_col > 0 && line_to_check[current_col - 1] != '1')
				return (0);
			
			/* Skip through entire space region */
			while (line_to_check[current_col] && line_to_check[current_col] == ' ')
				current_col++;
			
			/* Check that space region is properly bounded to the right */
			if (line_to_check[current_col] && line_to_check[current_col] != '1')
				return (0);
		}
		
		if (line_to_check[current_col])
			current_col++;
	}
	
	/* Last column must be wall or space (boundary) */
	if (!is_character_valid(line_to_check[current_col - 1], "1 "))
		return (0);
	
	return (1);
}

/**
 * @brief Validate all horizontal rows of the map
 * 
 * This function iterates through every row of the map and validates
 * that each row is properly enclosed by walls. Combined with vertical
 * validation, this ensures complete map boundary security.
 * 
 * @param game_map Pointer to the main game structure
 * @param start_row Starting row for validation (typically 0)
 * @param start_col Starting column for validation (typically 0)
 * @return 1 if all rows are valid, 0 if any row fails validation
 */
int	validate_horizontal_walls(t_game_map *game_map, int start_row, int start_col)
{
	int	current_row;

	current_row = start_row;
	
	/* Check each row of the map */
	while (game_map->map_grid[current_row])
	{
		if (!validate_map_line(game_map, game_map->map_grid[current_row], start_col))
			return (0);
		current_row++;
	}
	
	return (1);
}

/* ************************************************************************** */
/*                           MAIN WALL VALIDATION                            */
/* ************************************************************************** */

/**
 * @brief Comprehensive map wall validation function
 * 
 * This is the main entry point for map validation. It orchestrates
 * both horizontal and vertical wall validation to ensure the map
 * is completely enclosed and safe for gameplay.
 * 
 * The validation process includes:
 * 1. Horizontal boundary validation (top/bottom walls)
 * 2. Vertical boundary validation (left/right walls)
 * 3. Internal space region validation
 * 4. Character validity checking
 * 
 * If validation fails, texture paths are freed to prevent memory leaks
 * before returning the error status.
 * 
 * @param game_map Pointer to the main game structure
 * @return 1 if map is completely valid, 0 if any validation fails
 */
int	validate_map_walls(t_game_map *game_map)
{
	int	start_row;
	int	start_col;

	start_row = 0;
	start_col = 0;
	
	/* Perform comprehensive wall validation */
	if (!validate_horizontal_walls(game_map, start_row, start_col) || 
		!validate_vertical_walls(game_map, start_row, start_col))
	{
		/* Validation failed - clean up texture resources */
		free_texture_paths(game_map);
		return (0);
	}
	
	return (1);
}
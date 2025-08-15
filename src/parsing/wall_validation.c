#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           COLUMN VALIDATION                               */
/* ************************************************************************** */

/**
 * @brief Validate a single column of the map for proper wall enclosure
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
 */
int	validate_vertical_walls(t_game *game, int start_row, int start_col)
{
	int	current_col;

	current_col = start_col;
	
	/* Check each column of the map */
	while (current_col < game->map.width)
	{
		if (!validate_map_column(game->map.grid, start_row, current_col, game->map.height))
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
 */
int	validate_map_line(t_game *game, char *line_to_check, int start_col)
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
			handle_parsing_error(game, "Error\nInvalid character found in map\n");
		
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
 */
int	validate_horizontal_walls(t_game *game, int start_row, int start_col)
{
	int	current_row;

	current_row = start_row;
	
	/* Check each row of the map */
	while (game->map.grid[current_row])
	{
		if (!validate_map_line(game, game->map.grid[current_row], start_col))
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
 */
int	validate_map_walls(t_game *game)
{
	int	start_row;
	int	start_col;

	start_row = 0;
	start_col = 0;
	
	/* Perform comprehensive wall validation */
	if (!validate_horizontal_walls(game, start_row, start_col) || 
		!validate_vertical_walls(game, start_row, start_col))
	{
		/* Validation failed - clean up texture resources */
		free_texture_paths(game);
		return (0);
	}
	
	return (1);
}

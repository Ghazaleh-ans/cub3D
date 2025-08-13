#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           MAP VALIDATION                                  */
/* ************************************************************************** */

/**
 * @brief Validate that the parsed map meets all requirements
 */
static int	validate_parsed_map(t_game *game)
{
	if (!validate_map_walls(game))
	{
		ft_printf("Error\nMap is not properly enclosed by walls\n");
		return (0);
	}
	return (1);
}

/**
 * @brief Handle parsing errors with proper cleanup and messaging
 */
void	handle_parsing_error(t_game *game, char *error_message)
{
	/* Display error message to user */
	ft_printf("%s", error_message);
	
	/* Perform comprehensive cleanup */
	if (game->map.grid)
		free_string_array(game->map.grid);
	if (game->map.current_line)
		free(game->map.current_line);
	if (game->map.data_buffer)
		free(game->map.data_buffer);
	
	/* Free any texture paths that were allocated */
	free_texture_paths(game);
	
	/* Exit with failure status */
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                           MAP SIZE NORMALIZATION                          */
/* ************************************************************************** */

/**
 * @brief Normalize map to rectangular format for easier processing
 */
static void	normalize_map_dimensions(t_game *game)
{
	int	row_index;

	row_index = 0;
	
	/* Calculate map dimensions and locate player */
	calculate_map_dimensions(game, game->map.grid, row_index, row_index);
	
	/* Validate that exactly one player was found */
	if (game->map.height == 0 || game->map.width == 0)
		handle_parsing_error(game, "Error\nMap must contain exactly one player (N/S/E/W)\n");
	
	/* Resize all lines to have uniform width */
	row_index = 0;
	while (game->map.grid[row_index])
	{
		if (get_string_length_no_newline(game->map.grid[row_index]) < game->map.width)
		{
			game->map.grid[row_index] = resize_string_to_size(
				game->map.grid[row_index], game->map.width);
			if (!game->map.grid[row_index])
				handle_parsing_error(game, "Error\nMemory allocation failed during map normalization\n");
		}
		row_index++;
	}
}

/* ************************************************************************** */
/*                           MAP FILE READING                                */
/* ************************************************************************** */

/**
 * @brief Read and parse the entire map configuration file
 */
void	read_and_parse_map_file(int file_descriptor, t_game *game)
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
			parsing_map_data = extract_map_statistics(game, current_line);
		}
		
		if (parsing_map_data)
		{
			/* Now parsing map data */
			/* Validate that map doesn't contain separator character */
			if (ft_strchr(current_line, '/'))
			{
				free(current_line);
				handle_parsing_error(game, "Error\nInvalid character '/' found in map data\n");
			}
			
			/* Accumulate map data with separator */
			game->map.data_buffer = join_strings_with_separator(
				game->map.data_buffer, current_line);
		}
		
		free(current_line);
	}
	
	/* Convert accumulated map data to 2D array */
	game->map.grid = ft_split(game->map.data_buffer, '/');
	free(game->map.data_buffer);
	game->map.data_buffer = NULL;
	
	/* Normalize map to rectangular format */
	normalize_map_dimensions(game);
}

/* ************************************************************************** */
/*                           MAIN PARSING FUNCTION                           */
/* ************************************************************************** */

/**
 * @brief Main map file parsing function
 */
int	parse_map_file(t_game *game, int file_descriptor)
{
	/* Read and parse the entire file */
	read_and_parse_map_file(file_descriptor, game);
	
	/* Validate the parsed map structure */
	if (!validate_parsed_map(game))
		return (0);
	
	/* Close file descriptor as we're done reading */
	close(file_descriptor);
	
	return (1);
}
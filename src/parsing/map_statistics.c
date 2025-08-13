#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           PLAYER INITIALIZATION                           */
/* ************************************************************************** */

/**
 * @brief Set player position and initial direction vectors
 */
void	set_player_position_and_direction(t_game *game, char direction, 
		int row, int col)
{
	/* Set player position (convert to floating point for smooth movement) */
	game->player.pos_y = (double)row;
	game->player.pos_x = (double)col;
	game->player.initial_dir = direction;
	
	/* Set direction vector based on initial facing direction */
	if (direction == 'N')
	{
		game->player.dir_x = 0.0;
		game->player.dir_y = -1.0;  /* North is negative Y */
	}
	else if (direction == 'S')
	{
		game->player.dir_x = 0.0;
		game->player.dir_y = 1.0;   /* South is positive Y */
	}
	else if (direction == 'W')
	{
		game->player.dir_x = -1.0;  /* West is negative X */
		game->player.dir_y = 0.0;
	}
	else if (direction == 'E')
	{
		game->player.dir_x = 1.0;   /* East is positive X */
		game->player.dir_y = 0.0;
	}
}

/* ************************************************************************** */
/*                           MAP DIMENSION CALCULATION                       */
/* ************************************************************************** */

/**
 * @brief Calculate map dimensions and locate player position
 */
void	calculate_map_dimensions(t_game *game, char **map_array, 
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
				set_player_position_and_direction(game, 
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
		game->map.width = max_width;
		game->map.height = current_row;
	}
	else
	{
		/* Invalid player count - reset dimensions to indicate error */
		game->map.width = 0;
		game->map.height = 0;
	}
}

/* ************************************************************************** */
/*                           CONFIGURATION VALIDATION                        */
/* ************************************************************************** */

/**
 * @brief Validate that all required configuration elements are present
 */
static void	validate_configuration_completeness(t_game *game)
{
	int	color_index;

	/* Check that all texture paths are present */
	if (!game->textures.north.path || !game->textures.south.path ||
		!game->textures.west.path || !game->textures.east.path)
	{
		handle_parsing_error(game, "Error\nMissing texture definitions (NO/SO/WE/EA)\n");
	}
	
	/* Check that all color values are set (initialized to -1) */
	color_index = 0;
	while (color_index < 3)
	{
		if (game->map.floor_rgb[color_index] == -1 || 
			game->map.ceiling_rgb[color_index] == -1)
		{
			handle_parsing_error(game, "Error\nMissing color definitions (F/C)\n");
		}
		color_index++;
	}
}

/* ************************************************************************** */
/*                           MAIN STATISTICS FUNCTION                        */
/* ************************************************************************** */

/**
 * @brief Extract configuration data from a single line
 */
int	extract_map_statistics(t_game *game, char *config_line)
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
			extract_texture_path(game, &game->textures.north.path, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "SO ", 3))
			extract_texture_path(game, &game->textures.south.path, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "WE ", 3))
			extract_texture_path(game, &game->textures.west.path, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "EA ", 3))
			extract_texture_path(game, &game->textures.east.path, 
				ft_split(config_line, ' '));
		
		/* Parse color definitions */
		else if (!ft_strncmp(config_line, "F ", 2))
			extract_rgb_colors(game, game->map.floor_rgb, 
				ft_split(config_line, ' '));
		else if (!ft_strncmp(config_line, "C ", 2))
			extract_rgb_colors(game, game->map.ceiling_rgb, 
				ft_split(config_line, ' '));
		
		/* Unknown configuration element */
		else
			handle_parsing_error(game, "Error\nInvalid configuration element\n");
		
		elements_processed++;
		return (0);
	}
	
	/* All configuration elements processed - validate completeness */
	validate_configuration_completeness(game);
	
	/* Signal that we're ready to parse map data */
	return (1);
}

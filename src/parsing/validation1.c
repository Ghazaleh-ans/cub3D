/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mukibrok <mukibrok@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 01:39:51 by mukibrok          #+#    #+#             */
/*   Updated: 2025/08/14 10:23:27 by mukibrok         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Cub3D.h"

/**
 * Validate the player position in the map
 * Checks if there is exactly one player in the map and sets the player's position and direction
 * @param map Pointer to the game structure containing the map
 * @return true if the player is valid, false otherwise
 */

bool	validate_player(t_game *map)
{
	int	i;
	int	j;
	int	player_count;

	i = -1;
	player_count = 0;
	while (++i < map->map_height && map->map[i])
	{
		j = 0;
		while (map->map[i][j])
		{
			if (map->map[i][j] == NORTH || map->map[i][j] == SOUTH ||
				map->map[i][j] == EAST || map->map[i][j] == WEST)
			{
				player_count++;
				if (player_count > 1)
					return (print_error(ERR_MULTI_PLAYER), false);
				map->player.x = j + 0.5;
				map->player.y = i + 0.5;
				map->player.direction = map->map[i][j];
			}
			j++;
		}
	}
	if (player_count == 0)
		return (print_error(ERR_NO_PLAYER), false);
	return (true);
}

/**
 * Validate the map structure
 * Checks if the map is rectangular, has valid characters, and is closed
 * @param game Pointer to the game structure containing the map
 * @return true if the map is valid, false otherwise
 */

bool	validate_map_walls(t_game *game)
{
	char	**temp_map;
	bool		result;

	temp_map = create_temp_map(game);
	if (!temp_map)
		return (false);
	result = flood_fill(temp_map, (int)game->player.y, (int)game->player.x, game);
	free_double_ptr(temp_map);
	if (!result)
		return (print_error(ERR_MAP_NOT_CLOSED), false);
	return (true);
}

/**
 * Create a temporary copy of the map for flood fill
 * Normalize all lines to the same width (pad with spaces)
 */

char	**create_temp_map(t_game *game)
{
	char	**temp_map;
	int		i = 0;
	int		j;

	temp_map = malloc(sizeof(char*) * (game->map_height + 1));
	if (!temp_map)
		return (NULL);
	while (i < game->map_height)
	{
		temp_map[i] = malloc(sizeof(char) * (game->map_width + 1));
		if (!temp_map[i])
		{
			while (--i >= 0)
				free(temp_map[i]);
			free(temp_map);
			return (NULL);
		}
		j = 0;
		while (j < game->map_width)
		{
			if (j < (int)ft_strlen(game->map[i]) && game->map[i][j] != '\n')
				temp_map[i][j] = game->map[i][j];
			else
				temp_map[i][j] = SPACE;
			j++;
		}
		temp_map[i][game->map_width] = '\0';
		i++;
	}
	temp_map[game->map_height] = NULL;
	return (temp_map);
}

/**
 * Flood fill algorithm to check wall closure
 * Returns 0 if any empty space reaches the border
 */

int	flood_fill(char **map, int y, int x, t_game *game)
{
	/* Check bounds */
	if (y < 0 || y >= game->map_height || x < 0 || x >= game->map_width)
		return (0); /* Reached border - map not closed */

	/* If we hit a wall or already visited, stop */
	if (map[y][x] == WALL || map[y][x] == 'X')
		return (1);

	/* If we hit a space at the border, map is not closed */
	if (map[y][x] == SPACE && (y == 0 || y == game->map_height - 1 || 
		x == 0 || x == game->map_width - 1))
		return (0);

	/* Mark as visited */
	map[y][x] = 'X';

	/* Recursively check all 4 directions */
	if (!flood_fill(map, y - 1, x, game) ||	/* Up */
		!flood_fill(map, y + 1, x, game) ||	/* Down */
		!flood_fill(map, y, x - 1, game) ||	/* Left */
		!flood_fill(map, y, x + 1, game))		/* Right */
		return (0);
	return (1);
}
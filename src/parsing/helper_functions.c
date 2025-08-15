/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 00:32:21 by mukibrok          #+#    #+#             */
/*   Updated: 2025/08/15 16:51:18 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Initialize the game map structure
 * Updated to work with hybrid structure
 * @param map Pointer to the game structure to be initialized
 */

void	init_map(t_game *map)
{
	int	i;

	i = 0;
	map->map = NULL;
	map->map_width = 0;
	map->map_height = 0;
	
	/* Initialize texture paths array (new parsing system) */
	while (i < TEX_COUNT)
	{
		map->textures_paths[i] = NULL;
		i++;
	}
	
	/* Initialize color structures */
	map->floor_color = (t_color){-1, -1, -1};
	map->ceiling_color = (t_color){-1, -1, -1};
	
	/* Initialize player data */
	map->player.x = 0.0;
	map->player.y = 0.0;
	map->player.direction = 0;
}

/**
 * Count the number of elements in a double pointer array
 * @param split Double pointer array to count
 * @return Number of elements in the array
 */

int	split_len(char **split)
{
	int	count;

	count = 0;
	while (split[count])
		count++;
	return (count);
}

/**
 * Count the number of spaces in a line
 * @param line Pointer to the string to count spaces in
 * @return Number of spaces in the string
 */

int	space_count(char *line)
{
	int	count;

	count = 1;
	while (*line)
	{
		if (ft_isspace(*line))
			count++;
		line++;
	}
	return (count);
}

/**
 * Free a double pointer array
 * @param ptr Double pointer array to free
 * @return EXIT_SUCCESS if successful, EXIT_FAILURE if ptr is NULL
 */

int	free_double_ptr(char **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (EXIT_FAILURE);
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (EXIT_SUCCESS);
}

/**
 * Print the map and its properties
 * Updated to work with hybrid structure
 * @param map Pointer to the game structure containing the map and properties
 */

void	print_map(t_game *map)
{
	int i;
	
	if (!map || !map->map)
	{
		printf("Map is not initialized.\n");
		return;
	}
	
	/* Print texture paths using new structure */
	printf("NO: %s\n", map->textures_paths[NORTH_TEX] ? map->textures_paths[NORTH_TEX] : "None");
	printf("SO: %s\n", map->textures_paths[SOUTH_TEX] ? map->textures_paths[SOUTH_TEX] : "None");
	printf("WE: %s\n", map->textures_paths[WEST_TEX] ? map->textures_paths[WEST_TEX] : "None");
	printf("EA: %s\n", map->textures_paths[EAST_TEX] ? map->textures_paths[EAST_TEX] : "None");
	
	/* Print colors using new structure */
	printf("Floor Color: R=%d, G=%d, B=%d\n",
		map->floor_color.red, map->floor_color.green, map->floor_color.blue);
	printf("Ceiling Color: R=%d, G=%d, B=%d\n",
		map->ceiling_color.red, map->ceiling_color.green, map->ceiling_color.blue);
		
	printf("Map dimensions: %d x %d\n", map->map_width, map->map_height);
	printf("Player position: (%.1f, %.1f) facing %c\n", 
		map->player.x, map->player.y, map->player.direction);
	printf("Map contents:\n");
	
	for (i = 0; i < map->map_height; i++)
	{
		if (map->map[i])
			printf("%s\n", map->map[i]);
	}
}
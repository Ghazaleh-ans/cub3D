/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 22:46:12 by mukibrok          #+#    #+#             */
/*   Updated: 2025/08/15 16:50:52 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Free the allocated memory for the map and textures
 * @param map Pointer to the game structure containing the map and textures
 * Updated to work with hybrid structure (uses textures_paths[] instead of textures[])
 */

void	cleanup_map(t_game *map)
{
	int i;
	
	if (!map)
		return ;
	
	/* Free the map array using new parsing system */
	if (map->map)
		free_double_ptr(map->map);
	
	/* Free texture paths from new parsing system */
	i = 0;
	while (i < TEX_COUNT)
	{
		if (map->textures_paths[i])
		{
			free(map->textures_paths[i]);
			map->textures_paths[i] = NULL;
		}
		i++;
	}
	
	/* Also free rendering system texture paths if they exist */
	if (map->textures.north.path)
	{
		free(map->textures.north.path);
		map->textures.north.path = NULL;
	}
	if (map->textures.south.path)
	{
		free(map->textures.south.path);
		map->textures.south.path = NULL;
	}
	if (map->textures.east.path)
	{
		free(map->textures.east.path);
		map->textures.east.path = NULL;
	}
	if (map->textures.west.path)
	{
		free(map->textures.west.path);
		map->textures.west.path = NULL;
	}
}
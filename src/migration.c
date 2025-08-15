/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   migration.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 13:00:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 17:02:50 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * @brief Texture index conversion helper
 */
int	get_texture_index_from_direction(char direction)
{
	if (direction == 'N')
		return (NORTH_DIR);
	else if (direction == 'S')
		return (SOUTH_DIR);
	else if (direction == 'E')
		return (EAST_DIR);
	else if (direction == 'W')
		return (WEST_DIR);
	return (NORTH_DIR);
}

/**
 * @brief Character validation helper (compatible with both systems)
 */
int	is_character_valid(char character, char *valid_characters)
{
	int	index;

	index = 0;
	while (valid_characters[index])
	{
		if (character == valid_characters[index])
			return (1);
		index++;
	}
	return (0);
}
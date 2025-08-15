/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:15:48 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 16:56:51 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	put_pixel_to_image(t_game *game, int x, int y, int color)
{
	char	*pixel_address;

	if (y < 0 || y >= game->mlx.height || x < 0 || x >= game->mlx.width)
		return ;
	pixel_address = game->textures.screen.data + 
		(y * game->textures.screen.line_length + 
		x * (game->textures.screen.bits_per_pixel / 8));
	*(int *)pixel_address = color;
}

int	create_trgb_color(int transparency, int red, int green, int blue)
{
	return (transparency << 24 | red << 16 | green << 8 | blue);
}

/**
 * @brief Render background colors (updated for new color system)
 * 
 * This function now uses the t_color structs from the new parsing system
 * instead of the old int arrays.
 */
void	render_background_colors(t_game *game)
{
	int	screen_x;
	int	screen_y;
	int	ceiling_color;
	int	floor_color;

	/* Convert t_color structs to TRGB format for rendering */
	ceiling_color = create_trgb_color(0, game->ceiling_color.red,
		game->ceiling_color.green, game->ceiling_color.blue);
	floor_color = create_trgb_color(0, game->floor_color.red,
		game->floor_color.green, game->floor_color.blue);
		
	screen_x = 0;
	while (screen_x < game->mlx.width)
	{
		screen_y = 0;
		while (screen_y < game->mlx.height)
		{
			if (screen_y < game->mlx.height / 2)
				put_pixel_to_image(game, screen_x, screen_y, ceiling_color);
			else
				put_pixel_to_image(game, screen_x, screen_y, floor_color);
			screen_y++;
		}
		screen_x++;
	}
}

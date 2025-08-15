/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:53:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 16:57:18 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#ifdef BONUS

/**
 * @brief Draw minimap display (adapted for new map format)
 */
void	draw_minimap_display(t_game *game)
{
	int	row_index;
	int	col_index;
	int	cell_color;

	row_index = 0;
	while (row_index < game->map_height && game->map[row_index])
	{
		col_index = 0;
		while (col_index < game->map_width && game->map[row_index][col_index])
		{
			if (game->map[row_index][col_index] == '1')
				cell_color = COLOR_BLACK;
			else if (game->map[row_index][col_index] == '0')
			{
				/* Use new t_color struct format */
				cell_color = create_trgb_color(0, game->ceiling_color.red,
					game->ceiling_color.green, game->ceiling_color.blue);
			}
			else if (game->map[row_index][col_index] == 'N' || 
					 game->map[row_index][col_index] == 'S' ||
					 game->map[row_index][col_index] == 'E' ||
					 game->map[row_index][col_index] == 'W')
				cell_color = COLOR_PLAYER;
			else
				cell_color = COLOR_WALL;
			draw_minimap_case(game, col_index * MINIMAP_SCALE, 
				row_index * MINIMAP_SCALE, cell_color);
			col_index++;
		}
		row_index++;
	}
}

void	update_minimap_player_position(t_game *game, int previous_x, int previous_y)
{
	int	background_color;

	draw_minimap_case(game, (int)game->player.pos_x * MINIMAP_SCALE, 
		(int)game->player.pos_y * MINIMAP_SCALE, COLOR_PLAYER);
		
	if ((int)game->player.pos_x != previous_x || (int)game->player.pos_y != previous_y)
	{
		if (game->map[previous_y][previous_x] == '1')
			background_color = COLOR_BLACK;
		else
		{
			/* Use new t_color struct format */
			background_color = create_trgb_color(0, game->ceiling_color.red,
				game->ceiling_color.green, game->ceiling_color.blue);
		}
		
		draw_minimap_case(game, previous_x * MINIMAP_SCALE, 
			previous_y * MINIMAP_SCALE, background_color);
	}
}

void	put_pixel_to_minimap(t_game *game, int x, int y, int color)
{
	char	*pixel_address;

	if (y < 0 || y >= game->mlx.height || x < 0 || x >= game->mlx.width)
		return ;	
	pixel_address = game->textures.minimap.data + 
		(y * game->textures.minimap.line_length + 
		x * (game->textures.minimap.bits_per_pixel / 8));
	*(int *)pixel_address = color;
}

void	draw_minimap_case(t_game *game, int start_x, int start_y, int color)
{
	int	pixel_row;
	int	pixel_col;

	pixel_row = 0;
	while (pixel_row < MINIMAP_SCALE)
	{
		pixel_col = 0;
		while (pixel_col < MINIMAP_SCALE)
		{
			put_pixel_to_minimap(game, start_x + pixel_col, 
				start_y + pixel_row, color);
			pixel_col++;
		}
		pixel_row++;
	}
}

/**
 * @brief Initialize minimap system (adapted for new map format)
 */
void	init_minimap_system(t_game *game)
{
	int	minimap_width;
	int	minimap_height;

	minimap_width = game->map_width * MINIMAP_SCALE;
	minimap_height = game->map_height * MINIMAP_SCALE;
	
	game->textures.minimap.mlx_ptr = mlx_new_image(game->mlx.instance,
		minimap_width, minimap_height);
	if (!game->textures.minimap.mlx_ptr)
		handle_game_error(game, "Error\nFailed to create minimap\n");
		
	game->textures.minimap.data = mlx_get_data_addr(game->textures.minimap.mlx_ptr,
		&game->textures.minimap.bits_per_pixel, &game->textures.minimap.line_length,
		&game->textures.minimap.endian);
	if (!game->textures.minimap.data)
		handle_game_error(game, "Error\nFailed to initialize minimap data\n");
		
	draw_minimap_display(game);
}

#endif
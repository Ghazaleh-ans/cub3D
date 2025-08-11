/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:53:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:53:02 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           MINIMAP RENDERING                               */
/* ************************************************************************** */

/**
 * @brief Draw the complete minimap display
 * 
 * This function renders the entire minimap by iterating through the
 * map grid and drawing colored squares for each cell type:
 * - Walls ('1'): Black squares
 * - Empty spaces ('0'): Ceiling color squares
 * - Player position (N/S/E/W): Bright yellow squares
 * - Unknown/space characters: Dark gray squares
 * 
 * Each map cell is scaled up by MINIMAP_SCALE factor to make it visible.
 * The minimap provides a top-down view of the entire level layout.
 * 
 * @param game_map Pointer to the main game structure
 */
void	draw_minimap_display(t_game_map *game_map)
{
	int	row_index;
	int	col_index;
	int	cell_color;

	row_index = 0;
	
	/* Iterate through all map rows */
	while (game_map->map_grid[row_index])
	{
		col_index = 0;
		
		/* Iterate through all columns in current row */
		while (game_map->map_grid[row_index][col_index])
		{
			/* Determine color based on cell type */
			if (game_map->map_grid[row_index][col_index] == '1')
			{
				/* Wall cells - black */
				cell_color = COLOR_BLACK;
			}
			else if (game_map->map_grid[row_index][col_index] == '0')
			{
				/* Empty space - ceiling color */
				cell_color = create_trgb_color(0, game_map->ceiling_color_rgb[0],
					game_map->ceiling_color_rgb[1], game_map->ceiling_color_rgb[2]);
			}
			else if (is_character_valid(game_map->map_grid[row_index][col_index], "NSEW"))
			{
				/* Player position - bright yellow */
				cell_color = COLOR_PLAYER;
			}
			else
			{
				/* Unknown/space characters - dark gray */
				cell_color = COLOR_WALL;
			}
			
			/* Draw the scaled cell */
			draw_minimap_case(game_map, col_index * MINIMAP_SCALE, 
				row_index * MINIMAP_SCALE, cell_color);
			
			col_index++;
		}
		row_index++;
	}
}

/**
 * @brief Update minimap when player moves to a different grid cell
 * 
 * This function efficiently updates the minimap display when the player
 * moves from one grid cell to another:
 * 1. Draw player marker at new position
 * 2. Clear player marker from previous position (if different)
 * 3. Restore appropriate background color at old position
 * 
 * This selective updating is more efficient than redrawing the entire
 * minimap every frame, improving performance.
 * 
 * @param game_map Pointer to the main game structure
 * @param previous_x Previous X grid position
 * @param previous_y Previous Y grid position
 */
void	update_minimap_player_position(t_game_map *game_map, int previous_x, int previous_y)
{
	int	background_color;

	/* Draw player at new position */
	draw_minimap_case(game_map, (int)game_map->player_pos_x * MINIMAP_SCALE, 
		(int)game_map->player_pos_y * MINIMAP_SCALE, COLOR_PLAYER);
	
	/* Clear old position if player moved to different grid cell */
	if ((int)game_map->player_pos_x != previous_x || (int)game_map->player_pos_y != previous_y)
	{
		/* Restore background color based on map cell type */
		if (game_map->map_grid[previous_y][previous_x] == '1')
			background_color = COLOR_BLACK;
		else
			background_color = create_trgb_color(0, game_map->ceiling_color_rgb[0],
				game_map->ceiling_color_rgb[1], game_map->ceiling_color_rgb[2]);
		
		draw_minimap_case(game_map, previous_x * MINIMAP_SCALE, 
			previous_y * MINIMAP_SCALE, background_color);
	}
}

/* ************************************************************************** */
/*                           MINIMAP PIXEL OPERATIONS                        */
/* ************************************************************************** */

/**
 * @brief Put a pixel to the minimap image with bounds checking
 * 
 * This function safely writes a color value to a specific pixel in the
 * minimap image buffer. It includes bounds checking to prevent buffer
 * overflows and segmentation faults.
 * 
 * @param game_map Pointer to the main game structure
 * @param x X coordinate in minimap image
 * @param y Y coordinate in minimap image  
 * @param color ARGB color value to write
 */
void	put_pixel_to_minimap(t_game_map *game_map, int x, int y, int color)
{
	char	*pixel_address;

	/* Bounds checking to prevent buffer overflow */
	if (y < 0 || y >= game_map->display_height || x < 0 || x >= game_map->display_width)
		return ;
	
	/* Calculate pixel address in minimap image buffer */
	pixel_address = game_map->minimap_image.image_buffer + 
		(y * game_map->minimap_image.line_length + 
		x * (game_map->minimap_image.bits_per_pixel / 8));
	
	/* Write color value to pixel */
	*(int *)pixel_address = color;
}

/**
 * @brief Draw a single case/cell on the minimap
 * 
 * This function draws a filled square representing one map cell on the minimap.
 * The square size is determined by MINIMAP_SCALE, creating a visible
 * representation of each map grid cell.
 * 
 * @param game_map Pointer to the main game structure
 * @param start_x Starting X coordinate for the square
 * @param start_y Starting Y coordinate for the square
 * @param color Color to fill the square
 */
void	draw_minimap_case(t_game_map *game_map, int start_x, int start_y, int color)
{
	int	pixel_row;
	int	pixel_col;

	pixel_row = 0;
	
	/* Draw each row of the square */
	while (pixel_row < MINIMAP_SCALE)
	{
		pixel_col = 0;
		
		/* Draw each column of the square */
		while (pixel_col < MINIMAP_SCALE)
		{
			put_pixel_to_minimap(game_map, start_x + pixel_col, 
				start_y + pixel_row, color);
			pixel_col++;
		}
		pixel_row++;
	}
}
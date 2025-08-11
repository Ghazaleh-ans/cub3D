/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:15:48 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:15:51 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           PIXEL MANIPULATION                              */
/* ************************************************************************** */

/**
 * @brief Safely put a pixel to the main display image
 * 
 * This function writes a color value to a specific pixel in the image buffer.
 * It includes bounds checking to prevent segmentation faults and buffer overflows.
 * The pixel is written directly to the MLX image buffer for maximum performance.
 * 
 * @param game_map Pointer to the main game structure
 * @param x X coordinate of the pixel (0 to display_width-1)
 * @param y Y coordinate of the pixel (0 to display_height-1)
 * @param color ARGB color value to write
 */
void	put_pixel_to_image(t_game_map *game_map, int x, int y, int color)
{
	char	*pixel_address;

	/* Bounds checking to prevent buffer overflow */
	if (y < 0 || y >= game_map->display_height || x < 0 || x >= game_map->display_width)
		return ;
	
	/* Calculate pixel address in the image buffer */
	pixel_address = game_map->texture_images[4].image_buffer + 
		(y * game_map->texture_images[4].line_length + 
		x * (game_map->texture_images[4].bits_per_pixel / 8));
	
	/* Write the color value to the pixel */
	*(int *)pixel_address = color;
}

/* ************************************************************************** */
/*                          WALL HEIGHT CALCULATION                          */
/* ************************************************************************** */

/**
 * @brief Calculate wall line parameters for screen rendering
 * 
 * This function calculates how tall the wall should appear on screen
 * based on the distance to the wall. The further the wall, the shorter
 * it appears (perspective projection).
 * 
 * The calculation also determines the start and end Y coordinates
 * for drawing the wall, with proper clamping to screen boundaries.
 * 
 * @param game_map Pointer to the main game structure
 * @param line_height Pointer to store calculated line height
 * @param start_y Pointer to store starting Y coordinate
 * @param end_y Pointer to store ending Y coordinate
 */
static void	calculate_wall_line_parameters(t_game_map *game_map, int *line_height, 
		int *start_y, int *end_y)
{
	/* Calculate wall height based on distance (perspective projection) */
	*line_height = (int)(game_map->display_height / game_map->perpendicular_wall_distance);
	
	/* Calculate start and end points for the wall line */
	*start_y = (-(*line_height) / 2) + (game_map->display_height / 2);
	*end_y = ((*line_height) / 2) + (game_map->display_height / 2);
	
	/* Clamp to screen boundaries */
	if (*start_y < 0)
		*start_y = 0;
	if (*end_y >= game_map->display_height)
		*end_y = game_map->display_height - 1;
}

/**
 * @brief Calculate texture coordinates for wall rendering
 * 
 * This function determines which part of the texture to display on the wall.
 * It calculates:
 * 1. The exact point where the ray hit the wall (wall_hit_x)
 * 2. The X coordinate in the texture (texture_x)
 * 3. The step size and starting position for texture sampling
 * 
 * The texture mapping ensures that textures are applied correctly
 * regardless of which side of the wall was hit and from which direction.
 * 
 * @param game_map Pointer to the main game structure
 * @param line_start Y coordinate where wall line starts
 * @param line_height Height of the wall line on screen
 */
void	calculate_texture_coordinates(t_game_map *game_map, int line_start, int line_height)
{
	/* Calculate exact hit point on the wall */
	if (game_map->wall_side == 0)
	{
		/* Hit a vertical wall (North/South facing) */
		game_map->wall_hit_x = game_map->player_pos_y + 
			game_map->perpendicular_wall_distance * game_map->ray_direction_y;
	}
	else
	{
		/* Hit a horizontal wall (East/West facing) */
		game_map->wall_hit_x = game_map->player_pos_x + 
			game_map->perpendicular_wall_distance * game_map->ray_direction_x;
	}
	
	/* Remove integer part to get fractional coordinate (0.0 to 1.0) */
	game_map->wall_hit_x -= floor(game_map->wall_hit_x);
	
	/* Convert to texture coordinate (0 to TEXTURE_SIZE-1) */
	game_map->texture_x = (int)(game_map->wall_hit_x * TEXTURE_SIZE);
	
	/* Flip texture if necessary for correct orientation */
	if (game_map->wall_side == 0 && game_map->ray_direction_x > 0)
		game_map->texture_x = TEXTURE_SIZE - game_map->texture_x - 1;
	if (game_map->wall_side == 1 && game_map->ray_direction_y < 0)
		game_map->texture_x = TEXTURE_SIZE - game_map->texture_x - 1;
	
	/* Calculate step size for texture sampling */
	game_map->texture_step = 1.0 * TEXTURE_SIZE / line_height;
	
	/* Calculate starting position in texture */
	game_map->texture_position = (line_start - game_map->display_height / 2 + 
		line_height / 2) * game_map->texture_step;
}

/* ************************************************************************** */
/*                           TEXTURE SELECTION                               */
/* ************************************************************************** */

/**
 * @brief Determine which texture to use based on wall orientation
 * 
 * This function analyzes the wall hit information to determine which
 * of the four directional textures (North, South, East, West) should
 * be used for rendering. The selection is based on:
 * 1. Which side of the wall was hit (wall_side)
 * 2. The direction of the ray when it hit the wall
 * 
 * @param game_map Pointer to the main game structure
 * @return Index of the texture to use (NORTH, SOUTH, EAST, or WEST)
 */
static int	determine_wall_texture(t_game_map *game_map)
{
	if (game_map->wall_side == 1)
	{
		/* Horizontal wall hit */
		if (game_map->ray_direction_y < 0)
			return (SOUTH);  /* Looking north, hit south-facing wall */
		else
			return (NORTH);  /* Looking south, hit north-facing wall */
	}
	else
	{
		/* Vertical wall hit */
		if (game_map->ray_direction_x < 0)
			return (WEST);   /* Looking east, hit west-facing wall */
		else
			return (EAST);   /* Looking west, hit east-facing wall */
	}
}

/**
 * @brief Render a single pixel of the wall with texture
 * 
 * This function draws one pixel of the wall by sampling the appropriate
 * texture at the calculated coordinates. It handles the texture lookup
 * and applies the color to the screen.
 * 
 * @param game_map Pointer to the main game structure
 * @param screen_x X coordinate on screen
 * @param screen_y Y coordinate on screen
 * @param texture_index Index of texture to sample from
 */
static void	draw_textured_wall_pixel(t_game_map *game_map, int screen_x, 
		int screen_y, int texture_index)
{
	int	pixel_color;

	/* Get color from texture at current coordinates */
	pixel_color = get_texture_color(game_map, game_map->texture_x, 
		game_map->texture_y, texture_index);
	
	/* Apply the color to the screen */
	put_pixel_to_image(game_map, screen_x, screen_y, pixel_color);
}

/* ************************************************************************** */
/*                             WALL RENDERING                                */
/* ************************************************************************** */

/**
 * @brief Draw a complete vertical wall column on the screen
 * 
 * This is the main drawing function that renders a complete wall column.
 * It orchestrates the entire process:
 * 1. Calculate wall dimensions and screen coordinates
 * 2. Set up texture mapping parameters
 * 3. Iterate through each pixel of the wall column
 * 4. Sample the appropriate texture and draw each pixel
 * 
 * The function ensures proper texture mapping and handles the perspective
 * projection to create a realistic 3D appearance.
 * 
 * @param game_map Pointer to the main game structure
 * @param screen_x X coordinate of the column to draw
 */
void	draw_wall_column(t_game_map *game_map, int screen_x)
{
	int	line_height;
	int	texture_index;

	/* Calculate wall line parameters */
	calculate_wall_line_parameters(game_map, &line_height, 
		&game_map->draw_start, &game_map->draw_end);
	
	/* Set up texture coordinates */
	calculate_texture_coordinates(game_map, game_map->draw_start, line_height);
	
	/* Determine which texture to use */
	texture_index = determine_wall_texture(game_map);
	
	/* Draw each pixel of the wall column */
	while (game_map->draw_start < game_map->draw_end)
	{
		/* Calculate current Y coordinate in texture */
		game_map->texture_y = (int)game_map->texture_position & (TEXTURE_SIZE - 1);
		
		/* Move to next position in texture */
		game_map->texture_position += game_map->texture_step;
		
		/* Draw the textured pixel */
		draw_textured_wall_pixel(game_map, screen_x, game_map->draw_start, texture_index);
		
		/* Move to next pixel row */
		game_map->draw_start++;
	}
}

/* ************************************************************************** */
/*                         BACKGROUND RENDERING                              */
/* ************************************************************************** */

/**
 * @brief Create a TRGB color value from individual components
 * 
 * This function combines transparency, red, green, and blue values
 * into a single 32-bit integer that MLX can use for pixel colors.
 * 
 * Color format: 0xTTRRGGBB where:
 * - TT: Transparency (usually 0x00 for opaque)
 * - RR: Red component (0x00 to 0xFF)
 * - GG: Green component (0x00 to 0xFF)
 * - BB: Blue component (0x00 to 0xFF)
 * 
 * @param transparency Transparency value (0-255)
 * @param red Red component (0-255)
 * @param green Green component (0-255)
 * @param blue Blue component (0-255)
 * @return Combined TRGB color value
 */
int	create_trgb_color(int transparency, int red, int green, int blue)
{
	return (transparency << 24 | red << 16 | green << 8 | blue);
}

/**
 * @brief Render the floor and ceiling background
 * 
 * This function fills the screen with floor and ceiling colors before
 * the walls are drawn. The screen is divided horizontally:
 * - Upper half: Ceiling color
 * - Lower half: Floor color
 * 
 * This creates the basic environment that walls will be drawn on top of.
 * 
 * @param game_map Pointer to the main game structure
 */
void	render_background_colors(t_game_map *game_map)
{
	int	screen_x;
	int	screen_y;
	int	ceiling_color;
	int	floor_color;

	/* Create color values from RGB arrays */
	ceiling_color = create_trgb_color(0, game_map->ceiling_color_rgb[0],
		game_map->ceiling_color_rgb[1], game_map->ceiling_color_rgb[2]);
	floor_color = create_trgb_color(0, game_map->floor_color_rgb[0],
		game_map->floor_color_rgb[1], game_map->floor_color_rgb[2]);
	
	screen_x = 0;
	while (screen_x < game_map->display_width)
	{
		screen_y = 0;
		while (screen_y < game_map->display_height)
		{
			/* Draw ceiling in upper half of screen */
			if (screen_y < game_map->display_height / 2)
				put_pixel_to_image(game_map, screen_x, screen_y, ceiling_color);
			/* Draw floor in lower half of screen */
			else
				put_pixel_to_image(game_map, screen_x, screen_y, floor_color);
			screen_y++;
		}
		screen_x++;
	}
}
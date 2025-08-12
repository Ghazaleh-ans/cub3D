/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:15:48 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 17:20:44 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           PIXEL MANIPULATION                              */
/* ************************************************************************** */

/**
 * @brief Safely put a pixel to the main display image
 */
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

/* ************************************************************************** */
/*                          WALL HEIGHT CALCULATION                          */
/* ************************************************************************** */

/**
 * @brief Calculate wall line parameters for screen rendering
 */
static void	calculate_wall_line_parameters(t_game *game, int *line_height, 
		int *start_y, int *end_y)
{
	*line_height = (int)(game->mlx.height / game->ray.perp_wall_dist);
	*start_y = (-(*line_height) / 2) + (game->mlx.height / 2);
	*end_y = ((*line_height) / 2) + (game->mlx.height / 2);
	if (*start_y < 0)
		*start_y = 0;
	if (*end_y >= game->mlx.height)
		*end_y = game->mlx.height - 1;
}

/**
 * @brief Calculate texture coordinates for wall rendering
 */
void	calculate_texture_coordinates(t_game *game, int line_start, int line_height)
{
	if (game->ray.side == 0)
	{
		game->wall.hit_x = game->player.pos_y + 
			game->ray.perp_wall_dist * game->ray.dir_y;
	}
	else
	{
		game->wall.hit_x = game->player.pos_x + 
			game->ray.perp_wall_dist * game->ray.dir_x;
	}
	game->wall.hit_x -= floor(game->wall.hit_x);
	game->wall.tex_x = (int)(game->wall.hit_x * TEXTURE_SIZE);
	
	if (game->ray.side == 0 && game->ray.dir_x > 0)
		game->wall.tex_x = TEXTURE_SIZE - game->wall.tex_x - 1;
	if (game->ray.side == 1 && game->ray.dir_y < 0)
		game->wall.tex_x = TEXTURE_SIZE - game->wall.tex_x - 1;
		
	game->wall.tex_step = 1.0 * TEXTURE_SIZE / line_height;
	game->wall.tex_pos = (line_start - game->mlx.height / 2 + 
		line_height / 2) * game->wall.tex_step;
}

/* ************************************************************************** */
/*                           TEXTURE SELECTION                               */
/* ************************************************************************** */

/**
 * @brief Determine which texture to use based on wall orientation
 */
static int	determine_wall_texture(t_game *game)
{
	if (game->ray.side == 1)
	{
		if (game->ray.dir_y < 0)
			return (SOUTH);
		else
			return (NORTH);
	}
	else
	{
		if (game->ray.dir_x < 0)
			return (WEST);
		else
			return (EAST);
	}
}

/**
 * @brief Get the correct texture image based on direction
 */
// static t_image	*get_texture_by_direction(t_game *game, int direction)
// {
// 	if (direction == NORTH)
// 		return (&game->textures.north);
// 	else if (direction == SOUTH)
// 		return (&game->textures.south);
// 	else if (direction == EAST)
// 		return (&game->textures.east);
// 	else if (direction == WEST)
// 		return (&game->textures.west);
// 	return (&game->textures.north); /* fallback */
// }

/**
 * @brief Render a single pixel of the wall with texture
 */
static void	draw_textured_wall_pixel(t_game *game, int screen_x, 
		int screen_y, int texture_index)
{
	int	pixel_color;

	pixel_color = get_texture_color(game, game->wall.tex_x, 
		game->wall.tex_y, texture_index);
	put_pixel_to_image(game, screen_x, screen_y, pixel_color);
}

/* ************************************************************************** */
/*                             WALL RENDERING                                */
/* ************************************************************************** */

/**
 * @brief Draw a complete vertical wall column on the screen
 */
void	draw_wall_column(t_game *game, int screen_x)
{
	int	line_height;
	int	texture_index;

	calculate_wall_line_parameters(game, &line_height, 
		&game->wall.draw_start, &game->wall.draw_end);
	calculate_texture_coordinates(game, game->wall.draw_start, line_height);
	texture_index = determine_wall_texture(game);
	
	while (game->wall.draw_start < game->wall.draw_end)
	{
		game->wall.tex_y = (int)game->wall.tex_pos & (TEXTURE_SIZE - 1);
		game->wall.tex_pos += game->wall.tex_step;
		draw_textured_wall_pixel(game, screen_x, game->wall.draw_start, texture_index);
		game->wall.draw_start++;
	}
}

/* ************************************************************************** */
/*                         BACKGROUND RENDERING                              */
/* ************************************************************************** */

/**
 * @brief Create a TRGB color value from individual components
 */
int	create_trgb_color(int transparency, int red, int green, int blue)
{
	return (transparency << 24 | red << 16 | green << 8 | blue);
}

/**
 * @brief Render the floor and ceiling background
 */
void	render_background_colors(t_game *game)
{
	int	screen_x;
	int	screen_y;
	int	ceiling_color;
	int	floor_color;

	ceiling_color = create_trgb_color(0, game->map.ceiling_rgb[0],
		game->map.ceiling_rgb[1], game->map.ceiling_rgb[2]);
	floor_color = create_trgb_color(0, game->map.floor_rgb[0],
		game->map.floor_rgb[1], game->map.floor_rgb[2]);
		
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

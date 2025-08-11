/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:13:00 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:13:02 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           RAYCASTING MATHEMATICS                           */
/* ************************************************************************** */

/**
 * @brief Calculate delta distances for the DDA algorithm
 * 
 * Delta distances represent how far along the ray we must travel
 * to cross one map square in either X or Y direction. This is
 * fundamental to the DDA (Digital Differential Analyzer) algorithm.
 * 
 * The formula used is: delta_dist = |1 / ray_direction|
 * 
 * Special case: If ray direction is 0 (ray is parallel to axis),
 * we set delta distance to a very large value (INT_MAX) to
 * effectively disable stepping in that direction.
 * 
 * @param game_map Pointer to the main game structure
 */
void	calculate_delta_distances(t_game_map *game_map)
{
	/* Calculate delta distance for X direction */
	if (game_map->ray_direction_x == 0)
		game_map->delta_distance_x = INT_MAX;
	else
		game_map->delta_distance_x = fabs(1 / game_map->ray_direction_x);
	
	/* Calculate delta distance for Y direction */
	if (game_map->ray_direction_y == 0)
		game_map->delta_distance_y = INT_MAX;
	else
		game_map->delta_distance_y = fabs(1 / game_map->ray_direction_y);
}

/**
 * @brief Calculate step direction and initial side distances
 * 
 * This function determines:
 * 1. Which direction to step in the map grid (+1 or -1)
 * 2. Initial distance to the next map square boundary
 * 
 * For each axis (X and Y):
 * - If ray direction is negative, we step backwards (-1)
 * - If ray direction is positive, we step forwards (+1)
 * - Side distance is calculated based on current position and step direction
 * 
 * @param game_map Pointer to the main game structure
 */
void	calculate_step_and_side_distances(t_game_map *game_map)
{
	/* Calculate step direction and side distance for X axis */
	if (game_map->ray_direction_x < 0)
	{
		game_map->step_x = -1;
		game_map->side_distance_x = (game_map->player_pos_x - game_map->current_map_x) 
			* game_map->delta_distance_x;
	}
	else
	{
		game_map->step_x = 1;
		game_map->side_distance_x = (game_map->current_map_x + 1.0 - game_map->player_pos_x) 
			* game_map->delta_distance_x;
	}
	
	/* Calculate step direction and side distance for Y axis */
	if (game_map->ray_direction_y < 0)
	{
		game_map->step_y = -1;
		game_map->side_distance_y = (game_map->player_pos_y - game_map->current_map_y) 
			* game_map->delta_distance_y;
	}
	else
	{
		game_map->step_y = 1;
		game_map->side_distance_y = (game_map->current_map_y + 1.0 - game_map->player_pos_y) 
			* game_map->delta_distance_y;
	}
}

/**
 * @brief Execute the Digital Differential Analyzer (DDA) algorithm
 * 
 * DDA is the core algorithm that traces a ray through the map grid
 * until it hits a wall. It works by:
 * 1. Comparing side distances for X and Y directions
 * 2. Stepping to the closer boundary
 * 3. Updating the side distance for the stepped direction
 * 4. Checking if we hit a wall ('1' in the map)
 * 5. Repeating until a wall is found
 * 
 * The algorithm also tracks which side of the wall was hit:
 * - wall_side = 0: Hit a vertical wall (X-side)
 * - wall_side = 1: Hit a horizontal wall (Y-side)
 * 
 * @param game_map Pointer to the main game structure
 */
void	execute_dda_algorithm(t_game_map *game_map)
{
	int	wall_hit;

	wall_hit = 0;
	
	/* Continue until we hit a wall */
	while (wall_hit == 0)
	{
		/* Check which direction has the shorter distance to next boundary */
		if (game_map->side_distance_x < game_map->side_distance_y)
		{
			/* Step in X direction */
			game_map->side_distance_x += game_map->delta_distance_x;
			game_map->current_map_x += game_map->step_x;
			game_map->wall_side = 0;  /* Hit vertical wall */
		}
		else
		{
			/* Step in Y direction */
			game_map->side_distance_y += game_map->delta_distance_y;
			game_map->current_map_y += game_map->step_y;
			game_map->wall_side = 1;  /* Hit horizontal wall */
		}
		
		/* Check if we hit a wall */
		if (game_map->map_grid[game_map->current_map_y][game_map->current_map_x] == '1')
			wall_hit = 1;
	}
}

/**
 * @brief Initialize raycasting variables for a specific screen column
 * 
 * For each column of pixels on the screen, we need to:
 * 1. Calculate which ray to cast based on camera position
 * 2. Initialize map coordinates to player's current position
 * 3. Calculate ray direction based on player direction and camera plane
 * 
 * The camera_x value ranges from -1 to +1:
 * - -1: Leftmost pixel of screen
 * - 0: Center of screen
 * - +1: Rightmost pixel of screen
 * 
 * @param game_map Pointer to the main game structure
 * @param screen_x Current X coordinate on the screen (0 to screen_width-1)
 */
void	initialize_raycasting_for_column(t_game_map *game_map, int screen_x)
{
	/* Set current map position to player's position */
	game_map->current_map_x = (int)game_map->player_pos_x;
	game_map->current_map_y = (int)game_map->player_pos_y;
	
	/* Calculate camera X coordinate (-1 to +1) */
	game_map->camera_x = 2 * screen_x / (double)game_map->display_width - 1;
	
	/* Calculate ray direction based on player direction and camera plane */
	game_map->ray_direction_x = game_map->player_dir_x + game_map->camera_plane_x * game_map->camera_x;
	game_map->ray_direction_y = game_map->player_dir_y + game_map->camera_plane_y * game_map->camera_x;
}

/**
 * @brief Main raycasting function that renders the entire screen
 * 
 * This function orchestrates the complete raycasting process:
 * 1. Iterate through each column of the screen
 * 2. Cast a ray for each column
 * 3. Calculate wall distance and height
 * 4. Draw the wall column
 * 
 * The raycasting algorithm creates a 3D perspective view from a 2D map
 * by calculating distances to walls and drawing vertical lines with
 * heights inversely proportional to those distances.
 * 
 * @param game_map Pointer to the main game structure
 */
void	execute_raycasting(t_game_map *game_map)
{
	int	screen_x;

	screen_x = 0;
	
	/* Process each column of the screen */
	while (screen_x < game_map->display_width)
	{
		/* Initialize raycasting variables for this column */
		initialize_raycasting_for_column(game_map, screen_x);
		
		/* Calculate delta distances for DDA algorithm */
		calculate_delta_distances(game_map);
		
		/* Calculate step directions and initial side distances */
		calculate_step_and_side_distances(game_map);
		
		/* Execute DDA algorithm to find wall intersection */
		execute_dda_algorithm(game_map);
		
		/* Calculate perpendicular distance to avoid fisheye effect */
		if (game_map->wall_side == 0)
		{
			/* Vertical wall hit */
			game_map->perpendicular_wall_distance = (game_map->side_distance_x - game_map->delta_distance_x);
		}
		else
		{
			/* Horizontal wall hit */
			game_map->perpendicular_wall_distance = (game_map->side_distance_y - game_map->delta_distance_y);
		}
		
		/* Draw the wall column for this screen x-coordinate */
		draw_wall_column(game_map, screen_x);
		
		/* Move to next column */
		screen_x++;
	}
}
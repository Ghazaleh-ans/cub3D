/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:54:13 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 17:09:24 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_STRUCT_H
# define GAME_STRUCT_H

/**
 * @brief Image structure for MLX image handling
 * 
 * This structure contains all necessary information for managing
 * MLX images including textures and the main display buffer.
 */
typedef struct s_image
{
	void	*mlx_ptr;			/* MLX image pointer */
	char	*path;				/* Path to texture file (for wall textures) */
	char	*data;				/* Direct access to image pixel data */
	int		bits_per_pixel;		/* Number of bits per pixel */
	int		line_length;		/* Length of one line in bytes */
	int		endian;				/* Endianness of the image data */
	int		width;				/* Width of the image in pixels */
	int		height;				/* Height of the image in pixels */
}	t_image;

/**
 * @brief MLX-related data and window management
 */
typedef struct s_mlx
{
	void		*instance;			/* MLX instance pointer */
	void		*window;			/* MLX window pointer */
	int			width;				/* Window width in pixels */
	int			height;				/* Window height in pixels */
}	t_mlx;

/**
 * @brief Map configuration and data
 */
typedef struct s_map
{
	char		**grid;				/* 2D array representing the game map */
	int			width;				/* Width of the map in grid units */
	int			height;				/* Height of the map in grid units */
	int			floor_rgb[3];		/* Floor color RGB values [R,G,B] */
	int			ceiling_rgb[3];		/* Ceiling color RGB values [R,G,B] */
	char		*current_line;		/* Current line being parsed */
	char		*data_buffer;		/* Buffer for accumulating map data */
}	t_map;

/**
 * @brief Player position, direction and movement
 */
typedef struct s_player
{
	double		pos_x;				/* Current player X position */
	double		pos_y;				/* Current player Y position */
	double		dir_x;				/* Player direction vector X component */
	double		dir_y;				/* Player direction vector Y component */
	double		plane_x;			/* Camera plane X component (FOV) */
	double		plane_y;			/* Camera plane Y component (FOV) */
	char		initial_dir;		/* Initial player direction (N/S/E/W) */
	double		move_speed;			/* Player movement speed */
	double		rotate_speed;		/* Player rotation speed */
}	t_player;

/**
 * @brief Raycasting calculation data
 */
typedef struct s_ray
{
	double		dir_x;				/* Current ray direction X component */
	double		dir_y;				/* Current ray direction Y component */
	double		camera_x;			/* X coordinate on camera plane (-1 to 1) */
	double		side_dist_x;		/* Distance to next X map square */
	double		side_dist_y;		/* Distance to next Y map square */
	double		delta_dist_x;		/* Distance between X map squares */
	double		delta_dist_y;		/* Distance between Y map squares */
	double		perp_wall_dist;		/* Perpendicular distance to wall */
	int			map_x;				/* Current X position in map grid */
	int			map_y;				/* Current Y position in map grid */
	int			step_x;				/* Step direction in X (-1 or 1) */
	int			step_y;				/* Step direction in Y (-1 or 1) */
	int			side;				/* Which side of wall was hit (0=X, 1=Y) */
}	t_ray;

/**
 * @brief Wall rendering and texture data
 */
typedef struct s_wall
{
	double		hit_x;				/* X coordinate where ray hits wall */
	int			tex_x;				/* X coordinate in texture */
	int			tex_y;				/* Y coordinate in texture */
	double		tex_step;			/* Step size for texture sampling */
	double		tex_pos;			/* Current position in texture */
	int			line_height;		/* Height of wall line on screen */
	int			draw_start;			/* Start Y coordinate for drawing */
	int			draw_end;			/* End Y coordinate for drawing */
}	t_wall;

/**
 * @brief Texture management system
 */
typedef struct s_textures
{
	t_image		north;				/* North wall texture */
	t_image		south;				/* South wall texture */
	t_image		east;				/* East wall texture */
	t_image		west;				/* West wall texture */
	t_image		screen;				/* Main display buffer */
	
	#ifdef BONUS
	t_image		minimap;			/* Minimap image data (bonus feature) */
	#endif
}	t_textures;

/**
 * @brief Main game structure - orchestrates all subsystems
 */
typedef struct s_game
{
	t_mlx			mlx;			/* MLX window and display management */
	t_map			map;			/* Map data and configuration */
	t_player		player;			/* Player state and movement */
	t_ray			ray;			/* Raycasting calculations */
	t_wall			wall;			/* Wall rendering data */
	t_textures		textures;		/* Texture management */
}	t_game;

/* Legacy typedef for backward compatibility */
typedef t_game t_game_map;

/* ************************************************************************** */
/*                           STRUCTURE DOCUMENTATION                         */
/* ************************************************************************** */

/*
 * REFACTORED STRUCTURE BENEFITS:
 * 
 * 1. SEPARATION OF CONCERNS:
 *    - Each struct handles a specific aspect of the game
 *    - Easier to understand and maintain individual components
 *    - Clear relationships between different systems
 * 
 * 2. IMPROVED READABILITY:
 *    - Shorter, more descriptive member names
 *    - Logical grouping of related data
 *    - Self-documenting structure organization
 * 
 * 3. EASIER DEBUGGING:
 *    - Can examine specific subsystems independently
 *    - Clear data flow between components
 *    - Reduced complexity per structure
 * 
 * 4. BETTER MAINTAINABILITY:
 *    - Changes to one subsystem don't affect others
 *    - Easy to add new features to specific areas
 *    - Clear interface boundaries
 * 
 * USAGE PATTERNS:
 * 
 * - Access MLX data: game->mlx.instance, game->mlx.window
 * - Access map data: game->map.grid, game->map.width
 * - Access player: game->player.pos_x, game->player.dir_x
 * - Access textures: game->textures.north.mlx_ptr
 * - Access raycasting: game->ray.dir_x, game->ray.side_dist_x
 * 
 * MEMORY MANAGEMENT:
 * - Each subsystem can have its own cleanup function
 * - Clear ownership of pointers and resources
 * - Easier to track memory allocation/deallocation
 */

#endif
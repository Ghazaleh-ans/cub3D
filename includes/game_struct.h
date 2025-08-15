/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:54:13 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 16:44:32 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_STRUCT_H
# define GAME_STRUCT_H

/**
 * @brief Color structure for RGB values
 */
typedef struct s_color {
	int	red;
	int	green;
	int	blue;
} t_color;

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
 * @brief Player position, direction and movement
 */
typedef struct s_player
{
	double		x;					/* Current player X position (new parsing) */
	double		y;					/* Current player Y position (new parsing) */
	char		direction;			/* Player direction character (new parsing) */
	
	/* Rendering system fields (from original) */
	double		pos_x;				/* Current player X position */
	double		pos_y;				/* Current player Y position */
	double		dir_x;				/* Player direction vector X component */
	double		dir_y;				/* Player direction vector Y component */
	double		plane_x;			/* Camera plane X component (FOV) */
	double		plane_y;			/* Camera plane Y component (FOV) */
	char		initial_dir;		/* Initial player direction (N/S/E/W) */
	double		move_speed;			/* Player movement speed */
	double		rotate_speed;		/* Player rotation speed */
	
	/* Input handling */
	int			key_w;				/* Forward movement */
	int			key_s;				/* Backward movement */
	int			key_a;				/* Strafe left */
	int			key_d;				/* Strafe right */
	int			key_left;			/* Turn left */
	int			key_right;			/* Turn right */
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
 * @brief Texture identifiers for array access
 */
typedef enum e_texture_type
{
	NORTH_TEX,
	SOUTH_TEX,
	WEST_TEX,
	EAST_TEX,
	TEX_COUNT
}	t_texture_type;

/**
 * @brief Main game structure - orchestrates all subsystems
 * 
 * This structure combines both parsing approaches:
 * - Original structured approach for rendering
 * - New simple approach for parsing
 */
typedef struct s_game
{
	/* MLX and rendering subsystems (original structure) */
	t_mlx			mlx;			/* MLX window and display management */
	t_ray			ray;			/* Raycasting calculations */
	t_wall			wall;			/* Wall rendering data */
	t_textures		textures;		/* Texture management (structured) */
	
	/* Parsing data (new parsing system) */
	char			**map;			/* 2D array representing the game map */
	int				map_width;		/* Width of the map in grid units */
	int				map_height;		/* Height of the map in grid units */
	char			*textures_paths[TEX_COUNT];	/* Texture file paths */
	t_color			floor_color;	/* Floor color RGB values */
	t_color			ceiling_color;	/* Ceiling color RGB values */
	t_player		player;			/* Player state and movement */
	
	/* Legacy compatibility fields for rendering */
	char			*current_line;	/* Current line being parsed */
	char			*data_buffer;	/* Buffer for accumulating map data */
}	t_game;

/* ************************************************************************** */
/*                           STRUCTURE DOCUMENTATION                         */
/* ************************************************************************** */

/*
 * MIGRATION STRATEGY:
 * 
 * 1. DUAL COMPATIBILITY:
 *    - Supports both parsing systems during transition
 *    - New parsing uses: game->map, game->textures_paths[], game->floor_color
 *    - Rendering uses: game->textures.north.path, game->mlx.instance
 * 
 * 2. DATA CONVERSION:
 *    - After parsing: convert t_color to int arrays for rendering
 *    - Copy texture paths from textures_paths[] to textures.*.path
 *    - Copy map from char **map to structured format if needed
 * 
 * 3. GRADUAL MIGRATION:
 *    - Phase 1: Use new parsing with compatibility layer
 *    - Phase 2: Update rendering to use new structure
 *    - Phase 3: Remove legacy fields
 * 
 * USAGE PATTERNS:
 * 
 * - Parsing: game->map[y][x], game->textures_paths[NORTH_TEX]
 * - Rendering: game->textures.north.mlx_ptr, game->mlx.instance
 * - Player: game->player.x (parsing), game->player.pos_x (rendering)
 */

#endif
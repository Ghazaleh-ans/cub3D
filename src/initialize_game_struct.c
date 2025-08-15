/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_game_struct.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:04:12 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 17:01:20 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	init_mlx(t_game *game)
{
	game->mlx.instance = NULL;
	game->mlx.window = NULL;
	game->mlx.width = 0;
	game->mlx.height = 0;
}

static void	init_player(t_game *game)
{
	/* New parsing system fields */
	game->player.x = 0.0;
	game->player.y = 0.0;
	game->player.direction = 0;
	
	/* Original rendering system fields */
	game->player.pos_x = 0.0;
	game->player.pos_y = 0.0;
	game->player.dir_x = 0.0;
	game->player.dir_y = 0.0;
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.0;
	game->player.initial_dir = '\0';
	game->player.move_speed = 0.0;
	game->player.rotate_speed = 0.0;
	
	/* Input handling */
	game->player.key_w = 0;
	game->player.key_s = 0;
	game->player.key_a = 0;
	game->player.key_d = 0;
	game->player.key_left = 0;
	game->player.key_right = 0;
}

static void	init_textures(t_game *game)
{
	int i;
	
	/* Initialize new parsing system texture paths */
	i = 0;
	while (i < TEX_COUNT)
	{
		game->textures_paths[i] = NULL;
		i++;
	}
	
	/* Initialize original rendering system textures */
	game->textures.north.mlx_ptr = NULL;
	game->textures.north.path = NULL;
	game->textures.south.mlx_ptr = NULL;
	game->textures.south.path = NULL;
	game->textures.east.mlx_ptr = NULL;
	game->textures.east.path = NULL;
	game->textures.west.mlx_ptr = NULL;
	game->textures.west.path = NULL;
	game->textures.screen.mlx_ptr = NULL;
	game->textures.screen.path = NULL;
	
	#ifdef BONUS
	game->textures.minimap.mlx_ptr = NULL;
	#endif
}

static void	init_map_new_system(t_game *game)
{
	/* New parsing system fields */
	game->map = NULL;
	game->map_width = 0;
	game->map_height = 0;
	game->floor_color = (t_color){-1, -1, -1};
	game->ceiling_color = (t_color){-1, -1, -1};
	
	/* Legacy compatibility fields */
	game->current_line = NULL;
	game->data_buffer = NULL;
}

/**
 * @brief Initialize game structure with dual compatibility
 * 
 * This function initializes both the new parsing system fields
 * and the original rendering system fields for compatibility.
 */
void	init_game_structure(t_game *game)
{
	init_mlx(game);
	init_map_new_system(game);
	init_player(game);
	init_textures(game);
}
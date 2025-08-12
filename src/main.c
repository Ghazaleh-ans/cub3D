/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:30:20 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 17:10:19 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	validate_file_extension(char *filename)
{
	int	filename_length;
	int	extension_index;

	filename_length = ft_strlen(filename);
	if (filename_length < 4)
		return (0);
	extension_index = 0;
	filename_length -= 4;
	while (filename[filename_length] != '\0')
	{
		if (extension_index == 0 && filename[filename_length] != '.')
			return (0);
		if (extension_index == 1 && filename[filename_length] != 'c')
			return (0);
		if (extension_index == 2 && filename[filename_length] != 'u')
			return (0);
		if (extension_index == 3 && filename[filename_length] != 'b')
			return (0);
		filename_length++;
		extension_index++;
	}
	return (1);
}

int	validate_input_file(char *filename)
{
	int		fd;
	int		read_result;
	char	test_buffer[1];

	if (!validate_file_extension(filename))
	{
		ft_printf("Error\nInvalid file extension. Expected: .cub\n");
		return (0);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("Error\nCannot open file: %s\n", filename);
		return (0);
	}
	read_result = read(fd, test_buffer, 1);
	close(fd);
	if (read_result <= 0)
	{
		if (read_result == 0)
			ft_printf("Error\nFile is empty: %s\n", filename);
		else
			ft_printf("Error\nCannot read file: %s\n", filename);
		return (0);
	}
	return (1);
}

void	init_game_structure(t_game *game)
{
	/* Initialize MLX data */
	game->mlx.instance = NULL;
	game->mlx.window = NULL;
	game->mlx.width = 0;
	game->mlx.height = 0;

	/* Initialize map data */
	game->map.grid = NULL;
	game->map.width = 0;
	game->map.height = 0;
	game->map.floor_rgb[0] = -1;
	game->map.floor_rgb[1] = -1;
	game->map.floor_rgb[2] = -1;
	game->map.ceiling_rgb[0] = -1;
	game->map.ceiling_rgb[1] = -1;
	game->map.ceiling_rgb[2] = -1;
	game->map.current_line = NULL;
	game->map.data_buffer = NULL;

	/* Initialize player data */
	game->player.pos_x = 0.0;
	game->player.pos_y = 0.0;
	game->player.dir_x = 0.0;
	game->player.dir_y = 0.0;
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.0;
	game->player.initial_dir = '\0';
	game->player.move_speed = 0.0;
	game->player.rotate_speed = 0.0;

	/* Initialize textures */
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

int	main(int argc, char **argv)
{
	t_game	game;
	int		fd;

	init_game_structure(&game);
	if (argc != 2)
	{
		ft_printf("Error\nUsage: %s <map_file.cub>\n", argv[0]);
		ft_printf("Expected exactly one argument: path to .cub file\n");
		return (1);
	}
	if (!validate_input_file(argv[1]))
		return (2);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		ft_printf("Error\nFailed to open file for parsing: %s\n", argv[1]);
		return (3);
	}
	if (!parse_map_file(&game, fd))
	{
		ft_printf("Error\nMap parsing failed\n");
		free_string_array(game.map.grid);
		close(fd);
		return (4);
	}
	close(fd);
	if (init_game_engine(&game) != 0)
	{
		ft_printf("Error\nGame engine initialization failed\n");
		free_string_array(game.map.grid);
		return (5);
	}
	return (0);
}

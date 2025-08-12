/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:30:20 by gansari           #+#    #+#             */
/*   Updated: 2025/08/12 12:40:18 by gansari          ###   ########.fr       */
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

void	initialize_game_structure(t_game_map *game_map)
{
	int	texture_index;

	game_map->mlx_instance = NULL;
	game_map->mlx_window = NULL;
	game_map->map_grid = NULL;
	game_map->map_width = 0;
	game_map->map_height = 0;
	game_map->floor_color_rgb[0] = -1;
	game_map->floor_color_rgb[1] = -1;
	game_map->floor_color_rgb[2] = -1;
	game_map->ceiling_color_rgb[0] = -1;
	game_map->ceiling_color_rgb[1] = -1;
	game_map->ceiling_color_rgb[2] = -1;
	game_map->current_line = NULL;
	game_map->map_data_buffer = NULL;
	texture_index = 0;
	while (texture_index < 5)
	{
		game_map->texture_images[texture_index].mlx_image_ptr = NULL;
		game_map->texture_images[texture_index].texture_path = NULL;
		texture_index++;
	}
	#ifdef BONUS
	game_map->minimap_image.mlx_image_ptr = NULL;
	#endif
}

int	main(int argc, char **argv)
{
	t_game_map	game_map;
	int			fd;

	initialize_game_structure(&game_map);
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
	if (!parse_map_file(&game_map, fd))
	{
		ft_printf("Error\nMap parsing failed\n");
		free_string_array(game_map.map_grid);
		close(fd);
		return (4);
	}
	close(fd);
	if (initialize_game_engine(&game_map) != 0)
	{
		ft_printf("Error\nGame engine initialization failed\n");
		free_string_array(game_map.map_grid);
		return (5);
	}
	return (0);
}

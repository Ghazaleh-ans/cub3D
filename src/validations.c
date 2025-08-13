/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validations.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 15:28:22 by gansari           #+#    #+#             */
/*   Updated: 2025/08/13 15:37:36 by gansari          ###   ########.fr       */
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

int	is_valid_file_descriptor(int fd)
{
	if (fd < 0)
	{
		printf("Error\nInvalid file descriptor\n");
		return (0);
	}
	return (1);
}

int	validate_arguments(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Error\nUsage: %s <map_file.cub>\n", argv[0]);
		printf("Expected exactly one argument: path to .cub file\n");
		return (0);
	}
	return (1);
}

int	is_parseable_map_file(t_game *game, int fd)
{
	if(!is_valid_file_descriptor(fd))
		return (0);
	if (!parse_map_file(game, fd))
	{
		printf("Error\nMap parsing failed\n");
		free_string_array(game->map.grid);
		close(fd);
		return (0);
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
		printf("Error\nInvalid file extension. Expected: .cub\n");
		return (0);
	}
	fd = open(filename, O_RDONLY);
	if (!is_valid_file_descriptor(fd))
		return (0);
	read_result = read(fd, test_buffer, 1);
	close(fd);
	if (read_result <= 0)
	{
		if (read_result == 0)
			printf("Error\nFile is empty: %s\n", filename);
		else
			printf("Error\nCannot read file: %s\n", filename);
		return (0);
	}
	return (1);
}

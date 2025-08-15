/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:30:20 by gansari           #+#    #+#             */
/*   Updated: 2025/08/15 16:46:01 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * @brief Main function using new parsing system
 * 
 * This function integrates the new parsing system with the existing
 * rendering engine through a compatibility layer.
 */
int	main(int argc, char **argv)
{
	t_game	game;

	/* Validate command line arguments */
	if (!validate_arguments(argc, argv))
		return (1);
	
	/* Validate file extension using new system */
	if (!validate_file_extension(argv[1]))
	{
		print_error(ERR_FILE_EXT);
		return (2);
	}
	
	/* Initialize game structure (dual compatibility) */
	init_game_structure(&game);
	
	/* Parse map file using new parsing system */
	if (parse_file(&game, argv[1]) == EXIT_FAILURE)
	{
		cleanup_map(&game);
		return (4);
	}
	
	/* Validate parsed map */
	if (!validate_map(&game))
	{
		cleanup_map(&game);
		return (5);
	}
	
	/* Convert new parsing data to rendering format */
	convert_parsing_to_rendering(&game);
	
	/* Initialize and start game engine */
	if (init_game_engine(&game) != 0)
	{
		printf("Error\nGame engine initialization failed\n");
		cleanup_map(&game);
		return (6);
	}
	
	return (0);
}

/**
 * @brief Validate command line arguments (compatibility function)
 */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rgb_texture.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:47:16 by gansari           #+#    #+#             */
/*   Updated: 2025/08/11 14:47:19 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           TEXTURE PATH PARSING                            */
/* ************************************************************************** */

/**
 * @brief Extract and validate texture file path from configuration line
 * 
 * This function processes texture definition lines and extracts the file path:
 * 1. Validates the line format (should have exactly 2 elements)
 * 2. Checks for duplicate texture definitions
 * 3. Extracts and stores the texture path
 * 4. Removes trailing newlines for clean file paths
 * 5. Frees the temporary split array
 * 
 * Expected format: "DIRECTION ./path/to/texture.xpm"
 * Where DIRECTION is NO, SO, WE, or EA
 * 
 * @param game_map Pointer to the main game structure
 * @param texture_path Pointer to store the extracted texture path
 * @param split_line Array of strings from splitting the configuration line
 */
void	extract_texture_path(t_game_map *game_map, char **texture_path, 
		char **split_line)
{
	int	element_count;

	element_count = 0;
	
	/* Count elements in split line */
	while (split_line[element_count])
		element_count++;
	
	/* Handle case where line might have trailing newline as separate element */
	if (element_count == 3 && split_line[2][0] == '\n')
	{
		free(split_line[element_count - 1]);
		element_count--;
	}
	
	/* Validate line format and check for duplicates */
	if (element_count != 2 || *texture_path != NULL)
	{
		free_string_array(split_line);
		handle_parsing_error(game_map, "Error\nInvalid or duplicate texture definition\n");
	}
	
	/* Extract texture path, removing any trailing newline */
	*texture_path = ft_strdup(split_line[1]);
	if (!*texture_path)
	{
		free_string_array(split_line);
		handle_parsing_error(game_map, "Error\nMemory allocation failed for texture path\n");
	}
	
	/* Remove trailing newline if present */
	if ((*texture_path)[get_string_length_no_newline(*texture_path)] == '\n')
		(*texture_path)[get_string_length_no_newline(*texture_path)] = '\0';
	
	/* Clean up temporary split array */
	free_string_array(split_line);
}

/* ************************************************************************** */
/*                           RGB FORMAT VALIDATION                           */
/* ************************************************************************** */

/**
 * @brief Validate RGB color line format
 * 
 * This function performs comprehensive validation of RGB color strings:
 * 1. Length validation (maximum 11 characters for "255,255,255")
 * 2. Character validation (only digits and commas allowed)
 * 3. Comma count validation (exactly 2 commas required)
 * 4. Format validation (no consecutive commas, proper start/end)
 * 
 * Valid format: "R,G,B" where R, G, B are integers 0-255
 * Example: "220,100,0"
 * 
 * @param rgb_line String containing RGB color values
 * @return 1 if format is valid, 0 if any validation fails
 */
int	validate_rgb_line_format(char *rgb_line)
{
	int	char_index;
	int	comma_count;

	char_index = 0;
	comma_count = 0;
	
	/* Basic validation: must start and end with digits */
	if (!ft_isdigit(rgb_line[0]) || 
		!ft_isdigit(rgb_line[get_string_length_no_newline(rgb_line) - 1]))
		return (0);
	
	/* Length validation: maximum 11 characters for "255,255,255" */
	if (get_string_length_no_newline(rgb_line) > 11)
		return (0);
	
	/* Character and format validation */
	while (rgb_line[char_index + 1])
	{
		/* Count commas */
		if (rgb_line[char_index] == ',')
			comma_count++;
		
		/* Check for consecutive commas (invalid) */
		if (rgb_line[char_index] == ',' && rgb_line[char_index + 1] == ',')
			return (0);
		
		/* Validate character types (only digits and commas allowed) */
		if (!ft_isdigit(rgb_line[char_index]) && rgb_line[char_index] != ',')
			return (0);
		
		char_index++;
	}
	
	/* Must have exactly 2 commas for 3 color components */
	if (comma_count != 2)
		return (0);
	
	return (1);
}

/**
 * @brief Parse and validate individual RGB color values
 * 
 * This function extracts RGB values from a validated color string and
 * ensures each component is within the valid range [0-255]:
 * 1. Parses the first number before the first comma
 * 2. Finds and parses subsequent numbers after commas
 * 3. Validates each value is within RGB range (0-255)
 * 4. Stores values in the provided RGB array
 * 
 * @param rgb_array Array to store the three RGB values [R, G, B]
 * @param rgb_line String containing comma-separated RGB values
 * @return 1 if all values are valid, 0 if any value is out of range
 */
static int	parse_and_validate_rgb_values(int *rgb_array, char *rgb_line)
{
	int	char_index;
	int	component_index;

	char_index = 0;
	component_index = 1;
	
	/* Validate format before parsing */
	if (!validate_rgb_line_format(rgb_line))
		return (0);
	
	/* Parse first RGB component */
	rgb_array[0] = ft_atoi(rgb_line);
	
	/* Find and parse remaining components */
	while (rgb_line[char_index])
	{
		if (rgb_line[char_index] == ',' && component_index < 3)
		{
			rgb_array[component_index] = ft_atoi(rgb_line + char_index + 1);
			component_index++;
		}
		char_index++;
	}
	
	/* Validate all RGB values are within valid range [0-255] */
	component_index = 0;
	while (component_index < 3)
	{
		if (rgb_array[component_index] < 0 || rgb_array[component_index] > 255)
			return (0);
		component_index++;
	}
	
	return (1);
}

/* ************************************************************************** */
/*                           RGB COLOR EXTRACTION                            */
/* ************************************************************************** */

/**
 * @brief Extract RGB color values from configuration line
 * 
 * This function processes color definition lines (F or C) and extracts
 * the RGB values:
 * 1. Validates line format (should have exactly 2 elements)
 * 2. Handles potential trailing newline elements
 * 3. Parses and validates RGB values
 * 4. Stores values in the appropriate color array
 * 5. Performs cleanup of temporary data
 * 
 * Expected format: "F 220,100,0" or "C 225,30,0"
 * 
 * @param game_map Pointer to the main game structure
 * @param rgb_array Array to store RGB values (floor or ceiling)
 * @param split_line Array of strings from splitting the configuration line
 */
void	extract_rgb_colors(t_game_map *game_map, int *rgb_array, char **split_line)
{
	int	element_count;

	element_count = 0;
	
	/* Count elements in split line */
	while (split_line[element_count])
		element_count++;
	
	/* Handle potential trailing newline as separate element */
	if (element_count == 3 && split_line[2][0] == '\n')
	{
		free(split_line[element_count - 1]);
		element_count--;
	}
	
	/* Validate line format */
	if (element_count != 2)
	{
		free_string_array(split_line);
		handle_parsing_error(game_map, "Error\nInvalid color definition format\n");
	}
	
	/* Parse and validate RGB values */
	if (!parse_and_validate_rgb_values(rgb_array, split_line[1]))
	{
		free_string_array(split_line);
		handle_parsing_error(game_map, "Error\nInvalid RGB color values (must be 0-255)\n");
	}
	
	/* Clean up temporary split array */
	free_string_array(split_line);
}
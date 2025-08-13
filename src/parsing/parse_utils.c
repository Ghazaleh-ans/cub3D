#include "../../includes/cub3d.h"

/* ************************************************************************** */
/*                           CHARACTER VALIDATION                            */
/* ************************************************************************** */

/**
 * @brief Check if a character is present in a list of valid characters
 */
int	is_character_valid(char character, char *valid_characters)
{
	int	index;

	index = 0;
	
	/* Iterate through valid character set */
	while (valid_characters[index])
	{
		if (character == valid_characters[index])
			return (1);
		index++;
	}
	
	return (0);
}

/* ************************************************************************** */
/*                            STRING UTILITIES                               */
/* ************************************************************************** */

/**
 * @brief Calculate string length excluding newline character
 */
int	get_string_length_no_newline(char *string)
{
	int	length;

	length = 0;
	
	/* Count characters until newline or end of string */
	while (string && string[length] && string[length] != '\n')
		length++;
	
	return (length);
}

/**
 * @brief Resize a string to a specific length, padding with spaces
 */
char	*resize_string_to_size(char *original_string, int target_size)
{
	int		copy_index;
	char	*resized_string;

	copy_index = 0;
	
	/* Allocate memory for the resized string (+1 for null terminator) */
	resized_string = malloc(sizeof(char) * (target_size + 1));
	if (!resized_string)
		return (NULL);
	
	/* Copy existing content from original string */
	while (original_string && original_string[copy_index] && copy_index < target_size)
	{
		resized_string[copy_index] = original_string[copy_index];
		copy_index++;
	}
	
	/* Pad remaining space with space characters */
	while (copy_index < target_size)
	{
		resized_string[copy_index] = ' ';
		copy_index++;
	}
	
	/* Null-terminate the string */
	resized_string[target_size] = '\0';
	
	/* Free the original string if it exists */
	if (original_string)
		free(original_string);
	
	return (resized_string);
}

/* ************************************************************************** */
/*                           STRING JOINING UTILITIES                        */
/* ************************************************************************** */

/**
 * @brief Join two strings with a forward slash separator
 */
char	*join_strings_with_separator(char *first_string, char *second_string)
{
	int		total_length;
	int		copy_index;
	char	*joined_string;

	copy_index = 0;
	
	/* Calculate total length needed (excluding newlines, plus separator and null terminator) */
	total_length = get_string_length_no_newline(first_string) + 
		get_string_length_no_newline(second_string);
	
	/* Allocate memory for joined string (+2 for separator and null terminator) */
	joined_string = malloc(sizeof(char) * (total_length + 2));
	if (!joined_string)
		return (NULL);
	
	/* Copy first string if it exists */
	while (first_string && first_string[copy_index])
	{
		joined_string[copy_index] = first_string[copy_index];
		copy_index++;
	}
	
	/* Append second string (excluding newline) */
	while (*second_string && *second_string != '\n')
	{
		joined_string[copy_index] = *second_string;
		copy_index++;
		second_string++;
	}
	
	/* Add separator character */
	joined_string[total_length] = '/';
	
	/* Null-terminate the result */
	joined_string[total_length + 1] = '\0';
	
	/* Free the first string if it was provided */
	if (first_string)
		free(first_string);
	
	return (joined_string);
}

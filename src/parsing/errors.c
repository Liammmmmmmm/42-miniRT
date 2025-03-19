/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:21:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 14:19:28 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	invalid_struct_error(t_objects type, char **splited)
{
	if (type == AMBIANT_LIGHT)
		print_error("Invalid format for ambient lighting. Expected: "CYAN"A "
			BLUE"<ratio> <r,g,b>"NC);
	else if (type == CAMERA)
		print_error("Invalid format for camera. Expected: "CYAN"C "BLUE
			"<x,y,z> <x,y,z> <fov>"NC);
	else if (type == LIGHT)
		print_error("Invalid format for light. Expected: "CYAN"L "
			BLUE"<x,y,z> <brightness ratio> <r,g,b>"NC);
	else if (type == SPHERE)
		print_error("Invalid format for sphere. Expected: "CYAN"sp "
			BLUE"<x,y,z> <diameter> <r,g,b>"NC);
	else if (type == PLANE)
		print_error("Invalid format for plane. Expected: "CYAN"pl "
			BLUE"<x,y,z> <x,y,z> <r,g,b>"NC);
	else if (type == CYLINDER)
		print_error("Invalid format for plane. Expected: "CYAN"cy "
			BLUE"<x,y,z> <x,y,z> <diameter> <height> <r,g,b>"NC);
	free(splited);
	return (0);
}

int	invalid_float_error(char **splited, int i)
{
	ft_dprintf(2, RED"[Error]"NC" Invalid float: `%s'\n", splited[i]);
	free(splited);
	return (0);
}

int	print_line_error(int nb, char *line)
{
	remove_useless_spaces(line);
	ft_dprintf(2, RED"[Error]"NC" Syntax error on line %d: `%s'\n", nb, line);
	free(line);
	return (0);
}

int	to_many_el_error(char *el)
{
	ft_dprintf(2, RED"[Error]"NC" To many elements '%s'\n", el);
	return (0);
}

int	not_enough_el_error(char *el)
{
	ft_dprintf(2, RED"[Error]"NC" No element '%s'. Need at least one.\n", el);
	return (0);
}

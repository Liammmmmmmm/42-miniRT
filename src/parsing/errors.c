/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:21:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/14 13:18:40 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	invalid_struct_error(t_objects type, char **splited)
{
	if (type == AMBIANT_LIGHT)
		print_error(ERR_F" ambient lighting. Expected: A "BLUE
			"<ratio> <r,g,b> [r,g,b|texture]"NC);
	else if (type == CAMERA)
		print_error(ERR_F" camera. Expected: C "BLUE"<x,y,z> <x,y,z> <fov>"NC);
	else if (type == LIGHT)
		print_error(ERR_F" light. Expected: L "BLUE
			"<x,y,z> <brightness ratio> <r,g,b>"NC);
	else if (type == SPHERE)
		print_error(ERR_F" sphere. Expected: sp "BLUE
			"<x,y,z> <diameter> <r,g,b|material>"NC);
	else if (type == PLANE)
		print_error(ERR_F" plane. Expected: pl "BLUE
			"<x,y,z> <x,y,z> <r,g,b|material>"NC);
	else if (type == CYLINDER)
		print_error(ERR_F" plane. Expected: cy "BLUE
			"<x,y,z> <x,y,z> <diameter> <height> <r,g,b|material>"NC);
	else if (type == MATERIAL)
		print_error(ERR_F" material. Expected: mat "BLUE"<name> <albedo>"\
" <metallic> <roughness> <ior> <transmission> <emission_strength> "\
"<emission_color> [normal_map]"NC);
	else if (type == TEXTURE)
		print_error(ERR_F" texture. Expected: tex "BLUE"<name> <filepath>"NC);
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

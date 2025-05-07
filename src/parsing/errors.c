/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:21:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/05 19:02:21 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

int	invalid_struct_error(t_objects type, char **splited)
{
	if (type == AMBIANT_LIGHT)
		print_error(ERR_AMBIENT_LIGHT_FORMAT);
	else if (type == CAMERA)
		print_error(ERR_CAMERA_FORMAT);
	else if (type == LIGHT)
		print_error(ERR_LIGHT_FORMAT);
	else if (type == SPHERE)
		print_error(ERR_SPHERE_FORMAT);
	else if (type == PLANE)
		print_error(ERR_PLANE_FORMAT);
	else if (type == HYPERBOLOID)
		print_error(ERR_HYPERBOLOID_FORMAT);
	else if (type == CUSTOM)
		print_error(ERR_HYPERBOLOID_FORMAT); //changer sa
	else if (type == CYLINDER)
		print_error(ERR_CYLINDER_FORMAT);
	else if (type == CONE)
		print_error(ERR_CONE_FORMAT);
	else if (type == MATERIAL)
		print_error(ERR_MATERIAL_FORMAT);
	else if (type == TEXTURE)
		print_error(ERR_TEXTURE_FORMAT);
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

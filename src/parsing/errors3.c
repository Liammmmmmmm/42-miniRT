/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:08:52 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 14:10:30 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

static void	more_types(t_objects type)
{
	if (type == MATERIAL)
		print_error(ERR_MATERIAL_FORMAT);
	else if (type == TEXTURE)
		print_error(ERR_TEXTURE_FORMAT);
	else if (type == WINDOW)
		print_error(ERR_WINDOW_FORMAT);
}

int	invalid_struct_error(t_objects type, char **splited)
{
	if (type == AMBIANT_LIGHT)
		print_error(ERR_AMBIENT_LIGHT_FORMAT);
	else if (type == CAMERA)
		print_error(ERR_CAMERA_FORMAT);
	else if (type == LIGHT)
		print_error(ERR_LIGHT_FORMAT);
	else if (type == DIRECTIONAL_LIGHT)
		print_error(ERR_DLIGHT_FORMAT);
	else if (type == SPHERE)
		print_error(ERR_SPHERE_FORMAT);
	else if (type == PLANE)
		print_error(ERR_PLANE_FORMAT);
	else if (type == HYPERBOLOID)
		print_error(ERR_HYPERBOLOID_FORMAT);
	else if (type == CUSTOM)
		print_error(ERR_CUSTOM_FORMAT);
	else if (type == CYLINDER)
		print_error(ERR_CYLINDER_FORMAT);
	else if (type == CONE)
		print_error(ERR_CONE_FORMAT);
	more_types(type);
	free(splited);
	return (0);
}

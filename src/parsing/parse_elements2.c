/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:00:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 14:25:30 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_ambiant_light(t_scene *scene, char *line)
{
	char	**parts;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 3)
		return (invalid_struct_error(AMBIANT_LIGHT, parts));
	if (!is_valid_float(parts[1]))
		return (invalid_float_error(parts, 1));
	scene->amb_light.ratio = atof(parts[1]);
	if (scene->amb_light.ratio < 0.0 || scene->amb_light.ratio > 1.0)
	{
		free(parts);
		return (print_error("Invalid ambient light ratio."
			" Expected a value between 0.0 and 1.0"));
	}
	if (!parse_color(parts[2], &scene->amb_light.color))
		return (invalid_struct_error(AMBIANT_LIGHT, parts));
	free(parts);
	return (1);
}

/*

typedef struct s_camera
{
	t_vec3			position;
	t_vec3			orientation;
	unsigned char	fov; 
}	t_camera;

*/

int	parse_camera(t_scene *scene, char *line)
{
	char	**parts;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(CAMERA, parts));
	if (!parse_vector(parts[1], &scene->camera.position))
		return (invalid_struct_error(CAMERA, parts));
	if (!parse_vector_normalized(parts[2], &scene->camera.orientation))
		return (invalid_struct_error(CAMERA, parts));
	if (!is_valid_fov(parts[3], &scene->camera.fov))
	{
		free(parts);
		return (print_error("Invalid FOV. Expected a value between 0 and 180"));
	}
	free(parts);
	return (1);
}

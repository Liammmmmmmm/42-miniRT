/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:00:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/20 15:53:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_ambiant_light(t_scene *scene, char *line)
{
	char	**parts;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) < 3)
		return (invalid_struct_error(AMBIANT_LIGHT, parts));
	if (!is_valid_float(parts[1]))
		return (invalid_float_error(parts, 1));
	scene->amb_light.ratio = ft_atod(parts[1]);
	if (scene->amb_light.ratio < 0.0 || scene->amb_light.ratio > 1.0)
	{
		free(parts);
		return (print_error("Invalid ambient light ratio."
				" Expected a value between 0.0 and 1.0"));
	}
	if (!parse_color(parts[2], &scene->amb_light.color))
		return (invalid_struct_error(AMBIANT_LIGHT, parts));
	if (char_tab_len(parts) == 4)
	{
		if (!parse_color_or_tex(parts[3], &scene->amb_light.skybox_c, &scene->amb_light.skybox_t, scene))
			return (invalid_struct_error(AMBIANT_LIGHT, parts));
		scene->amb_light.have_skybox = 1;
		if (scene->amb_light.skybox_t && !scene->amb_light.skybox_t->img.pixel_data)
			scene->amb_light.skybox_t = NULL;
	}
	free(parts);
	return (1);
}

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

int	parse_light(t_scene *scene, char *line)
{
	char	**parts;
	t_light	*light;

	light = ft_calloc(sizeof(t_light), 1);
	if (!light)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = LIGHT;
	scene->elements[scene->el_amount++].object = light;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(LIGHT, parts));
	if (!parse_vector(parts[1], &light->position))
		return (invalid_struct_error(LIGHT, parts));
	if (!is_valid_double_el(parts[2], &light->brightness))
	{
		free(parts);
		return (print_error("Invalid light ratio."
				" Expected a value between 0.0 and 1.0"));
	}
	if (!parse_color(parts[3], &light->color))
		return (invalid_struct_error(LIGHT, parts));
	free(parts);
	return (1);
}

int	parse_sphere(t_scene *scene, char *line)
{
	char		**parts;
	t_sphere	*sphere;

	sphere = ft_calloc(sizeof(t_sphere), 1);
	if (!sphere)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = SPHERE;
	scene->elements[scene->el_amount++].object = sphere;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(SPHERE, parts));
	if (!parse_vector(parts[1], &sphere->position))
		return (invalid_struct_error(SPHERE, parts));
	if (!is_valid_size(parts[2], &sphere->diameter))
		return (invalid_size_error(parts));
	sphere->radius = sphere->diameter * 0.5;
	sphere->sqrt_radius = sphere->radius * sphere->radius;
	if (!parse_color_or_mat(parts[3], &sphere->color, &sphere->material, scene))
		return (invalid_struct_error(SPHERE, parts));
	free(parts);
	return (1);
}

int	parse_plane(t_scene *scene, char *line)
{
	char	**parts;
	t_plane	*plane;

	plane = ft_calloc(sizeof(t_plane), 1);
	if (!plane)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = PLANE;
	scene->elements[scene->el_amount++].object = plane;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(PLANE, parts));
	if (!parse_vector(parts[1], &plane->position))
		return (invalid_struct_error(PLANE, parts));
	if (!parse_vector_normalized(parts[2], &plane->normal))
		return (invalid_struct_error(PLANE, parts));
	if (!parse_color_or_mat(parts[3], &plane->color, &plane->material, scene))
		return (invalid_struct_error(PLANE, parts));
	free(parts);
	return (1);
}

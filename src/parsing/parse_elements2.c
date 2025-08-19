/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:00:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/19 16:55:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"
#include "importance_sampling.h"

int	parse_ambiant_light(t_scene *scene, char *line)
{
	char	**parts;	

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (!parse_ambient_light_ratio_and_color(scene, parts))
		return (0);
	if (((scene->amb_light.skybox_t && scene->amb_light.skybox_t->type == IMAGE)
			|| !scene->amb_light.skybox_t) && scene->amb_light.ratio < 0.0)
	{
		free(parts);
		return (print_error(MSG_ERR_AMBIENT_LIGHT_RATIO));
	}
	free(parts);
	if (scene->amb_light.skybox_t)
	{
		make_importance_sampling_map(scene);
		scene->amb_light.last_calc_importance = scene->amb_light.skybox_t;
	}
	return (1);
}

int	parse_camera(t_scene *scene, char *line)
{
	char	**parts;
	int		nb_parts;

	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	nb_parts = char_tab_len(parts);
	if (nb_parts < 4 || nb_parts > 6)
		return (invalid_struct_error(CAMERA, parts));
	if (!parse_vector(parts[1], &scene->camera.position))
		return (invalid_struct_error(CAMERA, parts));
	if (!parse_vector_normalized(parts[2], &scene->camera.orientation))
		return (invalid_struct_error(CAMERA, parts));
	if (!is_valid_fov(parts[3], &scene->camera.fov))
	{
		free(parts);
		return (print_error(ERR_INVALID_FOV));
	}
	if (nb_parts > 4 && !is_valid_size(parts[4], &scene->camera.defocus_angle))
		return (invalid_struct_error(CAMERA, parts));
	if (nb_parts > 5 && !is_valid_size(parts[5], &scene->camera.focus_dist))
		return (invalid_struct_error(CAMERA, parts));
	free(parts);
	return (1);
}

static int	parse_light_part2(t_light *light, char **parts, int nb_parts)
{
	double	tmpd;

	if (!is_valid_double_el(parts[2], &light->brightness))
	{
		free(parts);
		return (print_error(ERR_INVALID_LIGHT_RATIO));
	}
	if (nb_parts < 5)
		return (1);
	if (!is_valid_size(parts[4], &tmpd))
	{
		free(parts);
		return (print_error(ERR_INVALID_LIGHT_RADIUS));
	}
	light->radius = tmpd;
	light->shadow_sample = 5;
	if (nb_parts < 6)
		return (1);
	if (!is_valid_positive_int(parts[5], &light->shadow_sample))
	{
		free(parts);
		return (print_error(ERR_INVALID_LIGHT_SHADOW_SAMPLE));
	}
	return (1);
}

int	parse_light(t_scene *scene, char *line)
{
	char	**parts;
	t_light	*light;
	int		nb_parts;

	light = ft_calloc(sizeof(t_light), 1);
	if (!light)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = LIGHT;
	scene->elements[scene->el_amount++].object = light;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	nb_parts = char_tab_len(parts);
	if (nb_parts < 4 || nb_parts > 6)
		return (invalid_struct_error(LIGHT, parts));
	if (!parse_vector(parts[1], &light->position))
		return (invalid_struct_error(LIGHT, parts));
	if (!parse_color(parts[3], &light->color))
		return (invalid_struct_error(LIGHT, parts));
	if (parse_light_part2(light, parts, nb_parts) == 0)
		return (0);
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

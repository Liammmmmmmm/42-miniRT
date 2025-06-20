/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:29:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 13:42:46 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "importance_sampling.h"

static int	cmp_elements(t_scene *scene, char *line)
{
	int	valid;

	valid = is_valid_element(line);
	if ((cmp_type("sp", line) && parse_sphere(scene, line))
		|| (cmp_type("pl", line) && parse_plane(scene, line))
		|| (cmp_type("cy", line) && parse_cylinder(scene, line))
		|| (cmp_type("co", line) && parse_cone(scene, line))
		|| (cmp_type("hb", line) && parse_hyperboloid(scene, line))
		|| (cmp_type("cu", line) && parse_obj_custom(scene, line))
		|| (cmp_type("A", line) && parse_ambiant_light(scene, line))
		|| (cmp_type("C", line) && parse_camera(scene, line))
		|| (cmp_type("L", line) && parse_light(scene, line))
		|| (cmp_type("l", line) && parse_light(scene, line))
		|| (cmp_type("dl", line) && parse_dlight(scene, line))
		|| (cmp_type("tex", line) && parse_texture(scene, line))
		|| (cmp_type("mat", line) && parse_material(scene, line))
		|| (cmp_type("W", line) && parse_win_size(scene, line)))
		return (1);
	else if (!valid)
		ft_dprintf(2, RED"[Error]"NC" Invalid parameters for `%s'\n", line);
	return (0);
}

int	parse_elements(t_scene *scene, char **lines, int ln_amount)
{
	int	i;

	scene->build_bvh = 1;
	scene->el_amount = 0;
	scene->elements = ft_calloc(sizeof(t_object), ln_amount - 2
			- scene->tex_amount - scene->mat_amount - scene->have_win_el);
	if (!scene->elements)
		return (free_scene(scene, lines));
	i = -1;
	while (lines[++i])
	{
		if (cmp_elements(scene, lines[i]))
			continue ;
		return (free_scene(scene, lines));
	}
	scene->el_amount = ln_amount - 2 - scene->tex_amount - scene->mat_amount
		- scene->have_win_el;
	return (1);
}

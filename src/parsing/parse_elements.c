/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:29:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/11 16:04:29 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_tex_mat(t_scene *scene)
{
	int	i;

	if (scene->textures)
	{
		i = -1;
		while (++i < scene->tex_amount)
			free(scene->textures[i].img.pixel_data);
		free(scene->textures);
		scene->textures = NULL;
	}
	free(scene->materials);
	scene->materials = NULL;
}

int	free_scene(t_scene *scene, char **lines)
{
	int	i;

	if (lines)
		ft_free_tab_null_term(lines);
	if (scene->elements)
	{
		i = -1;
		while (++i < scene->el_amount)
		{
			free(scene->elements[i].object);
			scene->elements[i].object = NULL;
			scene->elements[i].type = NULL_OBJ;
		}
		free(scene->elements);
		scene->elements = NULL;
	}
	free_tex_mat(scene);
	return (0);
}

static int	cmp_elements(t_scene *scene, char *line)
{
	int	valid;

	valid = is_valid_element(line);
	if (cmp_type("sp", line) && parse_sphere(scene, line))
		return (1);
	else if (cmp_type("pl", line) && parse_plane(scene, line))
		return (1);
	else if (cmp_type("cy", line) && parse_cylinder(scene, line))
		return (1);
	else if (cmp_type("A", line) && parse_ambiant_light(scene, line))
		return (1);
	else if (cmp_type("C", line) && parse_camera(scene, line))
		return (1);
	else if (cmp_type("L", line) && parse_light(scene, line))
		return (1);
	else if (cmp_type("tex", line) && parse_texture(scene, line))
		return (1);
	else if (cmp_type("mat", line) && parse_material(scene, line))
		return (1);
	else if (!valid)
		ft_dprintf(2, RED"[Error]"NC" Invalid parameters for `%s'\n", line);
	return (0);
}

int	parse_elements(t_scene *scene, char **lines, int ln_amount)
{
	int	i;

	scene->el_amount = 0;
	scene->elements = ft_calloc(sizeof(t_object), ln_amount - 2
			- scene->tex_amount - scene->mat_amount);
	if (!scene->elements)
		return (free_scene(scene, lines));
	i = -1;
	while (lines[++i])
	{
		if (cmp_elements(scene, lines[i]))
			continue ;
		return (free_scene(scene, lines));
	}
	scene->el_amount = ln_amount - 2 - scene->tex_amount - scene->mat_amount;
	return (1);
}

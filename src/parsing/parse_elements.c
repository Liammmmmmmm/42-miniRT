/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:29:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 11:00:23 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	free_scene(t_scene *scene, char **lines)
{
	int	i;

	if (lines)
		ft_free_tab_null_term(lines);
	if (scene->elements)
	{
		i = 0;
		while (i < scene->el_amount)
		{
			free(scene->elements[i].object);
			i++;
		}
		free(scene->elements);
	}
	return (0);
}

static int	cmp_elements(t_scene *scene, char *line)
{
	if (cmp_type("sp", line))
		if (parse_camera(scene, line))
			return (1);
	if (cmp_type("pl", line))
		if (parse_camera(scene, line))
			return (1);
	if (cmp_type("cy", line))
		if (parse_camera(scene, line))
			return (1);
	if (cmp_type("A", line))
		if (parse_ambiant_light(scene, line))
			return (1);
	if (cmp_type("C", line))
		if (parse_camera(scene, line))
			return (1);
	if (cmp_type("L", line))
		if (parse_camera(scene, line))
			return (1);
	return (0);
}

int	parse_elements(t_scene *scene, char **lines, int ln_amount)
{
	int	i;

	scene->el_amount = ln_amount;
	scene->elements = ft_calloc(sizeof(t_object), ln_amount - 2);
	if (!scene->elements)
		return (free_scene(scene, lines));
	i = -1;
	while (lines[++i])
	{
		if (cmp_elements(scene, lines[i]))
			continue ;
		return (free_scene(scene, lines));
	}
}

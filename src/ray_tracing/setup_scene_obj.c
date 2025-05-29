/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene_obj.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:28:49 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/28 18:24:46 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

void	clear_obj_lists(t_minirt *minirt)
{
	if (minirt->scene.obj_lst.light_lst)
		free(minirt->scene.obj_lst.light_lst);
	if (minirt->scene.obj_lst.plane_lst)
		free(minirt->scene.obj_lst.plane_lst);
	minirt->scene.obj_lst.light_lst = NULL;
	minirt->scene.obj_lst.plane_lst = NULL;
	minirt->scene.obj_lst.light_nb = 0;
	minirt->scene.obj_lst.plane_nb = 0;
}

void	count_elements(t_minirt *minirt, int *light_count, int *plane_count)
{
	int	i;

	i = 0;
	*light_count = 0;
	*plane_count = 0;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == LIGHT)
			(*light_count)++;
		if (minirt->scene.elements[i].type == DIRECTIONAL_LIGHT)
			(*light_count)++;
		else if (minirt->scene.elements[i].type == PLANE)
			(*plane_count)++;
		i++;
	}
}

void	fill_obj_lists(t_minirt *minirt)
{
	int	i;

	i = 0;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == LIGHT)
			minirt->scene.obj_lst.light_lst[minirt->scene.obj_lst.light_nb++]
				= &minirt->scene.elements[i];
		if (minirt->scene.elements[i].type == DIRECTIONAL_LIGHT)
			minirt->scene.obj_lst.light_lst[minirt->scene.obj_lst.light_nb++]
				= &minirt->scene.elements[i];
		else if (minirt->scene.elements[i].type == PLANE)
			minirt->scene.obj_lst.plane_lst[minirt->scene.obj_lst.plane_nb++]
				= &minirt->scene.elements[i];
		i++;
	}
}

int	init_plane_light_lst(t_minirt *minirt)
{
	int	light_c;
	int	plane_c;

	clear_obj_lists(minirt);
	count_elements(minirt, &light_c, &plane_c);
	if (light_c != 0)
	{
		minirt->scene.obj_lst.light_lst = malloc(sizeof(t_object *) * light_c);
		if (!minirt->scene.obj_lst.light_lst)
		{
			ft_dprintf(2, "malloc failed in init_plane_light_lst\n");
			return (1);
		}
	}
	if (plane_c != 0)
	{
		minirt->scene.obj_lst.plane_lst = malloc(sizeof(t_object *) * plane_c);
		if (!minirt->scene.obj_lst.plane_lst)
		{
			ft_dprintf(2, "malloc failed in init_plane_light_lst\n");
			return (1);
		}
	}
	fill_obj_lists(minirt);
	return (0);
}

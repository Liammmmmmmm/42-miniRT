/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_object.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:50:19 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 11:31:02 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	free_element_content(t_object *object, t_scene *scene)
{
	if (!object || !object->object)
		return;
	if (object->type == CUSTOM)
	{
		free(((t_custom_object *)object->object)->name);
		free(((t_custom_object *)object->object)->obj_list);
		free(((t_custom_object *)object->object)->triangles);
	}
	if (object->type == PLANE)
		scene->obj_lst.plane_nb--;
	else if (object->type == LIGHT)
		scene->obj_lst.light_nb--;
	free(object->object);
	object->object = NULL;
}

static int	compact_elements_array(t_minirt *minirt, t_object *selected_obj)
{
	int	read_idx;
	int	write_idx;

	read_idx = 0;
	write_idx = 0;
	while (read_idx < minirt->scene.el_amount)
	{
		if (&minirt->scene.elements[read_idx] == selected_obj)
			free_element_content(&minirt->scene.elements[read_idx], &minirt->scene);
		else
		{
			minirt->scene.elements[write_idx] =
				minirt->scene.elements[read_idx];
			write_idx++;
		}
		read_idx++;
	}
	return (write_idx);
}

void	rm_object(void *vparam)
{
	t_minirt	*minirt;
	t_object	*selected_obj;
	int			new_size;

	minirt = ((t_btn_param *)vparam)->minirt;
	selected_obj = minirt->controls.ui_infos.selected_object;
	if (minirt->scene.el_amount <= 0 || !selected_obj)
		return ;
	new_size = compact_elements_array(minirt, selected_obj);
	if (new_size < minirt->scene.el_amount)
	{
		minirt->controls.ui_infos.selected_object = NULL;
		minirt->scene.el_amount = new_size;
		stop_minirt(minirt);
		minirt->scene.build_bvh = 1;
	}
}

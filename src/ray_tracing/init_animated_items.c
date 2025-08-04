/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_animated_items.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:42:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/26 17:01:18 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec3	*get_obj_pos(t_object *obj)
{
	if (obj->type == SPHERE)
		return (&((t_sphere *)obj->object)->position);
	else if (obj->type == PLANE)
		return (&((t_plane *)obj->object)->position);
	else if (obj->type == CYLINDER)
		return (&((t_cylinder *)obj->object)->position);
	else if (obj->type == CONE)
		return (&((t_cone *)obj->object)->position);
	else if (obj->type == HYPERBOLOID)
		return (&((t_hyperboloid *)obj->object)->position);
	else if (obj->type == CUSTOM)
		return (&((t_custom_object *)obj->object)->position);
	else if (obj->type == LIGHT)
		return (&((t_light *)obj->object)->position);
	return (NULL);
}

t_vec3	*get_obj_rot(t_object *obj)
{
	if (obj->type == PLANE)
		return (&((t_plane *)obj->object)->normal);
	else if (obj->type == CYLINDER)
		return (&((t_cylinder *)obj->object)->orientation);
	else if (obj->type == CONE)
		return (&((t_cone *)obj->object)->orientation);
	else if (obj->type == HYPERBOLOID)
		return (&((t_hyperboloid *)obj->object)->orientation);
	else if (obj->type == CUSTOM)
		return (&((t_custom_object *)obj->object)->orientation);
	return (NULL);
}

static void	find_item(t_minirt *minirt, t_uint i, t_uint *obj_num, int y)
{
	if (minirt->scene.elements[y].type == minirt->options.anim.objects[i].obj)
	{
		if (*obj_num == minirt->options.anim.objects[i].obj_num)
		{
			if (get_obj_pos(&minirt->scene.elements[y]))
				*get_obj_pos(&minirt->scene.elements[y]) = minirt->\
				options.anim.objects[i].points[minirt->options.anim.frame_i];
			if (get_obj_rot(&minirt->scene.elements[y]))
				*get_obj_rot(&minirt->scene.elements[y]) = minirt->\
			options.anim.objects[i].orientations[minirt->options.anim.frame_i];
			minirt->scene.build_bvh = 1;
			printf("%s\n",
				((t_custom_object *)minirt->scene.elements[y].object)->name);
		}
		(*obj_num)++;
	}
}

static void	item_itteration(t_minirt *minirt, t_uint i)
{
	t_uint	obj_num;
	int		y;

	if (minirt->options.anim.objects[i].obj == CAMERA)
	{
		minirt->scene.camera.position = minirt->\
		options.anim.objects[i].points[minirt->options.anim.frame_i];
		minirt->scene.camera.orientation = minirt->\
		options.anim.objects[i].orientations[minirt->options.anim.frame_i];
	}
	else
	{
		obj_num = 0;
		y = -1;
		while (++y < minirt->scene.el_amount)
		{
			find_item(minirt, i, &obj_num, y);
		}
	}
}

void	init_animated_items(t_minirt *minirt)
{
	t_uint	i;

	if (!minirt->options.anim.enabled
		|| minirt->options.anim.frame_i > minirt->options.anim.frames)
		return ;
	i = (t_uint)-1;
	while (++i < minirt->options.anim.nb_objects)
		item_itteration(minirt, i);
}

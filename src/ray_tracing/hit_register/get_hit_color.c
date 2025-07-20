/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_hit_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:50:29 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/17 11:51:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "hit_register.h"

t_fcolor	get_hit_register_color(t_mat *mat, t_color color,
	t_hit_record *hit, t_bvh *bvh)
{
	if (*bvh->render_mode == 2)
	{
		return ((t_fcolor){
			.r = (hit->normal.x + 1) * 0.5,
			.g = (hit->normal.z + 1) * 0.5,
			.b = (hit->normal.y + 1) * 0.5
		});
	}
	if (mat)
	{
		if (mat->color_tex)
			return (get_tex_color(mat->color_tex, hit->u, hit->v, hit->point));
		else
			return (color_to_fcolor(mat->color_value));
	}
	return (color_to_fcolor(color));
}

inline t_color	get_obj_color(t_object *obj)
{
	if (obj->type == SPHERE)
		return (((t_sphere *)obj->object)->color);
	else if (obj->type == PLANE)
		return (((t_plane *)obj->object)->color);
	else if (obj->type == CYLINDER)
		return (((t_cylinder *)obj->object)->color);
	else if (obj->type == CONE)
		return (((t_cone *)obj->object)->color);
	else if (obj->type == HYPERBOLOID)
		return (((t_hyperboloid *)obj->object)->color);
	else if (obj->type == TRIANGLE)
		return (((t_triangle *)obj->object)->color);
	return ((t_color){0, 0, 0});
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:22:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/23 16:05:59 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "obj_parsing.h"
#include "maths.h"

static inline void	set_vertex(t_aabb *aabb, t_vertex *verts)
{
	size_t	j;
	t_vec3	p;

	j = 0;
	while (j < 3)
	{
		p = verts[j].pos;
		if (p.x < aabb->min.x)
			aabb->min.x = p.x;
		if (p.y < aabb->min.y)
			aabb->min.y = p.y;
		if (p.z < aabb->min.z)
			aabb->min.z = p.z;
		if (p.x > aabb->max.x)
			aabb->max.x = p.x;
		if (p.y > aabb->max.y)
			aabb->max.y = p.y;
		if (p.z > aabb->max.z)
			aabb->max.z = p.z;
		j++;
	}
}

t_aabb	update_custom_obj_aabb(t_custom_object *obj)
{
	const t_vec3	delta = vec3_subtract(obj->position, obj->prev_position);
	t_aabb			aabb;

	aabb = obj->aabb;
	aabb.min = vec3_add(aabb.min, delta);
	aabb.max = vec3_add(aabb.max, delta);
	return (aabb);
}

t_aabb	compute_custom_obj(t_custom_object *obj, char init)
{
	t_aabb		aabb;
	t_vertex	verts[3];
	size_t		i;

	if (!init)
		return (update_custom_obj_aabb(obj));
	i = 0;
	if (!obj || obj->triangle_count == 0 || !obj->triangles)
		return ((t_aabb){.min = {0, 0, 0}, .max = {0, 0, 0}});
	aabb.min = obj->triangles[0].v0.pos;
	aabb.max = obj->triangles[0].v0.pos;
	while (i < obj->triangle_count)
	{
		verts[0] = obj->triangles[i].v0;
		verts[1] = obj->triangles[i].v1;
		verts[2] = obj->triangles[i].v2;
		set_vertex(&aabb, verts);
		i++;
	}
	return (aabb);
}

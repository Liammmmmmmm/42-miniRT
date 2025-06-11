/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_finilizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:54:41 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 21:01:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "maths.h"
#include "minirt.h"

static t_vec3	rotate_point_by_orientation(t_vec3 point, t_vec3 orientation)
{
	t_vec3	base_vector;
	t_vec3	rotation_axis;
	float	rotation_angle;

	base_vector = (t_vec3){0.0f, 0.0f, 1.0f};
	orientation = vec3_unit(orientation);
	rotation_angle = acosf(vec3_dot(base_vector, orientation));
	rotation_axis = vec3_cross(base_vector, orientation);
	if (vec3_length(rotation_axis) < 0.0001f)
	{
		if (rotation_angle > PI_D - 0.01f)
			return (rotate_around_axis(point, (t_vec3){1.0f, 0.0f, 0.0f},
			PI_D));
		return (point);
	}
	rotation_axis = vec3_unit(rotation_axis);
	return (rotate_around_axis(point, rotation_axis, rotation_angle));
}

static void	apply_transformations(t_vertex_indexed *v, t_custom_object *obj)
{
	v->pos = vec3_multiply(v->pos, obj->scale);
	v->pos = rotate_point_by_orientation(v->pos, obj->orientation);
	v->pos = vec3_add(v->pos, obj->position);
}

int	flatten_mesh_to_object(t_custom_object *obj, t_mesh *mesh)
{
	size_t				i;
	t_triangle			*tri;
	t_vertex_indexed	v[3];
	uint32_t			idx[3];

	obj->triangle_count = mesh->index_count / 3;
	obj->triangles = malloc(sizeof(t_triangle) * obj->triangle_count);
	obj->obj_list = malloc(sizeof(t_object) * obj->triangle_count);
	if (!obj->triangles || !obj->obj_list)
		return (1);
	i = 0;
	while (i < obj->triangle_count)
	{
		tri = &obj->triangles[i];
		idx[0] = mesh->indices[i * 3 + 0];
		idx[1] = mesh->indices[i * 3 + 1];
		idx[2] = mesh->indices[i * 3 + 2];
		v[0] = mesh->vertices[idx[0]];
		v[1] = mesh->vertices[idx[1]];
		v[2] = mesh->vertices[idx[2]];
		apply_transformations(&v[0], obj);
		apply_transformations(&v[1], obj);
		apply_transformations(&v[2], obj);
		tri->v0 = (t_vertex){v[0].pos, v[0].normal, v[0].uv.x, v[0].uv.y};
		tri->v1 = (t_vertex){v[1].pos, v[1].normal, v[1].uv.x, v[1].uv.y};
		tri->v2 = (t_vertex){v[2].pos, v[2].normal, v[2].uv.x, v[2].uv.y};
		tri->center = vec3_divide_scalar(vec3_add(vec3_add(v[0].pos,
						v[1].pos), v[2].pos), 3.0);
		tri->material = obj->material;
		tri->color = obj->color;
		tri->obj = obj;
		obj->obj_list[i] = (t_object){tri, TRIANGLE};
		i++;
	}
	obj->name = mesh->name;
	obj->prev_position = obj->position;
	obj->prev_orientation = obj->orientation;
	obj->prev_scale = obj->scale;
	obj->aabb = compute_custom_obj(obj, 1);
	return (0);
}

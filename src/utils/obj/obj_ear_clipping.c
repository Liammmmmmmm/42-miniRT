/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_ear_clipping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:18:13 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "obj_parsing.h"

static inline void	add_position_scale(t_triangle *t, t_custom_object *obj)
{
	const t_vec3	axis_raw = vec3_cross((t_vec3){0.0f, 0.0f, 1.0f},
			obj->orientation);
	const float		angle = acosf(vec3_dot((t_vec3){0.0f, 0.0f, 1.0f},
				obj->orientation));
	t_vec3			axis;

	axis = axis_raw;
	if (vec3_length(axis_raw) > 0.0001f)
		axis = vec3_unit(axis_raw);
	t->v0.pos = vec3_multiply(t->v0.pos, obj->scale);
	t->v1.pos = vec3_multiply(t->v1.pos, obj->scale);
	t->v2.pos = vec3_multiply(t->v2.pos, obj->scale);
	if (angle > 0.0001f)
	{
		t->v0.pos = rotate_around_axis(t->v0.pos, axis, angle);
		t->v1.pos = rotate_around_axis(t->v1.pos, axis, angle);
		t->v2.pos = rotate_around_axis(t->v2.pos, axis, angle);
	}
	t->v0.pos = vec3_add(t->v0.pos, obj->position);
	t->v1.pos = vec3_add(t->v1.pos, obj->position);
	t->v2.pos = vec3_add(t->v2.pos, obj->position);
}

static inline void	compute_triangle_center(t_triangle *t)
{
	t->center = vec3_divide_scalar(
			vec3_add(vec3_add(t->v0.pos, t->v1.pos), t->v2.pos), 3.0);
}

char	add_single_triangle(long *e, t_vertex *vertices, t_vector *v, \
	t_custom_object *obj)
{
	t_triangle	t;

	t = (t_triangle){vertices[0], vertices[1], vertices[2], (t_vec3){0, 0, 0}, \
		NULL, (t_color){0, 0, 0}, obj};
	compute_triangle_center(&t);
	add_position_scale(&t, obj);
	if (vector_add(v, &t) == -1)
	{
		print_error("Vector add in add_single_triangle in ear_clipping.");
		return (1);
	}
	(*e)++;
	return (0);
}

char	init_triangle(t_vertex *vertices, size_t vertex_count, t_vector *v, \
	t_custom_object *obj)
{
	t_triangle	t;

	t = (t_triangle){vertices[(0 + vertex_count - 1) % vertex_count], \
		vertices[0], vertices[(0 + 1) % vertex_count], (t_vec3){0, 0, 0}, \
		NULL, (t_color){0, 0, 0}, obj};
	compute_triangle_center(&t);
	add_position_scale(&t, obj);
	if (vector_add(v, &t) == -1)
		return (print_error1("Vector add in ear_clipping."));
	return (0);
}

long	ear_clipping(t_vertex *vertices, size_t vertex_count, t_vector *v, \
	t_custom_object *obj)
{
	long		e;
	size_t		j;

	e = 0;
	while (vertex_count > 3)
	{
		if (init_triangle(vertices, vertex_count, v, obj))
			return (-1);
		j = 0;
		while (j < vertex_count - 1)
		{
			vertices[j] = vertices[j + 1];
			j++;
		}
		vertex_count--;
		e++;
	}
	if (vertex_count == 3 && add_single_triangle(&e, vertices, v, obj) == 1)
		return (-1);
	return (e);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_ear_clipping.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 19:02:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	compute_triangle_center(t_triangle *t)
{
	t->center = vec3_divide_scalar(
			vec3_add(vec3_add(t->v0.pos, t->v1.pos), t->v2.pos), 3.0);
}

char	add_single_triangle(long *e, t_vertex *vertices, t_vector *v)
{
	t_triangle	t;

	t = (t_triangle){vertices[0], vertices[1], vertices[2], (t_vec3){0, 0, 0}, \
		NULL, (t_color){0, 0, 0}};
	compute_triangle_center(&t);
	if (vector_add(v, &t) == -1)
		return (1);
	(*e)++;
	return (0);
}

long	ear_clipping(t_vertex *vertices, size_t vertex_count, t_vector *v)
{
	long		e;
	size_t		j;
	t_triangle	t;

	e = 0;
	while (vertex_count > 3)
	{
		t = (t_triangle){vertices[(0 + vertex_count - 1) % vertex_count], \
			vertices[0], vertices[(0 + 1) % vertex_count], (t_vec3){0, 0, 0}, \
			NULL, (t_color){0, 0, 0}};
		compute_triangle_center(&t);
		vector_add(v, &t);
		j = 0;
		while (j < vertex_count - 1)
		{
			vertices[j] = vertices[j + 1];
			j++;
		}
		vertex_count--;
		e++;
	}
	if (vertex_count == 3 && add_single_triangle(&e, vertices, v) == 1)
		return (-1);
	return (e);
}

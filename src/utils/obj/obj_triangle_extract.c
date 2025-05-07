/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_triangle_extract.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:02 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 19:04:55 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static long	clip_one_face(t_obj_temp *obj, t_vector *v, size_t i)
{
	t_vertex	*vertex;
	size_t		j;

	vertex = malloc(sizeof(t_vertex) * obj->face[i].vertex_count);
	j = 0;
	while (j < obj->face[i].vertex_count)
	{
		vertex[j].normal = obj->vn[obj->face[i].vn_idx[j] - 1];
		vertex[j].pos = obj->v[obj->face[i].v_idx[j] - 1];
		vertex[j].u = obj->vt[obj->face[i].vt_idx[j] - 1].x;
		vertex[j].v = obj->vt[obj->face[i].vt_idx[j] - 1].y;
		j++;
	}
	j = ear_clipping(vertex, obj->face[i].vertex_count, v);
	free(vertex);
	return (j);
}

int	extract_all_triangle(t_obj_temp *obj, t_triangle **out, \
	size_t *triangle_count, const char *file)
{
	size_t		i;
	size_t		size;
	long		return_value;
	t_vector	v;

	*triangle_count = 0;
	size = 0;
	vector_init(&v, sizeof(t_triangle), 2);
	printf("\nStart parsing obj :"YELLOW" %s\n"NC, file);
	i = 0;
	while (i < obj->face_count)
	{
		return_value = clip_one_face(obj, &v, i);
		if (return_value < 0)
		{
			free(v.data);
			print_error("Error of malloc int extract all triangle\n");
		}
		size += return_value;
		print_progress_bar(i, obj->face_count - 1);
		i++;
	}
	*out = (t_triangle *)v.data;
	*triangle_count = size;
	return (0);
}

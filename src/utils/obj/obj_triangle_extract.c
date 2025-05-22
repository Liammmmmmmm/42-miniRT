/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_triangle_extract.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:32:02 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/22 07:49:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static long	clip_one_face(t_obj_temp *tmp, t_vector *v, size_t i, \
	t_custom_object *obj)
{
	t_vertex	*vertex;
	size_t		j;

	vertex = malloc(sizeof(t_vertex) * tmp->face[i].vertex_count);
	j = 0;
	while (j < tmp->face[i].vertex_count)
	{
		vertex[j].normal = tmp->vn[tmp->face[i].vn_idx[j] - 1];
		vertex[j].pos = tmp->v[tmp->face[i].v_idx[j] - 1];
		vertex[j].u = tmp->vt[tmp->face[i].vt_idx[j] - 1].x;
		vertex[j].v = tmp->vt[tmp->face[i].vt_idx[j] - 1].y;
		j++;
	}
	j = ear_clipping(vertex, tmp->face[i].vertex_count, v, obj);
	free(vertex);
	return (j);
}

int	extract_all_triangle(t_obj_temp *tmp, t_custom_object *obj,
	const char *file)
{
	size_t		i;
	size_t		size;
	long		return_value;
	t_vector	v;

	obj->triangle_count = 0;
	size = 0;
	vector_init(&v, sizeof(t_triangle), 1);
	printf("\nStart parsing obj :"YELLOW" %s\n"NC, file);
	i = 0;
	while (i < tmp->face_count)
	{
		return_value = clip_one_face(tmp, &v, i, obj);
		if (return_value < 0)
		{
			free(v.data);
			print_error("Error of malloc int extract all triangle\n");
		}
		size += return_value;
		print_progress_bar(i, tmp->face_count - 1);
		i++;
	}
	obj->triangles = (t_triangle *)v.data;
	obj->triangle_count = size;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_vertex_cache.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:52:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:55:32 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "minirt.h"

uint32_t	hash_triplet(t_face_idx_triplet t)
{
	uint32_t	hash;

	hash = 2166136261u;
	hash = (hash * 16777619) ^ t.v_idx;
	hash = (hash * 16777619) ^ t.vt_idx;
	hash = (hash * 16777619) ^ t.vn_idx;
	return (hash);
}

size_t	get_hash_map_size(size_t vertex_count)
{
	size_t	size;

	if (vertex_count == 0)
		return (1024);
	size = vertex_count;
	size--;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size |= size >> 32;
	size++;
	if (size < 1024)
		return (1024);
	return (size);
}

static void	finalize_mesh(t_mesh *mesh, t_mesh_build_data *build_data,
	t_parser_temp_data *data)
{
	mesh->vertices = build_data->vertices->data;
	mesh->vertex_count = build_data->vertices->num_elements;
	mesh->indices = build_data->indices->data;
	mesh->index_count = build_data->indices->num_elements;
	mesh->name = data->object_name;
	data->object_name = NULL;
	build_data->vertices->data = NULL;
	build_data->indices->data = NULL;
}

int	build_indexed_mesh(t_mesh *mesh, t_parser_temp_data *data)
{
	t_mesh_build_data		build_data;
	t_face_idx_triplet		*ptr;
	t_face_idx_triplet		key;
	size_t					j;

	if (init_mesh_vectors(&build_data, data) != 0)
		return (1);
	j = 0;
	while (j < data->temp_faces.num_elements)
	{
		ptr = vector_get(&data->temp_faces, j);
		key = normalize_indices(ptr, data);
		if (process_face(key, &build_data, data) != 0)
		{
			cleanup_mesh_data(&build_data);
			return (1);
		}
		j++;
		print_progress_bar(j, data->temp_faces.num_elements);
	}
	free_cache(build_data.cache, build_data.map_size);
	finalize_mesh(mesh, &build_data, data);
	free(build_data.vertices);
	free(build_data.indices);
	return (0);
}

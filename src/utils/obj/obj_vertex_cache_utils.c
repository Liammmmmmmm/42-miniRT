/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_vertex_cache_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:09:48 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:56:33 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "minirt.h"

void	free_cache(t_cache_entry **map, size_t map_size)
{
	size_t			i;
	t_cache_entry	*current;
	t_cache_entry	*to_free;

	i = 0;
	if (!map)
		return ;
	while (i < map_size)
	{
		current = map[i];
		while (current)
		{
			to_free = current;
			current = current->next;
			free(to_free);
		}
		i++;
	}
	free(map);
}

void	cleanup_mesh_data(t_mesh_build_data *build_data)
{
	if (build_data->vertices)
	{
		if (build_data->vertices->data)
			free(build_data->vertices->data);
		free(build_data->vertices);
	}
	if (build_data->indices)
	{
		if (build_data->indices->data)
			free(build_data->indices->data);
		free(build_data->indices);
	}
	if (build_data->cache)
		free_cache(build_data->cache, build_data->map_size);
}

t_face_idx_triplet	normalize_indices(t_face_idx_triplet *ptr,
	t_parser_temp_data *data)
{
	t_face_idx_triplet	key;

	if (ptr->v_idx > 0)
		key.v_idx = ptr->v_idx - 1;
	else
		key.v_idx = (int)data->temp_positions.num_elements + ptr->v_idx;
	if (ptr->vt_idx > 0)
		key.vt_idx = ptr->vt_idx - 1;
	else
		key.vt_idx = (int)data->temp_uvs.num_elements + ptr->vt_idx;
	if (ptr->vn_idx > 0)
		key.vn_idx = ptr->vn_idx - 1;
	else
		key.vn_idx = (int)data->temp_normals.num_elements + ptr->vn_idx;
	return (key);
}

t_vertex_indexed	create_vertex(t_face_idx_triplet key,
	t_parser_temp_data *data)
{
	t_vertex_indexed	new_v;

	if (key.v_idx >= 0 && (size_t)key.v_idx < data->temp_positions.num_elements)
		new_v.pos = *(t_vec3 *)vector_get(&data->temp_positions, key.v_idx);
	else
		new_v.pos = (t_vec3){0, 0, 0};
	if (key.vn_idx >= 0 && (size_t)key.vn_idx < data->temp_normals.num_elements)
		new_v.normal = *(t_vec3 *)vector_get(&data->temp_normals, key.vn_idx);
	else
		new_v.normal = (t_vec3){0, 1, 0};
	if (key.vt_idx >= 0 && (size_t)key.vt_idx < data->temp_uvs.num_elements)
		new_v.uv = *(t_vec2 *)vector_get(&data->temp_uvs, key.vt_idx);
	else
		new_v.uv = (t_vec2){0, 0};
	return (new_v);
}

int	process_face(t_face_idx_triplet key, t_mesh_build_data *build_data,
	t_parser_temp_data *data)
{
	int					found_idx;
	uint32_t			new_idx;
	t_vertex_indexed	new_v;

	found_idx = cache_get(build_data->cache, key, build_data->map_size);
	if (found_idx != -1)
	{
		new_idx = found_idx;
		if (vector_add(build_data->indices, &new_idx) == -1)
			return (print_error1("Vector add in build_indexed_mesh."));
	}
	else
	{
		new_v = create_vertex(key, data);
		new_idx = build_data->vertices->num_elements;
		if (vector_add(build_data->vertices, &new_v) == -1
			|| vector_add(build_data->indices, &new_idx) == -1
			|| cache_add(build_data->cache, key, new_idx, build_data->map_size))
			return (print_error1("Vector add or cache add in process_face"));
	}
	return (0);
}

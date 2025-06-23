/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_vertex_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 17:45:58 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:56:42 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "minirt.h"

char	cache_add(t_cache_entry **map, t_face_idx_triplet key, uint32_t val,
	size_t map_size)
{
	uint32_t		index;
	t_cache_entry	*new_entry;

	index = hash_triplet(key) % map_size;
	new_entry = malloc(sizeof(t_cache_entry));
	if (!new_entry)
		return (1);
	new_entry->key = key;
	new_entry->value = val;
	new_entry->next = map[index];
	map[index] = new_entry;
	return (0);
}

int	cache_get(t_cache_entry **map, t_face_idx_triplet key,
	size_t map_size)
{
	uint32_t		index;
	t_cache_entry	*current;

	index = hash_triplet(key) % map_size;
	current = map[index];
	while (current)
	{
		if (current->key.v_idx == key.v_idx && current->key.vt_idx == key.vt_idx
			&& current->key.vn_idx == key.vn_idx)
			return (current->value);
		current = current->next;
	}
	return (-1);
}

int	init_mesh_vectors(t_mesh_build_data *build_data, t_parser_temp_data *data)
{
	build_data->vertices = malloc(sizeof(t_vector));
	build_data->indices = malloc(sizeof(t_vector));
	if (!build_data->vertices || !build_data->indices)
		return (1);
	if (vector_init(build_data->vertices, sizeof(t_vertex_indexed), \
			data->temp_positions.num_elements) == -1 \
		|| vector_init(build_data->indices, sizeof(uint32_t), \
			data->temp_faces.num_elements) == -1)
	{
		if (build_data->vertices->data)
			free(build_data->vertices->data);
		return (print_error1("Vector init in build_indexed_mesh."));
	}
	build_data->map_size = get_hash_map_size(data->temp_positions.num_elements);
	build_data->cache
		= ft_calloc(build_data->map_size, sizeof(t_cache_entry *));
	if (!build_data->cache)
	{
		free(build_data->vertices->data);
		free(build_data->indices->data);
		return (1);
	}
	return (0);
}

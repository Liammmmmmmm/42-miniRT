/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_vertex_cache.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:52:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 22:12:48 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"

#define HASH_MAP_SIZE 16384

static uint32_t	hash_triplet(t_face_idx_triplet t)
{
	uint32_t	hash;

	hash = 2166136261u;
	hash = (hash * 16777619) ^ t.v_idx;
	hash = (hash * 16777619) ^ t.vt_idx;
	hash = (hash * 16777619) ^ t.vn_idx;
	return (hash % HASH_MAP_SIZE);
}

static void	cache_add(t_cache_entry **map, t_face_idx_triplet key, uint32_t val)
{
	uint32_t		index;
	t_cache_entry	*new_entry;

	index = hash_triplet(key);
	new_entry = malloc(sizeof(t_cache_entry));
	if (!new_entry)
		return ;
	new_entry->key = key;
	new_entry->value = val;
	new_entry->next = map[index];
	map[index] = new_entry;
}

static int	cache_get(t_cache_entry **map, t_face_idx_triplet key)
{
	uint32_t		index;
	t_cache_entry	*current;

	index = hash_triplet(key);
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

static void	free_cache(t_cache_entry **map)
{
	int				i;
	t_cache_entry	*current;
	t_cache_entry	*to_free;

	i = 0;
	if (!map)
		return ;
	while (i < HASH_MAP_SIZE)
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

int	build_indexed_mesh(t_mesh *mesh, t_parser_temp_data *data)
{
	t_vector		v;
	t_vector		i;
	t_cache_entry	**cache;
	size_t			j;
	int				found_idx;
	uint32_t		new_idx;

	vector_init(&v, sizeof(t_vertex_indexed), data->temp_positions.num_elements);
	vector_init(&i, sizeof(uint32_t), data->temp_faces.num_elements);
	cache = ft_calloc(HASH_MAP_SIZE, sizeof(t_cache_entry *));
	if (!cache)
		return (-1);
	j = 0;
	while (j < data->temp_faces.num_elements)
	{
		t_face_idx_triplet *ptr = vector_get(&data->temp_faces, j);
		t_face_idx_triplet key = {ptr->v_idx > 0 ? ptr->v_idx - 1 : (int)data->temp_positions.num_elements + ptr->v_idx, ptr->vt_idx > 0 ? ptr->vt_idx - 1 : (int)data->temp_uvs.num_elements + ptr->vt_idx, ptr->vn_idx > 0 ? ptr->vn_idx - 1 : (int)data->temp_normals.num_elements + ptr->vn_idx};
		found_idx = cache_get(cache, key);
		if (found_idx != -1)
		{
			new_idx = found_idx;
			vector_add(&i, &new_idx);
		}
		else
		{
			t_vertex_indexed new_v;
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
			new_idx = v.num_elements;
			vector_add(&v, &new_v);
			vector_add(&i, &new_idx);
			cache_add(cache, key, new_idx);
		}
		j++;
	}
	return (free_cache(cache), mesh->vertices = v.data, mesh->vertex_count = v.num_elements, mesh->indices = i.data, mesh->index_count = i.num_elements, mesh->name = data->object_name, data->object_name = NULL, 0);
}

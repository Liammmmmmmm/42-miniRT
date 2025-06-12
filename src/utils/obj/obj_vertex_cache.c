/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_vertex_cache.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 20:52:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/12 15:57:17 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "minirt.h"

static uint32_t	hash_triplet(t_face_idx_triplet t)
{
	uint32_t	hash;

	hash = 2166136261u;
	hash = (hash * 16777619) ^ t.v_idx;
	hash = (hash * 16777619) ^ t.vt_idx;
	hash = (hash * 16777619) ^ t.vn_idx;
	return (hash);
}

static size_t	get_hash_map_size(size_t vertex_count)
{
	size_t	size;

	if (vertex_count == 0)
		return (1024);
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

static char	cache_add(t_cache_entry **map, t_face_idx_triplet key, uint32_t val,
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

static int	cache_get(t_cache_entry **map, t_face_idx_triplet key,
	size_t map_size)
{
	uint32_t		index;
	t_cache_entry	*current;

	index = hash_triplet(key) % map_size;;
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

static void	free_cache(t_cache_entry **map, size_t map_size)
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

int	build_indexed_mesh(t_mesh *mesh, t_parser_temp_data *data)
{
	t_vector			v;
	t_vector			i;
	t_cache_entry		**cache;
	size_t				j;
	int					found_idx;
	uint32_t			new_idx;
	size_t				map_size;
	t_vertex_indexed	new_v;
	t_face_idx_triplet	*ptr;
	t_face_idx_triplet	key;

	if (vector_init(&v, sizeof(t_vertex_indexed), data->temp_positions.num_elements) == -1
		|| vector_init(&i, sizeof(uint32_t), data->temp_faces.num_elements) == -1)
	{
		if (v.data)
			free(v.data);
		print_error("Vector init in build_indexed_mesh.");
		return (1);
	}
	map_size = get_hash_map_size(data->temp_positions.num_elements);
	cache = ft_calloc(map_size, sizeof(t_cache_entry *));
	if (!cache)
	{
		free(v.data);
		free(i.data);
		return (1);
	}
	j = 0;
	while (j < data->temp_faces.num_elements)
	{
		ptr = vector_get(&data->temp_faces, j);
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
		found_idx = cache_get(cache, key, map_size);
		if (found_idx != -1)
		{
			new_idx = found_idx;
			if (vector_add(&i, &new_idx) == -1)
			{
				free(v.data);
				free(i.data);
				free_cache(cache, map_size);
				print_error("Vector add in build_indexed_mesh.");
				return (1);
			}
		}
		else
		{
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
			if (vector_add(&v, &new_v) == -1 || vector_add(&i, &new_idx) == -1
				|| cache_add(cache, key, new_idx, map_size) == 1)
			{
				free(v.data);
				free(i.data);
				free_cache(cache, map_size);
				print_error("Vector add or cache add in build_indexed_mesh.");
				return (1);
			}
		}
		j++;
		print_progress_bar(j, data->temp_faces.num_elements);
	}
	free_cache(cache, map_size);
	mesh->vertices = v.data;
	mesh->vertex_count = v.num_elements;
	mesh->indices = i.data;
	mesh->index_count = i.num_elements;
	mesh->name = data->object_name;
	data->object_name = NULL;
	return (0);
}

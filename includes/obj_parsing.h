/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:42:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 17:51:47 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	OBJ_PARSING_H
# define OBJ_PARSING_H

# include "structs.h"

int					parse_obj(char *file, t_custom_object *obj);
t_vec3				rotate_around_axis(t_vec3 v, t_vec3 axis, float angle);
void				ft_update_obj_position(t_custom_object *obj);
int					build_indexed_mesh(t_mesh *mesh, t_parser_temp_data *data);
t_aabb				update_custom_obj_aabb(t_custom_object *obj);
t_aabb				compute_custom_obj(t_custom_object *obj, char init);
int					flatten_mesh_to_object(t_custom_object *obj, t_mesh *mesh);
int					read_file_to_temp_data(char *filepath,
	t_parser_temp_data *data);
char				add_single_triangle(long *e, t_vertex *vertices,
	t_vector *v, t_custom_object *obj);
long				ear_clipping(t_vertex *vertices, size_t vertex_count,
	t_vector *v, t_custom_object *obj);
int					parse_obj(char *filepath, t_custom_object *out_obj);
void				*vector_get(t_vector *vector, size_t index);
char				*skip_space(char *str);
char				parse_vec3(char *line, t_vector *vec_array);
char				parse_vec2(char *line, t_vector *vec_array);
int					parse_int_and_advance(char **line);
char				parse_face(char *line, t_vector *face_indices);
void				free_cache(t_cache_entry **map, size_t map_size);
char				cache_add(t_cache_entry **map, t_face_idx_triplet key,
	uint32_t val, size_t map_size);
int					cache_get(t_cache_entry **map, t_face_idx_triplet key,
	size_t map_size);
void				cleanup_mesh_data(t_mesh_build_data *build_data);
t_face_idx_triplet	normalize_indices(t_face_idx_triplet *ptr,
	t_parser_temp_data *data);
t_vertex_indexed	create_vertex(t_face_idx_triplet key,
	t_parser_temp_data *data);
int					process_face(t_face_idx_triplet key, t_mesh_build_data
	*build_data, t_parser_temp_data *data);
char				cache_add(t_cache_entry **map, t_face_idx_triplet key,
	uint32_t val, size_t map_size);
int					cache_get(t_cache_entry **map, t_face_idx_triplet key,
	size_t map_size);
int					init_mesh_vectors(t_mesh_build_data *build_data,
	t_parser_temp_data *data);
uint32_t			hash_triplet(t_face_idx_triplet t);
size_t				get_hash_map_size(size_t vertex_count);

#endif
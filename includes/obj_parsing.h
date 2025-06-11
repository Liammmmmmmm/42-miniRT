/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:42:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 21:44:59 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	OBJ_PARSING_H
# define OBJ_PARSING_H

# include "structs.h"

typedef struct s_face_idx_triplet
{
	int	v_idx;
	int	vt_idx;
	int	vn_idx;
}	t_face_idx_triplet;

typedef struct s_parser_temp_data
{
	t_vector	temp_positions;
	t_vector	temp_normals;
	t_vector	temp_uvs;
	t_vector	temp_faces;
	char		*object_name;
}	t_parser_temp_data;

typedef struct s_cache_entry
{
	t_face_idx_triplet		key;
	uint32_t				value;
	struct s_cache_entry	*next;
}	t_cache_entry;

t_vec3	rotate_around_axis(t_vec3 v, t_vec3 axis, float angle);
void	ft_update_obj_position(t_custom_object *obj);
int		build_indexed_mesh(t_mesh *mesh, t_parser_temp_data *data);
t_aabb	update_custom_obj_aabb(t_custom_object *obj);
t_aabb	compute_custom_obj(t_custom_object *obj, char init);
int		flatten_mesh_to_object(t_custom_object *obj, t_mesh *mesh);
int		read_file_to_temp_data(char *filepath, t_parser_temp_data *data);
char	add_single_triangle(long *e, t_vertex *vertices, t_vector *v, \
	t_custom_object *obj);
long	ear_clipping(t_vertex *vertices, size_t vertex_count, t_vector *v, \
	t_custom_object *obj);
int		parse_obj(char *filepath, t_custom_object *out_obj);
void	*vector_get(t_vector *vector, size_t index);

#endif
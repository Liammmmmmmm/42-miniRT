/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:42:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/22 14:53:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	OBJ_PARSING_H
# define OBJ_PARSING_H

#include "structs.h"

void	get_value_v(char *line, t_obj_temp *tmp, char *error);
void	get_value_vn(char *line, t_obj_temp *tmp, char *error);
void	get_value_vt(char *line, t_obj_temp *tmp, char *error);
void	get_value_face(char *line, t_obj_temp *tmp, char *error);
void	get_value_name(char *line, char **out, char *error);
void	get_value_face(char *line, t_obj_temp *tmp, char *error);
char	count_obj_data(const char *filename, t_obj_temp *tmp, size_t *size);
void	ft_free_tab_face(t_face *tab, size_t i);
void	ft_free_tab_triangle(t_triangle **tab, size_t i);
int		extract_all_triangle(t_obj_temp *tmp, t_custom_object *obj, const char *file);
long	ear_clipping(t_vertex *vertices, size_t vertex_count, t_vector *v, \
	t_custom_object *obj);
t_vec3	rotate_around_axis(t_vec3 v, t_vec3 axis, float angle);
void	ft_update_obj_position(t_custom_object *obj);

#endif
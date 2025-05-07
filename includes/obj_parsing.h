/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:42:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 18:28:18 by madelvin         ###   ########.fr       */
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
int		extract_all_triangle(t_obj_temp *obj, t_triangle **out, size_t *triangle_count, const char *file);
long	ear_clipping(t_vertex *vertices, size_t vertex_count, t_vector *v);

#endif
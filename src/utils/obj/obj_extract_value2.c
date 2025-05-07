/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_extract_value2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 13:58:05 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/07 18:02:43 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_face_malloc(int i, char *error, t_face *face)
{
	face->vertex_count = i;
	face->v_idx = malloc(sizeof(size_t) * i);
	face->vn_idx = malloc(sizeof(size_t) * i);
	face->vt_idx = malloc(sizeof(size_t) * i);
	if (!face->v_idx || !face->vn_idx || !face->vt_idx)
	{
		print_error("malloc error in obj parser");
		free(face->v_idx);
		free(face->vn_idx);
		free(face->vt_idx);
		*error = 1;
	}
	return ;
}

static void	extract_face_value(t_face *face, char **tokens)
{
	int		i;
	char	**vertex_data;

	i = 0;
	while (tokens[i])
	{
		vertex_data = ft_split_in_line(tokens[i], "/");
		if (vertex_data)
		{
			if (vertex_data[0])
				face->v_idx[i] = ft_atoi(vertex_data[0]);
			if (vertex_data[1])
				face->vt_idx[i] = ft_atoi(vertex_data[1]);
			if (vertex_data[2])
				face->vn_idx[i] = ft_atoi(vertex_data[2]);
			free(vertex_data);
		}
		i++;
	}
}

void	get_value_face(char *line, t_obj_temp *tmp, char *error)
{
	t_face	face;
	char	**tokens;
	int		i;

	i = 0;
	if (*error != 0 || ft_strncmp(line, "f ", 2) != 0)
		return ;
	tokens = ft_split_in_line(line + 2, " ");
	if (tokens == NULL)
	{
		print_error("malloc error in obj parser");
		*error = 1;
		return ;
	}
	while (tokens[i])
		i++;
	init_face_malloc(i, error, &face);
	if (*error == 1)
		return ;
	extract_face_value(&face, tokens);
	free(tokens);
	tmp->face[tmp->face_count++] = face;
	return ;
}

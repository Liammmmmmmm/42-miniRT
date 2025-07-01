/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parse_face.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 16:28:30 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 17:00:57 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parsing.h"
#include "minirt.h"

static char	parse_line(char *ptr, t_vector *corner_indices)
{
	t_face_idx_triplet	triplet;

	while (*ptr)
	{
		ptr = skip_space(ptr);
		if (!ft_isdigit(*ptr) && *ptr != '-' && *ptr != '+')
			break ;
		triplet = (t_face_idx_triplet){0, 0, 0};
		triplet.v_idx = parse_int_and_advance(&ptr);
		if (*ptr == '/')
		{
			ptr++;
			if (*ptr != '/')
				triplet.vt_idx = parse_int_and_advance(&ptr);
			if (*ptr == '/')
			{
				ptr++;
				triplet.vn_idx = parse_int_and_advance(&ptr);
			}
		}
		if (vector_add(corner_indices, &triplet) == -1)
			return (print_error1("Vector add in parse_face"));
	}
	return (0);
}

char	parse_triangle(t_vector *corner_indices, t_vector *face_indices)
{
	size_t	i;

	i = 0;
	while (++i < corner_indices->num_elements - 1)
	{
		if (vector_add(face_indices, vector_get(corner_indices, 0)) == -1 \
	|| vector_add(face_indices, vector_get(corner_indices, i)) == -1 \
	|| vector_add(face_indices, vector_get(corner_indices, i + 1)) == -1)
		{
			free(corner_indices->data);
			return (print_error1("Vector add in parse_face"));
		}
	}
	return (0);
}

char	parse_face(char *line, t_vector *face_indices)
{
	t_vector			corner_indices;

	if (vector_init(&corner_indices, sizeof(t_face_idx_triplet), 4) == -1)
		return (print_error1("Vector init in parse_face"));
	if (parse_line(line, &corner_indices))
	{
		free(corner_indices.data);
		return (1);
	}
	if (corner_indices.num_elements >= 3)
	{
		if (parse_triangle(&corner_indices, face_indices))
		{
			free(corner_indices.data);
			return (1);
		}
	}
	free(corner_indices.data);
	return (0);
}

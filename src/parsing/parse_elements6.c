/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 18:34:28 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 18:34:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

int	parse_plane(t_scene *scene, char *line)
{
	char	**parts;
	t_plane	*plane;

	plane = ft_calloc(sizeof(t_plane), 1);
	if (!plane)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = PLANE;
	scene->elements[scene->el_amount++].object = plane;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(PLANE, parts));
	if (!parse_vector(parts[1], &plane->position))
		return (invalid_struct_error(PLANE, parts));
	if (!parse_vector_normalized(parts[2], &plane->normal))
		return (invalid_struct_error(PLANE, parts));
	if (!parse_color_or_mat(parts[3], &plane->color, &plane->material, scene))
		return (invalid_struct_error(PLANE, parts));
	free(parts);
	return (1);
}

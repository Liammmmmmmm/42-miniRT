/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:31:25 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/25 17:18:41 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_cone(t_scene *scene, char *line)
{
	char		**parts;
	t_cone		*cone;

	cone = ft_calloc(sizeof(t_cone), 1);
	if (!cone)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = CONE;
	scene->elements[scene->el_amount++].object = cone;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 6)
		return (invalid_struct_error(CONE, parts));
	if (!parse_vector(parts[1], &cone->position))
		return (invalid_struct_error(CONE, parts));
	if (!parse_vector_normalized(parts[2], &cone->orientation))
		return (invalid_struct_error(CONE, parts));
	if (!is_valid_size(parts[3], &cone->diameter))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[4], &cone->height))
		return (invalid_size_error(parts));
	if (!parse_color_or_mat(parts[5], &cone->color, &cone->material, scene))
		return (invalid_struct_error(CONE, parts));
	free(parts);
	return (1);
}

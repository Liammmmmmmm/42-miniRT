/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:00:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 15:11:00 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_cylinder(t_scene *scene, char *line)
{
	char		**parts;
	t_cylinder	*cylinder;

	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = CYLINDER;
	scene->elements[scene->el_amount++].object = cylinder;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 6)
		return (invalid_struct_error(CYLINDER, parts));
	if (!parse_vector(parts[1], &cylinder->position))
		return (invalid_struct_error(CYLINDER, parts));
	if (!parse_vector_normalized(parts[2], &cylinder->orientation))
		return (invalid_struct_error(CYLINDER, parts));
	if (!is_valid_size(parts[3], &cylinder->diameter))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[4], &cylinder->height))
		return (invalid_size_error(parts));
	if (!parse_color(parts[5], &cylinder->color))
		return (invalid_struct_error(CYLINDER, parts));
	free(parts);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:31:25 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/28 18:16:04 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

int	parse_dlight(t_scene *scene, char *line)
{
	char	**parts;
	t_dlight	*light;

	light = ft_calloc(sizeof(t_dlight), 1);
	if (!light)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = DIRECTIONAL_LIGHT;
	scene->elements[scene->el_amount++].object = light;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 4)
		return (invalid_struct_error(DIRECTIONAL_LIGHT, parts));
	if (!parse_vector_normalized(parts[1], &light->orientation))
		return (invalid_struct_error(DIRECTIONAL_LIGHT, parts));
	if (!is_valid_double_el(parts[2], &light->brightness))
	{
		free(parts);
		return (print_error(ERR_INVALID_LIGHT_RATIO));
	}
	if (!parse_color(parts[3], &light->color))
		return (invalid_struct_error(DIRECTIONAL_LIGHT, parts));
	free(parts);
	return (1);
}

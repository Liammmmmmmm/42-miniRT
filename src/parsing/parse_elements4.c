/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:31:25 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/05 20:00:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_cone(t_scene *scene, char *line)
{
	char		**parts;
	t_cone		*cone;
	int			nb_parts;

	cone = ft_calloc(sizeof(t_cone), 1);
	if (!cone)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = CONE;
	scene->elements[scene->el_amount++].object = cone;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	nb_parts = char_tab_len(parts);
	if (nb_parts < 6 || nb_parts > 7)
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
	cone->material_top = NULL;
	if (nb_parts > 6 && !parse_only_mat(parts[6], &cone->material_top, scene))
		return (invalid_struct_error(CONE, parts));
	cone->orientation = vec3_unit(cone->orientation);
	free(parts);
	return (1);
}

int	parse_hyperboloid(t_scene *scene, char *line)
{
	char			**parts;
	t_hyperboloid	*hyp;

	hyp = ft_calloc(sizeof(t_hyperboloid), 1);
	if (!hyp)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = HYPERBOLOID;
	scene->elements[scene->el_amount++].object = hyp;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 9)
		return (invalid_struct_error(HYPERBOLOID, parts));
	if (!parse_vector(parts[1], &hyp->position))
		return (invalid_struct_error(HYPERBOLOID, parts));
	if (!parse_vector_normalized(parts[2], &hyp->orientation))
		return (invalid_struct_error(HYPERBOLOID, parts));
	if (!is_valid_size(parts[3], &hyp->height))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[4], &hyp->a))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[5], &hyp->b))
		return (invalid_size_error(parts));
	if (!is_valid_size(parts[6], &hyp->c))
		return (invalid_size_error(parts));
	if (!is_valid_double_el_no_bordered(parts[7], &hyp->shape))
		return (invalid_size_error(parts));
	if (!parse_color_or_mat(parts[8], &hyp->color, &hyp->material, scene))
		return (invalid_struct_error(HYPERBOLOID, parts));
	hyp->orientation = vec3_unit(hyp->orientation);
	free(parts);
	return (1);
}

int	parse_obj_custom(t_scene *scene, char *line)
{
	char			**parts;
	t_custom_object	*obj;

	obj = ft_calloc(sizeof(t_custom_object), 1);
	if (!obj)
		return (print_error(strerror(errno)));
	scene->elements[scene->el_amount].type = CUSTOM;
	scene->elements[scene->el_amount++].object = obj;
	parts = ft_split_in_line(line, " ");
	if (!parts)
		return (print_error(strerror(errno)));
	if (char_tab_len(parts) != 6)
		return (invalid_struct_error(CUSTOM, parts));
	if (!parse_vector(parts[1], &obj->position))
		return (invalid_struct_error(CUSTOM, parts));
	if (!parse_vector_normalized(parts[2], &obj->orientation))
		return (invalid_struct_error(CUSTOM, parts));
	if (!parse_vector(parts[3], &obj->scale))
		return (invalid_size_error(parts));
	if (!parse_obj(parts[4], obj))
		return (invalid_size_error(parts)); // a changer
	if (!parse_color_or_mat(parts[5], &obj->color, &obj->material, scene))
		return (invalid_struct_error(CUSTOM, parts));
	free(parts);
	return (1);
}

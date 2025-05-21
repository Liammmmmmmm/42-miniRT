/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:38:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 10:37:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	parse_object_type_and_num(char *str, t_obj_anim *obj)
{
	char	**parts;
	int		tab_size;

	parts = ft_split_in_line(str, ".");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	if (tab_size != 2 && tab_size != 1)
		return (anim_print_error_f(parts, "Invalid object name. Usage: OBJ[.num]"));
	obj->obj = get_object_type(parts[0]);
	if (obj->obj == NULL_OBJ)
		return (anim_print_error_f(parts, "Invalid object type. Usage: OBJ[.num]"));
	if (tab_size == 1)
		obj->obj_num = 0;
	else
		obj->obj_num = ft_atoi(parts[1]);
	if (obj->obj_num < 0)
		obj->obj_num = 0;
	free(parts);
	return (1);
}


int	parse_object_points(char *str, t_obj_anim *obj)
{
	char	**parts;
	int		tab_size;

	parts = ft_split_in_line(str, "|");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	if (tab_size != 2 && tab_size != 1)
		return (anim_print_error_f(parts, "Invalid animation parameter. Usage: Movement points [| orientation vectors]"));
	if (!parse_movement_points(parts[0], obj))
	{
		free(parts);
		return (0);
	}
	if (tab_size == 2 && !parse_orientation_points(parts[1], obj))
	{
		free(obj->orientations);
		free(obj->points);
		obj->orientations = NULL;
		obj->points = NULL;
		free(parts);
		return (0);
	}
	else if (tab_size == 1)
		set_auto_orientation_points(obj);
	free(parts);
	return (1);
}

int	parse_one_anim_obj(char *str, t_obj_anim *obj)
{
	char	**parts;

	parts = ft_split_in_line(str, ":");
	if (!parts)
		return (print_error("Malloc error"));
	if (char_tab_len(parts) != 2)
		return (anim_print_error_f(parts, "Invalid animation parameter. Usage: OBJ[.num]:POINTS"));
	if (!parse_object_type_and_num(parts[0], obj))
	{
		free(parts);
		return (0);
	}
	if (!parse_object_points(parts[1], obj))
	{
		free(parts);
		return (0);
	}
	free(parts);
	return (1);
}

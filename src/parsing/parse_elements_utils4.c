/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils4.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:25:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/25 11:40:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_tex	*get_texture(char *str, t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < scene->tex_amount)
	{
		if (ft_strcmp(str, scene->textures[i].name) == 0)
			return (&scene->textures[i]);
	}
	return (NULL);
}

int	parse_color_or_tex(char *str, t_color *store, t_tex **tex, t_scene *scene)
{
	int	res;

	if (contains_var_chars(str))
	{
		res = is_valid_variable_name_tex(str, scene);
		if (res == 0)
			*tex = get_texture(str, scene);
		else if (res == 1)
			return (texture_item_error(0, str));
		else
			return (texture_item_error(1, str));
		return (1);
	}
	return (parse_color(str, store));
}

int	is_valid_double_el_no_bordered(char *str, double *co)
{
	if (is_valid_float(str))
	{
		*co = ft_atod(str);
		return (1);
	}
	return (0);
}

int	parse_double_or_tex(char *str, double *d, t_tex **tex, t_scene *scene)
{
	int	res;

	if (contains_var_chars(str))
	{
		res = is_valid_variable_name_tex(str, scene);
		if (res == 0)
			*tex = get_texture(str, scene);
		else if (res == 1)
			return (texture_item_error(0, str));
		else
			return (texture_item_error(1, str));
		return (1);
	}
	return (is_valid_double_el_no_bordered(str, d));
}

int	parse_double_b_or_tex(char *str, double *d, t_tex **tex, t_scene *scene)
{
	int	res;

	if (contains_var_chars(str))
	{
		res = is_valid_variable_name_tex(str, scene);
		if (res == 0)
			*tex = get_texture(str, scene);
		else if (res == 1)
			return (texture_item_error(0, str));
		else
			return (texture_item_error(1, str));
		return (1);
	}
	return (is_valid_double_el(str, d));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:30:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/25 10:30:43 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_valid_variable_name_tex(char *str, t_scene *scene)
{
	int	i;

	i = is_valid_variable_name(str);
	if (i != 1)
		return (i);
	i = -1;
	while (++i < scene->tex_amount)
	{
		if (ft_strcmp(str, scene->textures[i].name) == 0)
			return (0);
	}
	return (1);
}

int	is_valid_variable_name_mat(char *str, t_scene *scene)
{
	int	i;

	i = is_valid_variable_name(str);
	if (i != 1)
		return (i);
	i = -1;
	while (++i < scene->mat_amount)
	{
		if (ft_strcmp(str, scene->materials[i].name) == 0)
			return (0);
	}
	return (1);
}

int	contains_var_chars(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalpha(str[i]) || str[i] == '_')
			return (1);
		i++;
	}
	return (0);
}

t_mat	*get_material(char *str, t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < scene->mat_amount)
	{
		if (ft_strcmp(str, scene->materials[i].name) == 0)
			return (&scene->materials[i]);
	}
	return (NULL);
}

int	parse_color_or_mat(char *str, t_color *store, t_mat **mat, t_scene *scene)
{
	int	res;

	if (contains_var_chars(str))
	{
		res = is_valid_variable_name_mat(str, scene);
		if (res == 0)
			*mat = get_material(str, scene);
		else if (res == 1)
			return (material_item_error(0, str));
		else
			return (material_item_error(1, str));
		return (1);
	}
	return (parse_color(str, store));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils3.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:30:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 16:33:47 by lilefebv         ###   ########lyon.fr   */
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

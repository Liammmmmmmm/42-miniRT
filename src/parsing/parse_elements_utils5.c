/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements_utils5.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:41:31 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 14:22:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "error_message.h"

int	is_extension(char *filename, char *extension)
{
	int	fi;
	int	ei;

	fi = ft_sstrlen(filename) - 1;
	ei = ft_sstrlen(extension) - 1;
	while (ei >= -1 && fi >= 0)
	{
		if (!((ei == -1 && filename[fi] == '.')
				|| (filename[fi] == extension[ei])))
			break ;
		ei--;
		fi--;
	}
	if (ei == -2)
		return (1);
	return (0);
}

inline int	parse_ambient_light_ratio_and_color(t_scene *scene, char **parts)
{
	if (char_tab_len(parts) != 3)
		return (invalid_struct_error(AMBIANT_LIGHT, parts));
	if (!is_valid_float(parts[1]))
		return (invalid_float_error(parts, 1));
	scene->amb_light.ratio = ft_atod(parts[1]);
	if (!parse_color_or_tex(parts[2], &scene->amb_light.skybox_c,
			&scene->amb_light.skybox_t, scene))
		return (invalid_struct_error(AMBIANT_LIGHT, parts));
	if (scene->amb_light.skybox_t && scene->amb_light.skybox_t->type == IMAGE
		&& !scene->amb_light.skybox_t->img.rgba)
		scene->amb_light.skybox_t = NULL;
	if (scene->amb_light.skybox_t && scene->amb_light.skybox_t->type == HDR
		&& !scene->amb_light.skybox_t->hdr.pixels)
		scene->amb_light.skybox_t = NULL;
	return (1);
}

int	parse_only_mat(char *str, t_mat **mat, t_scene *scene)
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
	return (0);
}

/**
 * This function isn't protected against every overflows
 */
int	is_valid_positive_int(char *str, int *co)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	if (i > 10)
		return (0);
	*co = ft_atoi(str);
	return (1);
}

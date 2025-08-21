/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:00:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 21:56:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "utils.h"

int	check_every_elements_same_frames_am(int tab_size, t_animation *anim)
{
	int	i;

	i = -1;
	while (++i < tab_size)
	{
		if (i != 0 && anim->frames != anim->objects[i].frames)
		{
			free_anim(anim);
			return (print_error("Every object should have the same amount of "
					"frame"));
		}
		anim->frames = anim->objects[i].frames;
	}
	return (1);
}

int	animate_option_parsing(char *str, t_animation *anim)
{
	char	**parts;
	int		tab_size;
	int		i;
	int		res;

	parts = ft_split_in_line(str, ";");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	anim->nb_objects = tab_size;
	anim->objects = ft_calloc(tab_size, sizeof(t_obj_anim));
	i = -1;
	while (++i < tab_size)
	{
		if (!parse_one_anim_obj(parts[i], &anim->objects[i]))
		{
			free_anim(anim);
			free(parts);
			return (0);
		}
	}
	res = check_every_elements_same_frames_am(tab_size, anim);
	free(parts);
	return (res);
}

int	is_animate_option(t_minirt *minirt, char *argvi, int *y)
{
	if (ft_strcmp(argvi, "--animate") == 0)
		return (print_error_sy("Animate option need a value", y));
	else if (ft_strncmp(argvi, "--animate=", 10) == 0)
	{
		if (ft_strlen(argvi + 10) > 16)
		{
			minirt->options.anim.enabled = 1;
			if (!animate_option_parsing(argvi + 10, &minirt->options.anim))
			{
				minirt->options.anim.enabled = 0;
				*y = 0;
			}
			return (1);
		}
		return (print_error_sy("Invalid animation value", y));
	}
	return (0);
}

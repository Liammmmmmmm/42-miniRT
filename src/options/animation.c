/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:00:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/20 13:39:07 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	animate_option_parsing(char *str, t_animation *anim)
{
	char	**parts;
	int		tab_size;
	int		i;

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
	debug_print_animation(anim);
	i = -1;
	while (++i < tab_size)
	{
		if (i != 0 && anim->frames != anim->objects[i].frames)
		{
			free_anim(anim);
			return (anim_print_error_f(parts, "Every object should have the same amount of frame"));
		}
		anim->frames = anim->objects[i].frames;
	}
	free(parts);
	return (1);
}

int	is_animate_option(t_minirt *minirt, char *argvi, int *y)
{
	if (ft_strcmp(argvi, "--animate") == 0)
	{
		*y = 0;		
		return (print_error1("Animate option need a value"));
	}
	else if (ft_strncmp(argvi, "--animate=", 10) == 0)
	{
		if (ft_strlen(argvi + 10) > 16)
		{
			minirt->options.anim.enabled = 1;
			if (!animate_option_parsing(argvi + 10, &minirt->options.anim))
				*y = 0;
			return (1);
		}
		*y = 0;
		return (print_error1("Invalid animation value"));
	}
	return (0);
}

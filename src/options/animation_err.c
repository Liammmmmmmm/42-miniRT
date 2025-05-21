/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_err.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:25:52 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 10:45:06 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_anim(t_animation *anim)
{
	t_uint	i;

	i = (t_uint)-1;
	if (!anim->objects)
		return ;
	while (++i < anim->nb_objects)
	{
		free(anim->objects[i].points);
		free(anim->objects[i].orientations);
	}
	free(anim->objects);
}

int	anim_print_error_f(char **parts, char *err)
{
	free(parts);
	return (print_error(err));
}

void	debug_print_animation(t_animation *anim)
{
	int			i;
	int			y;
	t_obj_anim	obj;
	
	i = -1;
	while (++i < (int)anim->nb_objects)
	{
		obj = anim->objects[i];
		y = -1;
		printf("Object : %s.%u\n", get_object_name(obj.obj), obj.obj_num);
		while (++y < (int)obj.frames)
		{
			printf("  Frame %d : Pos -> %f,%f,%f  Orientation -> %f,%f,%f\n", y, obj.points[y].x, obj.points[y].y, obj.points[y].z, obj.orientations[y].x, obj.orientations[y].y, obj.orientations[y].z);
			
		}
	}
}

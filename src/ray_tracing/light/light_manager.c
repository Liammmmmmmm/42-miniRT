/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:27:09 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 19:06:26 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"
#include "material.h"
#include "bvh.h"
#include "minirt.h"
#include <math.h>

static void	add_all_light_contribution(t_hit_record *hit_record,
	t_minirt *minirt, t_vec3 view_dir, t_lcolor *light_color)
{
	t_light		*light;
	t_dlight	*dlight;
	int			i;

	i = 0;
	while (i < minirt->scene.obj_lst.light_nb)
	{
		if (minirt->scene.obj_lst.light_lst[i]->type == LIGHT)
		{
			light = (t_light *)minirt->scene.obj_lst.light_lst[i]->object;
			if (check_plight_hit(minirt, hit_record->point, light->position)
				== 0)
				add_plight(light_color, hit_record, light, view_dir);
		}
		if (minirt->scene.obj_lst.light_lst[i]->type == DIRECTIONAL_LIGHT)
		{
			dlight = (t_dlight *)minirt->scene.obj_lst.light_lst[i]->object;
			if (check_dlight_hit_dir(minirt, hit_record->point,
					vec3_negate(dlight->orientation)) == 0)
				add_dlight(light_color, hit_record, dlight, view_dir);
		}
		i++;
	}
}

t_fcolor	compute_light_v2(t_hit_record *hit_record, t_minirt *minirt)
{
	t_lcolor	light_color;
	t_vec3		view_dir;

	light_color = (t_lcolor){0, 0, 0};
	view_dir = vec3_unit(vec3_subtract(minirt->scene.camera.position, \
		hit_record->point));
	add_all_light_contribution(hit_record, minirt, view_dir, &light_color);
	return ((t_fcolor){light_color.r / 255.0, light_color.g / 255.0,
		light_color.b / 255.0});
}

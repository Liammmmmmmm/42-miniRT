/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dependant_values.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 09:36:26 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 19:19:20 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	general_scene_info(t_minirt *minirt)
{
	t_fcolor	*new_render;

	minirt->scene.camera.orientation
		= vec3_unit(minirt->scene.camera.orientation);
	minirt->controls.max_bounces = round(minirt->controls.max_bounces);
	minirt->controls.res_render_x = round(minirt->controls.res_render_x);
	minirt->controls.res_render_y = round(minirt->controls.res_render_y);
	if ((int)minirt->controls.res_render_x != minirt->scene.render_width
		|| (int)minirt->controls.res_render_y != minirt->scene.render_height)
	{
		if (!minirt->controls.movements.last_frame_is_moving)
		{
			minirt->scene.render_width = (int)minirt->controls.res_render_x;
			minirt->scene.render_height = (int)minirt->controls.res_render_y;
		}
		new_render = malloc(sizeof(t_fcolor) * minirt->scene.render_width
				* minirt->scene.render_height);
		if (new_render)
		{
			free(minirt->screen.float_render);
			minirt->screen.float_render = new_render;
		}
	}
	if ((!minirt->scene.amb_light.skybox_t
			|| minirt->scene.amb_light.skybox_t->type != HDR)
		&& minirt->scene.amb_light.ratio < 0)
		minirt->scene.amb_light.ratio = 0;
}

static void	check_mats(t_minirt *minirt)
{
	int	i;

	i = -1;
	while (++i < minirt->scene.mat_amount)
		set_values_mat(&minirt->scene.materials[i]);
}

void	set_dependant_values(t_minirt *minirt)
{
	int	i;

	general_scene_info(minirt);
	i = -1;
	while (++i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
			set_values_sphere(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == CYLINDER)
			set_values_cylinder(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == PLANE)
			set_values_plane(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == LIGHT)
			set_values_light(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == CONE)
			set_values_cone(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == HYPERBOLOID)
			set_values_hyperboloid(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == CUSTOM)
			set_values_custom(minirt->scene.elements[i].object);
		else if (minirt->scene.elements[i].type == DIRECTIONAL_LIGHT)
			set_values_directional_light(minirt->scene.elements[i].object);
	}
	check_mats(minirt);
}

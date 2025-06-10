/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_object2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:45:58 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:18:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_selected_sphere(t_minirt *minirt, t_sphere *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[6], &obj->diameter);
	minirt->controls.float_input[6].y = 905;
}

void	set_selected_plane(t_minirt *minirt, t_plane *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->normal.x);
	link_float_input(&minirt->controls.float_input[4], &obj->normal.y);
	link_float_input(&minirt->controls.float_input[5], &obj->normal.z);
}

void	set_selected_cylinder(t_minirt *minirt, t_cylinder *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	minirt->controls.dropdown[1].selected = (void **)&obj->material_top;
	minirt->controls.dropdown[2].selected = (void **)&obj->material_bot;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->orientation.x);
	link_float_input(&minirt->controls.float_input[4], &obj->orientation.y);
	link_float_input(&minirt->controls.float_input[5], &obj->orientation.z);
	link_float_input(&minirt->controls.float_input[6], &obj->diameter);
	link_float_input(&minirt->controls.float_input[7], &obj->height);
	minirt->controls.float_input[6].y = 965;
	minirt->controls.float_input[7].y = 995;
}

void	set_selected_cone(t_minirt *minirt, t_cone *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	minirt->controls.dropdown[1].selected = (void **)&obj->material_top;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->orientation.x);
	link_float_input(&minirt->controls.float_input[4], &obj->orientation.y);
	link_float_input(&minirt->controls.float_input[5], &obj->orientation.z);
	link_float_input(&minirt->controls.float_input[6], &obj->diameter);
	link_float_input(&minirt->controls.float_input[7], &obj->height);
	minirt->controls.float_input[6].y = 935;
	minirt->controls.float_input[7].y = 965;
}

void	set_selected_hyperboloid(t_minirt *minirt, t_hyperboloid *obj)
{
	link_color_picker(&minirt->controls.color_picker[0], &obj->color);
	minirt->controls.dropdown[0].selected = (void **)&obj->material;
	link_float_input(&minirt->controls.float_input[0], &obj->position.x);
	link_float_input(&minirt->controls.float_input[1], &obj->position.y);
	link_float_input(&minirt->controls.float_input[2], &obj->position.z);
	link_float_input(&minirt->controls.float_input[3], &obj->orientation.x);
	link_float_input(&minirt->controls.float_input[4], &obj->orientation.y);
	link_float_input(&minirt->controls.float_input[5], &obj->orientation.z);
	link_float_input(&minirt->controls.float_input[6], &obj->shape);
	link_float_input(&minirt->controls.float_input[7], &obj->height);
	link_float_input(&minirt->controls.float_input[8], &obj->a);
	link_float_input(&minirt->controls.float_input[9], &obj->b);
	link_float_input(&minirt->controls.float_input[10], &obj->c);
	minirt->controls.float_input[6].y = 905;
	minirt->controls.float_input[7].y = 935;
	minirt->controls.float_input[8].y = 965;
	minirt->controls.float_input[9].y = 995;
	minirt->controls.float_input[10].y = 1025;
}

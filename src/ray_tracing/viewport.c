/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:36:33 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/15 11:09:03 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"

static void	init_camera_values(t_minirt *minirt)
{
	minirt->scene.camera.fov = minirt->controls.values.fov;
	minirt->scene.camera.focus_dist
		= minirt->controls.values.focus_dist / 10.0;
	minirt->scene.camera.defocus_angle
		= minirt->controls.values.defocus_angle / 30.0;
	minirt->scene.camera.orientation = vec3_unit(
			minirt->scene.camera.orientation);
}

static void	init_viewport_values(t_minirt *minirt, t_viewport *vp, t_vec3 *u)
{
	ft_bzero(vp, sizeof(t_viewport));
	init_camera_values(minirt);
	init_bvh(&minirt->scene.bvh, minirt->scene.elements,
		minirt->scene.el_amount);
	init_plane_light_lst(minirt);
	vp->gamma = sqrt(minirt->controls.values.gamma / 1000.0);
	vp->render_w = minirt->scene.render_width;
	vp->render_h = minirt->scene.render_height;
	vp->height = 2 * tan((minirt->controls.values.fov * PI_D / 180) / 2)
		* minirt->scene.camera.focus_dist;
	vp->width = vp->height * ((float)vp->render_w / vp->render_h);
	*u = vec3_unit(vec3_cross((t_vec3){0, 1, 0},
				vec3_negate(minirt->scene.camera.orientation)));
}

static void	init_viewport_vectors(t_minirt *minirt, t_viewport *vp, t_vec3 u)
{
	vp->u = vec3_multiply_scalar(u, vp->width);
	vp->v = vec3_multiply_scalar(
			vec3_cross(minirt->scene.camera.orientation, u), vp->height);
	vp->pixel_delta_u = vec3_divide_scalar(vp->u, vp->render_w);
	vp->pixel_delta_v = vec3_divide_scalar(vp->v, vp->render_h);
	vp->upper_left = vec3_subtract(vec3_subtract(
				vec3_add(minirt->scene.camera.position,
					vec3_multiply_scalar(minirt->scene.camera.orientation,
						minirt->scene.camera.focus_dist)),
				vec3_divide_scalar(vp->u, 2)),
			vec3_divide_scalar(vp->v, 2));
	vp->pixel00_loc = vec3_add(vp->upper_left, vec3_multiply_scalar(
				vec3_add(vp->pixel_delta_u, vp->pixel_delta_v), 0.5));
	vp->defocus_radius = minirt->scene.camera.focus_dist
		* tan(minirt->scene.camera.defocus_angle * PI_D / 360);
	vp->defocus_disk_u = vec3_multiply_scalar(u, vp->defocus_radius);
	vp->defocus_disk_v = vec3_multiply_scalar(
			vec3_cross(minirt->scene.camera.orientation, u),
			vp->defocus_radius);
}

t_viewport	init_viewport(t_minirt *minirt)
{
	t_viewport	vp;
	t_vec3		u;

	init_viewport_values(minirt, &vp, &u);
	init_viewport_vectors(minirt, &vp, u);
	return (vp);
}

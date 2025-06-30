/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:36:33 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/30 16:25:23 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "caustic.h"
#include "bvh.h"

static void	init_camera_values(t_minirt *minirt)
{
	t_float_input	*fl_in;

	minirt->scene.camera.fov = minirt->controls.values.fov;
	minirt->scene.camera.orientation = vec3_unit(
			minirt->scene.camera.orientation);
	fl_in = minirt->controls.float_input;
	link_float_input(&fl_in[19], &minirt->scene.camera.position.x);
	link_float_input(&fl_in[20], &minirt->scene.camera.position.y);
	link_float_input(&fl_in[21], &minirt->scene.camera.position.z);
	link_float_input(&fl_in[22], &minirt->scene.camera.orientation.x);
	link_float_input(&fl_in[23], &minirt->scene.camera.orientation.y);
	link_float_input(&fl_in[24], &minirt->scene.camera.orientation.z);
	link_float_input(&fl_in[25], &minirt->scene.camera.defocus_angle);
	link_float_input(&fl_in[26], &minirt->scene.camera.focus_dist);
	link_float_input(&fl_in[27], &minirt->scene.amb_light.ratio);
}

static void	init_viewport_values(t_minirt *minirt, t_viewport *vp, t_vec3 *u)
{
	t_vec3	up;

	ft_bzero(vp, sizeof(t_viewport));
	init_camera_values(minirt);
	if (minirt->scene.build_bvh)
	{
		init_bvh(&minirt->scene.bvh, minirt->scene.elements,
			minirt->scene.el_amount);
		minirt->scene.build_bvh = 0;
	}
	init_plane_light_lst(minirt);
	caustic_manager(minirt, 100000);
	vp->gamma = minirt->viewport.gamma;
	vp->render_w = minirt->scene.render_width;
	vp->render_h = minirt->scene.render_height;
	vp->width = 2.0 * tan(minirt->controls.values.fov * (PI_D / 180.0) / 2.0)
		* minirt->scene.camera.focus_dist;
	vp->height = vp->width / ((double)vp->render_w / (double)vp->render_h);
	up = (t_vec3){0, 1, 0};
	if (fabs(minirt->scene.camera.orientation.y) > 0.999)
		up = (t_vec3){1, 0, 0};
	*u \
	= vec3_unit(vec3_cross(up, vec3_negate(minirt->scene.camera.orientation)));
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

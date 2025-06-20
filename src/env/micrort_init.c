/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micrort_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:31:51 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 15:10:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	init_camera_values(t_minirt *minirt)
{
	minirt->micrort.camera.fov = minirt->controls.values.fov;
	minirt->micrort.camera.focus_dist = 2;
	minirt->micrort.camera.defocus_angle = 0;
	minirt->micrort.camera.orientation = (t_vec3){-1, 0, 0};
	minirt->micrort.camera.position = (t_vec3){2.5, 0, 0};
}

static void	init_viewport_values(t_minirt *minirt, t_viewport *vp, t_vec3 *u)
{
	t_vec3	up;

	ft_bzero(vp, sizeof(t_viewport));
	vp->gamma = 1.0;
	vp->render_w = 299;
	vp->render_h = 219;
	vp->width = 2.0 * tan(70 * (PI_D / 180.0) / 2.0)
		* minirt->micrort.camera.focus_dist;
	vp->height = vp->width / ((double)vp->render_w / (double)vp->render_h);
	up = (t_vec3){0, 1, 0};
	if (fabs(minirt->micrort.camera.orientation.y) > 0.999)
		up = (t_vec3){1, 0, 0};
	*u = vec3_unit(vec3_cross(up,
				vec3_negate(minirt->micrort.camera.orientation)));
}

static void	init_viewport_vectors(t_minirt *minirt, t_viewport *vp, t_vec3 u)
{
	vp->u = vec3_multiply_scalar(u, vp->width);
	vp->v = vec3_multiply_scalar(
			vec3_cross(minirt->micrort.camera.orientation, u), vp->height);
	vp->pixel_delta_u = vec3_divide_scalar(vp->u, vp->render_w);
	vp->pixel_delta_v = vec3_divide_scalar(vp->v, vp->render_h);
	vp->upper_left = vec3_subtract(vec3_subtract(
				vec3_add(minirt->micrort.camera.position,
					vec3_multiply_scalar(minirt->micrort.camera.orientation,
						minirt->micrort.camera.focus_dist)),
				vec3_divide_scalar(vp->u, 2)),
			vec3_divide_scalar(vp->v, 2));
	vp->pixel00_loc = vec3_add(vp->upper_left, vec3_multiply_scalar(
				vec3_add(vp->pixel_delta_u, vp->pixel_delta_v), 0.5));
	vp->defocus_radius = minirt->micrort.camera.focus_dist
		* tan(minirt->micrort.camera.defocus_angle * PI_D / 360);
	vp->defocus_disk_u = vec3_multiply_scalar(u, vp->defocus_radius);
	vp->defocus_disk_v = vec3_multiply_scalar(
			vec3_cross(minirt->micrort.camera.orientation, u),
			vp->defocus_radius);
}

static t_viewport	init_micrort_viewport(t_minirt *minirt)
{
	t_viewport	vp;
	t_vec3		u;

	init_camera_values(minirt);
	init_viewport_values(minirt, &vp, &u);
	init_viewport_vectors(minirt, &vp, u);
	return (vp);
}

int	init_micrort(t_minirt *minirt)
{
	minirt->micrort.render = ft_calloc(299 * 219, sizeof(t_fcolor));
	if (!minirt->micrort.render)
		return (0);
	minirt->micrort.viewport = init_micrort_viewport(minirt);
	minirt->micrort.sphere.diameter = 2;
	minirt->micrort.sphere.radius = 1;
	minirt->micrort.sphere.sqrt_radius = 1 * 1;
	minirt->micrort.sphere.position = (t_vec3){0, 0, 0};
	minirt->micrort.max_sample = 16;
	minirt->micrort.sample = 0;
	return (1);
}

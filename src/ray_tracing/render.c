/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/18 13:14:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "material.h"
#include <math.h>

t_color ray_color(t_minirt *minirt, t_ray ray, int depth, char *hit)
{
	t_color			color;
	t_color			background;
	t_hit_record	hit_record;
	double			a;

	if (depth <= 0)
		return (t_color){0, 0, 0};
	a = 0.5 * (ray.dir.y + 1);
	if (a > 1)
		a = 1;
	if (a < 0)
		a = 0;
	background.r = ((1 - a) * 255 + a * 128);
	background.g = ((1 - a) * 255 + a * 178);
	background.b = ((1 - a) * 255 + a * 255);
	if (hit_register_all(minirt, &ray, &hit_record) == 1)
	{
		if (hit_record.mat)
			color = hit_record.mat->color_value;
		else
			color = hit_record.color;
		color = color_multiply(color, compute_light(&hit_record, minirt));
		color = material_manager((t_mat_manager){hit_record, ray, minirt, color, depth});
		if (hit)
			*hit = 1;
		return (color);
	}
	if (hit)
		*hit = 0;
	return (color_scale(background, minirt->scene.amb_light.ratio));
}

void	calc_one_sample(t_minirt *minirt, t_vec3 offset)
{
	t_color	color;
	t_uint	tpix;
	t_uint	i;
	t_ray	ray;
	char	bounce_hit;

	bounce_hit = 0;
	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	while (i < tpix)
	{
		if (minirt->scene.camera.defocus_angle <= 0)
			ray.orig = minirt->scene.camera.position;
		else
			ray.orig = defocus_disk_sample(minirt);
		ray.dir = vec3_subtract(
			vec3_add(
				vec3_add(
					minirt->viewport.pixel00_loc, 
					vec3_multiply_scalar(minirt->viewport.pixel_delta_u, (i % minirt->mlx.img.width) + offset.x)
				),
				vec3_multiply_scalar(minirt->viewport.pixel_delta_v, (i / minirt->mlx.img.width) + offset.y)
			), 
			ray.orig
		);
		color = ray_color(minirt, ray, 10, &bounce_hit);
		minirt->screen.render[i].color.r += color.r * minirt->viewport.gamma;
		minirt->screen.render[i].color.g += color.g * minirt->viewport.gamma;
		minirt->screen.render[i].color.b += color.b * minirt->viewport.gamma;
		i++;
	}
}

void	draw_pixels(t_minirt *minirt)
{
	t_vec3	offset;

	offset = vec3_random();
	calc_one_sample(minirt, offset);
	minirt->screen.sample++;
	put_render_to_frame(minirt);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
	printf("Sample %d\n", minirt->screen.sample);
}

t_viewport	init_viewport(t_minirt *minirt)
{
	t_viewport	viewport;
	t_vec3		u;


	/* a securiser */
	minirt->scene.camera.fov = minirt->controls.values.fov;
	init_bvh(&minirt->scene.bvh, minirt->scene.elements, minirt->scene.el_amount);
	// print_bvh(bvh, 0, 0);
	/* a securiser */

	minirt->scene.camera.focus_dist = minirt->controls.values.focus_dist / 10.0;
	minirt->scene.camera.defocus_angle = minirt->controls.values.defocus_angle / 30.0;
	printf("Focus dist: %f\nDefocus angle: %f\n", minirt->scene.camera.focus_dist, minirt->scene.camera.defocus_angle);
	minirt->scene.camera.orientation = vec3_unit(minirt->scene.camera.orientation);
	viewport.gamma = sqrt(minirt->controls.values.gamma / 1000.0);
	viewport.height = 2 * tan((minirt->controls.values.fov * PI_10D/180)/2) * minirt->scene.camera.focus_dist;
	// viewport.focal_length = viewport.height / (2.0 * tan(minirt->controls.values.fov * 0.5 * PI_10D / 180));
	viewport.width = viewport.height * ((float)minirt->mlx.img.width / minirt->mlx.img.height);
	u = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, vec3_negate(minirt->scene.camera.orientation)));
	viewport.u = vec3_multiply_scalar(u, viewport.width);
	viewport.v = vec3_multiply_scalar(vec3_cross(minirt->scene.camera.orientation, u), viewport.height);
	viewport.pixel_delta_u = vec3_divide_scalar(viewport.u, minirt->mlx.img.width);
	viewport.pixel_delta_v = vec3_divide_scalar(viewport.v, minirt->mlx.img.height);
	viewport.upper_left = vec3_subtract(
		vec3_subtract(
			vec3_add(minirt->scene.camera.position, vec3_multiply_scalar(minirt->scene.camera.orientation, minirt->scene.camera.focus_dist)),
			vec3_divide_scalar(viewport.u, 2)
		),
		vec3_divide_scalar(viewport.v, 2)
	);

	viewport.pixel00_loc = vec3_add(viewport.upper_left, vec3_multiply_scalar(vec3_add(viewport.pixel_delta_u, viewport.pixel_delta_v), 0.5));
	viewport.defocus_radius = minirt->scene.camera.focus_dist * tan(minirt->scene.camera.defocus_angle * PI_10D / 360);;
	viewport.defocus_disk_u = vec3_multiply_scalar(u, viewport.defocus_radius);
	viewport.defocus_disk_v = vec3_multiply_scalar(vec3_cross(minirt->scene.camera.orientation, u), viewport.defocus_radius);
	return (viewport);
}

void	render(t_minirt *minirt)
{
	t_uint	tpix;
	t_uint	i;

	if (!minirt->screen.start_render || minirt->screen.pause_render)
		return ;
	if (minirt->screen.sample == 0)
	{

		i = -1;
		tpix = minirt->mlx.img.width * minirt->mlx.img.height;
		while (++i < tpix)
			ft_bzero(&minirt->screen.render[i].color, sizeof(t_lcolor));
		minirt->viewport = init_viewport(minirt);
	}

	draw_pixels(minirt);
	if (minirt->screen.sample == minirt->screen.spp)
	{
		minirt->screen.sample = 0;
		minirt->screen.start_render = 0;
	}
}

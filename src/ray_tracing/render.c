/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/28 13:29:39 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "material.h"
#include <math.h>

void	calc_one_sample(t_minirt *minirt, t_vec3 offset)
{
	t_color			color;
	const t_uint	tpi = minirt->viewport.render_w * minirt->viewport.render_h;
	t_uint			i;
	t_ray			ray;
	char			bounce_hit;

	bounce_hit = 0;
	i = 0;
	while (i < tpi)
	{
		if (minirt->scene.camera.defocus_angle <= 0)
			ray.orig = minirt->scene.camera.position;
		else
			ray.orig = defocus_disk_sample(minirt);
		ray.dir = vec3_subtract(vec3_add(vec3_add(minirt->viewport.pixel00_loc,
						vec3_multiply_scalar(minirt->viewport.pixel_delta_u,
							(i % minirt->viewport.render_w) + offset.x)),
					vec3_multiply_scalar(minirt->viewport.pixel_delta_v,
						(i / minirt->viewport.render_w) + offset.y)), ray.orig);
		color = ray_color(minirt, ray, 20, &bounce_hit).color;
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
	minirt->screen.last_sample_am = minirt->screen.sample;
	put_render_to_frame(minirt);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
	printf("Sample %d\n", minirt->screen.sample);
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
		minirt->viewport = init_viewport(minirt);
		tpix = minirt->viewport.render_w * minirt->viewport.render_h;
		while (++i < tpix)
			ft_bzero(&minirt->screen.render[i].color, sizeof(t_lcolor));
	}
	draw_pixels(minirt);
	if (minirt->screen.sample == minirt->screen.spp)
	{
		minirt->screen.sample = 0;
		minirt->screen.start_render = 0;
	}
}

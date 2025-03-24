/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 11:07:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

// void	basic_image(t_minirt *minirt)
// {
// 	t_uint	i;
// 	t_uint	y;
// 	t_color	color;

// 	i = 0;
// 	y = 0;
// 	while (i < minirt->mlx.img.height)
// 	{
// 		y = 0;
// 		while (y < minirt->mlx.img.width)
// 		{
// 			color.r = i * 255 / minirt->mlx.img.height;
// 			color.g = y * 255 / minirt->mlx.img.width;
// 			color.b = (minirt->mlx.img.height - i) * 255 / minirt->mlx.img.height;
// 			minirt->screen.render[minirt->mlx.img.width * i + y].color = color;
// 			y++;
// 		}
// 		i++;
// 	}
// }

t_color ray_color(t_minirt *minirt, t_ray ray)
{
	double			a;
	t_color			color;
	t_color			obj_color;
	t_hit_record	hit_record;
	t_vec3			adjusted_normal;

	a = 0.5 * (ray.dir.y + 1);
	color.r = (1 - a) * 255 + a * 128;
	color.g = (1 - a) * 255 + a * 178;
	color.b = (1 - a) * 255 + a * 255;
	if (hit_register(minirt, ray, &hit_record, &obj_color) == 1)
	{
		adjusted_normal = vec3_add(hit_record.normal, vec3_init(1.0, 1.0, 1.0));
		adjusted_normal = vec3_multiply_scalar(adjusted_normal, 0.5);
		color.r = (unsigned char)(adjusted_normal.x * obj_color.r);
		color.g = (unsigned char)(adjusted_normal.y * obj_color.g);
		color.b = (unsigned char)(adjusted_normal.z * obj_color.b);
		return (color);
	}
	return (color);
}

t_vec3	sample_square()
{
	float x = random_float() - 0.5;
	float y = random_float() - 0.5;
	return (t_vec3){ x, y, 0 };
}

void	calc_one_sample(t_minirt *minirt, t_vec3 offset)
{
	t_uint	tpix;
	t_uint	i;
	t_ray	ray;
	t_color color;

	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	printf("START\n");
	while (i < tpix)
	{		
		ray.orig = minirt->scene.camera.position;
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
		color = ray_color(minirt, ray);
		minirt->screen.render[i].color.r += color.r;
		minirt->screen.render[i].color.g += color.g;
		minirt->screen.render[i].color.b += color.b;
		i++;
	}
	printf("END\n");
}

void	draw_pixels(t_minirt *minirt)
{
	int		samples_per_pixel = 5;
	t_vec3	offset;
	t_uint	tpix;
	t_uint	i;

	i = -1;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	while (++i < tpix)
		ft_bzero(&minirt->screen.render[i].color, sizeof(t_color));
	minirt->screen.sample = 0;
	while (minirt->screen.sample < samples_per_pixel)
	{
		offset = sample_square();
		calc_one_sample(minirt, offset);
		minirt->screen.sample++;
		printf("%ld, %ld, %ld\n", minirt->screen.render[0].color.r, minirt->screen.render[0].color.g, minirt->screen.render[0].color.b);
		put_render_to_frame(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
		printf("Sample %d\n", minirt->screen.sample);
	}	
}

t_viewport	init_viewport(t_minirt *minirt)
{
	t_viewport	viewport;
	t_vec3		u;

	viewport.focal_length = 1.0;
	viewport.height = 2.0;
	viewport.width = viewport.height * ((double)minirt->mlx.img.width / minirt->mlx.img.height);
	u = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, vec3_negate(minirt->scene.camera.orientation)));
	viewport.u = vec3_multiply_scalar(u, viewport.width);
	viewport.v = vec3_multiply_scalar(vec3_cross(minirt->scene.camera.orientation, u), viewport.height);
	viewport.pixel_delta_u = vec3_divide_scalar(viewport.u, minirt->mlx.img.width);
	viewport.pixel_delta_v = vec3_divide_scalar(viewport.v, minirt->mlx.img.height);
	viewport.upper_left = vec3_subtract(
		vec3_subtract(
			vec3_add(minirt->scene.camera.position, vec3_multiply_scalar(minirt->scene.camera.orientation, viewport.focal_length)),
			vec3_divide_scalar(viewport.u, 2)
		),
		vec3_divide_scalar(viewport.v, 2)
	);
	viewport.pixel00_loc = vec3_add(viewport.upper_left, vec3_multiply_scalar(vec3_add(viewport.pixel_delta_u, viewport.pixel_delta_v), 0.5));
	return (viewport);
}

void	render(t_minirt *minirt, int sample)
{	
	minirt->viewport = init_viewport(minirt);
	// printf("u : %f %f %f\n", viewport.u.x, viewport.u.y, viewport.u.z);
	// printf("v : %f %f %f\n", viewport.v.x, viewport.v.y, viewport.v.z);

	// printf("LC : %f %f %f\n", viewport.upper_left.x, viewport.upper_left.y, viewport.upper_left.z);
	//basic_image(minirt);
	draw_pixels(minirt);
}

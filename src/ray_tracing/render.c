/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/24 11:33:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

void	basic_image(t_minirt *minirt)
{
	t_uint	i;
	t_uint	y;
	t_color	color;

	i = 0;
	y = 0;
	while (i < minirt->mlx.img.height)
	{
		y = 0;
		while (y < minirt->mlx.img.width)
		{
			color.r = i * 255 / minirt->mlx.img.height;
			color.g = y * 255 / minirt->mlx.img.width;
			color.b = (minirt->mlx.img.height - i) * 255 / minirt->mlx.img.height;
			minirt->screen.render[minirt->mlx.img.width * i + y].color = color;
			y++;
		}
		i++;
	}
}

t_color ray_color(t_minirt *minirt, t_ray ray, int depth)
{
	double			a;
	t_color			color;
	t_color			obj_color;
	t_hit_record	hit_record;
	t_vec3			adjusted_normal;
	t_vec3			direction;

	if (depth <= 0)
        return (t_color){0, 0, 0};
	a = 0.5 * (ray.dir.y + 1);
	color.r = (1 - a) * 255 + a * 128;
	color.g = (1 - a) * 255 + a * 178;
	color.b = (1 - a) * 255 + a * 255;
	if (hit_register(minirt, ray, &hit_record, &obj_color) == 1)
	{
		adjusted_normal = vec3_unit(hit_record.normal);
		direction = random_on_hemisphere(adjusted_normal);
		ray.dir = direction;
		ray.orig = hit_record.point;
		t_color bounce_color = ray_color(minirt, ray, depth - 1);
		color.r = 0.5 * bounce_color.r;
        color.g = 0.5 * bounce_color.g;
        color.b = 0.5 * bounce_color.b;
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

t_color	vec3_to_color(t_vec3 vec) {
	t_color color;
	color.r = (unsigned char)(vec.x * 255.0f);
	color.g = (unsigned char)(vec.y * 255.0f);
	color.b = (unsigned char)(vec.z * 255.0f);
	return color;
}

t_vec3	color_to_vec3(t_color color) {
	t_vec3 vec;
	vec.x = color.r / 255.0f;
	vec.y = color.g / 255.0f;
	vec.z = color.b / 255.0f;
	return vec;
}

t_vec3	apply_pixel_samples_scale(t_vec3 color, float pixel_samples_scale)
{
	return ((t_vec3){
		color.x * pixel_samples_scale,
		color.y * pixel_samples_scale,
		color.z * pixel_samples_scale
	});
}

void	draw_pixels(t_minirt *minirt)
{
	t_uint	tpix;
	t_uint	i;
	t_ray	ray;
	int		samples_per_pixel = 5;
	int		sample;
	float	pixel_samples_scale = 1.0 / samples_per_pixel;
	t_vec3 color;

	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	while (i < tpix)
	{
		color = vec3_init(0, 0, 0);
		sample = 0;
		while (sample < samples_per_pixel)
		{
			t_vec3 offset = sample_square();
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
			color = vec3_add(color, color_to_vec3(ray_color(minirt, ray, 2)));
			sample++;
		}
		if (i % 10000 == 0)
			ft_printf("%d/%d\n", i, tpix);
		minirt->screen.render[i].color = vec3_to_color(apply_pixel_samples_scale(color, pixel_samples_scale));
		i++;
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

void	render(t_minirt *minirt)
{	
	minirt->viewport = init_viewport(minirt);
	// printf("u : %f %f %f\n", viewport.u.x, viewport.u.y, viewport.u.z);
	// printf("v : %f %f %f\n", viewport.v.x, viewport.v.y, viewport.v.z);

	// printf("LC : %f %f %f\n", viewport.upper_left.x, viewport.upper_left.y, viewport.upper_left.z);
	//basic_image(minirt);
	draw_pixels(minirt);
}

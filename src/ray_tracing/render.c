/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/21 16:04:46 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

t_color ray_color(t_minirt *minirt, t_ray ray)
{
	double	a;
	t_color	color;
	int	i;

	a = 0.5 * (ray.dir.y + 1);
	color.r = (1 - a) * 255 + a * 128;
	color.g = (1 - a) * 255 + a * 178;
	color.b = (1 - a) * 255 + a * 255;
	i = 0;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == SPHERE)
		{
			t_sphere *sphere;
			sphere = minirt->scene.elements[i].object;
			if (hit_sphere(sphere->position, sphere->diameter / 2, &ray))
			{
				return (sphere->color);
			}
		}
		i++;
	}
    return (color);
}

void	draw_pixels(t_minirt *minirt)
{
	t_uint	tpix;
	t_uint	i;
	t_ray	ray;

	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	while (i < tpix)
	{
		ray.orig = minirt->scene.camera.position;
		ray.dir = vec3_subtract(
			vec3_add(
				vec3_add(
					minirt->viewport.pixel00_loc, 
					vec3_multiply_scalar(minirt->viewport.pixel_delta_u, i % minirt->mlx.img.width)
				), 
				vec3_multiply_scalar(minirt->viewport.pixel_delta_v, i / minirt->mlx.img.width)
			), 
			ray.orig
		);
		minirt->screen.render[i].color = ray_color(minirt, ray);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/25 14:41:03 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color ray_color(t_minirt *minirt, t_ray ray, int depth)
{
	double			a;
	t_color			color;
	t_color			attenuation;
	t_hit_record	hit_record;
	t_ray			scatted;

	if (depth <= 0)
		return (t_color){0, 0, 0};
	a = 0.5 * (ray.dir.y + 1);
	color.r = (1 - a) * 255 + a * 128;
	color.g = (1 - a) * 255 + a * 178;
	color.b = (1 - a) * 255 + a * 255;
	if (hit_register(minirt, ray, &hit_record) == 1)
	{
		if (calc_ray_reflection(hit_record, ray, &scatted, &attenuation))
		{
			t_color bounce_color = ray_color(minirt, scatted, depth - 1);
			color.r = bounce_color.r * attenuation.r / 255;
			color.g = bounce_color.g * attenuation.g / 255;
			color.b = bounce_color.b * attenuation.b / 255;
			return (color);
		}
		return (t_color){0, 0, 0};
	}
	return (color);
}

t_vec3	sample_square()
{
	double x = random_double() - 0.5;
	double y = random_double() - 0.5;
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
		color = ray_color(minirt, ray, 10);
		minirt->screen.render[i].color.r += color.r * minirt->viewport.gamma;
		minirt->screen.render[i].color.g += color.g * minirt->viewport.gamma;
		minirt->screen.render[i].color.b += color.b * minirt->viewport.gamma;
		i++;
	}
}

void	draw_pixels(t_minirt *minirt)
{
	t_vec3	offset;

	offset = sample_square();
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

	((t_sphere *)minirt->scene.elements[0].object)->material = malloc(sizeof(t_mat) * 1);
	((t_sphere *)minirt->scene.elements[1].object)->material = malloc(sizeof(t_mat) * 1);
	((t_sphere *)minirt->scene.elements[2].object)->material = malloc(sizeof(t_mat) * 1);
	((t_sphere *)minirt->scene.elements[3].object)->material = malloc(sizeof(t_mat) * 1);
	((t_sphere *)minirt->scene.elements[0].object)->material->metallic_value = 0;
	((t_sphere *)minirt->scene.elements[1].object)->material->metallic_value = 0;
	((t_sphere *)minirt->scene.elements[2].object)->material->metallic_value = 1;
	((t_sphere *)minirt->scene.elements[3].object)->material->metallic_value = 2;
	((t_sphere *)minirt->scene.elements[0].object)->material->color_value = ((t_sphere *)minirt->scene.elements[0].object)->color;
	((t_sphere *)minirt->scene.elements[1].object)->material->color_value = ((t_sphere *)minirt->scene.elements[1].object)->color;
	((t_sphere *)minirt->scene.elements[2].object)->material->color_value = ((t_sphere *)minirt->scene.elements[2].object)->color;
	((t_sphere *)minirt->scene.elements[3].object)->material->color_value = ((t_sphere *)minirt->scene.elements[3].object)->color;
	viewport.gamma = sqrt(0.8);
	viewport.height = 2.0;
	viewport.focal_length = viewport.height / (2.0 * tan(minirt->scene.camera.fov * 0.5 * PI_10D / 180));
	viewport.width = viewport.height * ((float)minirt->mlx.img.width / minirt->mlx.img.height);
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
	t_uint	tpix;
	t_uint	i;

	if (!minirt->screen.start_render)
		return ;
	if (minirt->screen.sample == 0)
	{

		i = -1;
		tpix = minirt->mlx.img.width * minirt->mlx.img.height;
		while (++i < tpix)
			ft_bzero(&minirt->screen.render[i].color, sizeof(t_color));
		minirt->viewport = init_viewport(minirt);
	}

	draw_pixels(minirt);
	if (minirt->screen.sample == minirt->screen.spp)
	{
		minirt->screen.sample = 0;
		minirt->screen.start_render = 0;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/04 14:36:02 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <math.h>

t_vec3	random_in_unit_disk()
{
	t_vec3	p;

	p.z = 0;
	while (1)
	{
		p.x = random_double_in_interval(-1, 1);
		p.y = random_double_in_interval(-1, 1);
		if (vec3_length_squared(p) < 1)
			return (p);
	}
	return (p);
}

t_vec3 defocus_disk_sample(t_minirt *minirt)
{
	t_vec3	random_point;

	random_point = random_in_unit_disk();
	return (vec3_add(
		minirt->scene.camera.position,
		vec3_add(
			vec3_multiply_scalar(minirt->viewport.defocus_disk_u, random_point.x),
			vec3_multiply_scalar(minirt->viewport.defocus_disk_v, random_point.y)
		)
	));
}

char	is_hit_before_target(t_minirt *minirt, t_vec3 origin, t_vec3 target)
{
	t_ray			shadow_ray;
	t_hit_record	hit_record;
	double			distance_to_target;

	shadow_ray.orig = origin;
	shadow_ray.dir = vec3_unit(vec3_subtract(target, origin));
	distance_to_target = vec3_length(vec3_subtract(target, origin));
	if (hit_register(minirt, shadow_ray, &hit_record))
	{
		if (hit_record.t < distance_to_target)
			return (1);
	}
	return (0);
}

double	compute_light(t_hit_record *hit_record, t_minirt *minirt)
{
	t_lcolor	light_color;
	t_vec3		light_dir;
	t_light		*light;
	double		n_dot_l;
	double		light_intensity;
	int			i;
	int			count;

	light_intensity = minirt->scene.amb_light.ratio;
	light_color = (t_lcolor){0, 0, 0};
	// light_intensity = minirt->scene.amb_light.ratio;
	// light_color = (t_lcolor){minirt->scene.amb_light.color.r * light_intensity, minirt->scene.amb_light.color.g * light_intensity, minirt->scene.amb_light.color.b * light_intensity};
	i = 0;
	count = 1;
	while (i < minirt->scene.el_amount)
	{
		if (minirt->scene.elements[i].type == LIGHT)
		{
			light = minirt->scene.elements[i].object;
			light_dir = vec3_unit(vec3_subtract(light->position, hit_record->point));
			if (is_hit_before_target(minirt, hit_record->point, light->position) == 1)
			{
				i++;
				continue ;
			}
			n_dot_l = vec3_dot(hit_record->normal, light_dir);
			if (n_dot_l > 0.0)
			{
				light_intensity += (light->brightness * n_dot_l);
				count++;
			}
		}
		i++;
	}
	return (fmin(light_intensity, 1.0));
}

t_color ray_color(t_minirt *minirt, t_ray ray, int depth)
{
	t_color			color;
	// t_color			attenuation;
	t_hit_record	hit_record;
	// t_color			bounce_color;
	t_color			amb;
	double			ratio;

	if (depth <= 0)
		return (t_color){0, 0, 0};
	color = minirt->scene.amb_light.color;
	amb = minirt->scene.amb_light.color;
	ratio = minirt->scene.amb_light.ratio;
	color.r *= minirt->scene.amb_light.ratio;
	color.g *= minirt->scene.amb_light.ratio;
	color.b *= minirt->scene.amb_light.ratio;
	if (hit_register(minirt, ray, &hit_record) == 1)
	{
		// bounce_color = ray_color(minirt, scatted, depth - 1);
		ratio = compute_light(&hit_record, minirt);
		color.r = ratio * hit_record.color.r;
		color.g = ratio * hit_record.color.g;
		color.b = ratio * hit_record.color.b;
		return (color);
	}
	return (t_color){0, 0, 0};
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

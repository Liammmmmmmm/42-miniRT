/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_trace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:48:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/16 11:24:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	material_manager_v3(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data data)
{
	if (!hit_record->mat)
		return (default_mat(minirt, ray, hit_record, data));
	if (hit_record->mat->emission_strength > 0)
	{
		*data.accumulation = add_fcolor(*data.accumulation,
				multiply_fcolor(multiply_scalar_fcolor(
				color_to_fcolor(hit_record->mat->emission_color),
				hit_record->mat->emission_strength), *data.power));
	}
	if (hit_record->mat->metallic_value == 1.0)
		metallic_color(ray, hit_record, data.power);
	else if (hit_record->mat->metallic_value == 0.0)
		dielectric_mat(minirt, ray, hit_record, data);
	else if (hit_record->mat->metallic_value > random_double())
		metallic_color(ray, hit_record, data.power);
	else
		dielectric_mat(minirt, ray, hit_record, data);
}

static inline t_fcolor	add_skybox(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data data)
{
	if (hit_record->mat)
	{
		return (add_fcolor(*data.accumulation, multiply_fcolor(\
multiply_scalar_fcolor(get_background_color(minirt, *ray), \
minirt->scene.amb_light.ratio * hit_record->mat->ao_value), *data.power)));
	}
	return (add_fcolor(*data.accumulation, multiply_fcolor(\
	multiply_scalar_fcolor(get_background_color(minirt, *ray),
					minirt->scene.amb_light.ratio), *data.power)));
}

t_fcolor	path_trace(t_minirt *minirt, t_ray ray, int max_depth)
{
	t_fcolor		accumulation;
	t_fcolor		power;
	t_hit_record	hit_record;

	accumulation = (t_fcolor){0.0, 0.0, 0.0};
	power = (t_fcolor){1.0, 1.0, 1.0};
	hit_record.mat = NULL;
	while (--max_depth >= 0)
	{
		if (hit_register_all(minirt, &ray, &hit_record) == 1)
		{
			if (minirt->scene.bvh.normal_mode) // A deplacer pour pas bouffer de perf
				return (hit_record.color);
			ray.orig = hit_record.point;
			material_manager_v3(minirt, &ray, &hit_record,
				(t_ray_data){&power, &accumulation});
		}
		else
		{
			accumulation = add_skybox(minirt, &ray, &hit_record,
					(t_ray_data){&power, &accumulation});
			break ;
		}
	}
	return (accumulation);
}

t_fcolor	path_trace_normal(t_minirt *minirt, t_ray ray)
{
	t_hit_record	hit_record;

	if (hit_register_all(minirt, &ray, &hit_record) == 1)
		return (hit_record.color);
	return (t_fcolor){0, 0, 0};
}

void	debug_path_trace(t_minirt *minirt, t_ray ray, int max_depth)
{
	t_fcolor		accumulated_color;
	t_fcolor		power;
	t_hit_record	hit_record;
	int				bounce;

	bounce = 0;
	accumulated_color = (t_fcolor){0.0, 0.0, 0.0};
	power = (t_fcolor){1.0, 1.0, 1.0};
	while (bounce < max_depth && bounce < 10)
	{
		minirt->controls.traced_ray[bounce] = ray.orig;
		printf("hit : ");
		print_vec3(minirt->controls.traced_ray[bounce]);
		printf(" - dir: ");
		print_vec3(ray.dir);
		printf("\n");
		if (hit_register_all(minirt, &ray, &hit_record) == 1)
		{
			ray.orig = hit_record.point;
			material_manager_v3(minirt, &ray, &hit_record,
				(t_ray_data){&power, &accumulated_color});
		}
		else
		{
			minirt->controls.traced_ray[bounce + 1]
				= vec3_add(ray.orig, vec3_multiply_scalar(ray.dir, 4));
			printf("skybox : ");
			print_vec3(minirt->controls.traced_ray[bounce + 1]);
			printf(" - dir: ");
			print_vec3(ray.dir);
			printf("\n");
			break ;
		}
		bounce++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_trace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:48:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/28 18:41:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	material_manager_v3(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data data)
{
	if (!hit_record->mat)
		return (default_mat(minirt, ray, hit_record, data));
	if (hit_record->mat->emission_strength > 0)
	{
		*data.accumulation = add_fcolor(*data.accumulation,
				multiply_fcolor(multiply_scalar_fcolor(
						hit_record->mat->emission_color,
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

inline t_fcolor	add_skybox(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data data)
{
	if (hit_record->mat)
	{
		return (add_fcolor(*data.accumulation, multiply_fcolor(\
multiply_scalar_fcolor(get_background_color_clamp(minirt, *ray), \
hit_record->mat->ao_value), *data.power)));
	}
	return (add_fcolor(*data.accumulation, multiply_fcolor(
				get_background_color_clamp(minirt, *ray), *data.power)));
}

t_fcolor	path_trace(t_minirt *minirt, t_ray ray, int max_depth, int i)
{
	t_fcolor			accumulation;
	t_fcolor			power;
	t_hit_register_data	data;
	int					initial_depth;

	accumulation = (t_fcolor){0.0, 0.0, 0.0};
	power = (t_fcolor){1.0, 1.0, 1.0};
	ft_bzero(&data, sizeof(t_hit_register_data));
	data.ray = &ray;
	initial_depth = max_depth;
	while (--max_depth >= 0)
	{
		if (hit_register_all(minirt, &data) == 1)
		{
			if (max_depth == initial_depth - 1)
			{
				minirt->screen.albedo_buffer[i].r += data.hit_record.color.r;
				minirt->screen.albedo_buffer[i].g += data.hit_record.color.g;
				minirt->screen.albedo_buffer[i].b += data.hit_record.color.b;
				minirt->screen.normal_buffer[i].r += (data.hit_record.normal.x + 1.0) * 0.5;
				minirt->screen.normal_buffer[i].g += (data.hit_record.normal.y + 1.0) * 0.5;
				minirt->screen.normal_buffer[i].b += (data.hit_record.normal.z + 1.0) * 0.5;
				minirt->screen.depth_buffer_denoise[i] += data.hit_record.t;
			}

			ray.orig = data.hit_record.point;
			material_manager_v3(minirt, &ray, &data.hit_record,
				(t_ray_data){&power, &accumulation});
		}
		else
		{
			accumulation = add_skybox(minirt, &ray, &data.hit_record,
					(t_ray_data){&power, &accumulation});
			break ;
		}
	}
	return (accumulation);
}

t_fcolor	path_trace_normal(t_minirt *minirt, t_ray ray)
{
	t_hit_register_data	data;

	data.hit_record.mat = NULL;
	data.is_light = 0;
	data.ray = &ray;
	data.depth = 0;
	if (hit_register_all(minirt, &data) == 1)
		return (data.hit_record.color);
	return ((t_fcolor){0, 0, 0});
}

int	path_trace_bvh(t_minirt *minirt, t_ray ray)
{
	t_hit_register_data	data;

	data.hit_record.mat = NULL;
	data.is_light = 0;
	data.ray = &ray;
	data.depth = 0;
	if (hit_register_all(minirt, &data) == 1)
		return (data.depth);
	return (-1);
}

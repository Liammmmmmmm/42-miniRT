/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/18 13:11:17 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "material.h"

inline char	default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
	t_ray_data data)
{
	t_hit_register_data	data_tmp;
	t_vec2	uv;


	*data.accumulation = add_fcolor(
		*data.accumulation,
		multiply_fcolor(
			multiply_fcolor(
				hit_record->color,
				*data.power
				),
			compute_light_v2(hit_record, minirt)
			)
		);
	*data.power = multiply_fcolor(
			*data.power,
			hit_record->color
			);

	data_tmp.interval = (t_interval){0.001, 1000};
	data_tmp.is_light = 1;
	data_tmp.ray = ray;
	uv = calc_inverse_transform_sampling_uv(&minirt->scene);
	ray->dir = calc_inverse_transform_sampling_dir(&uv);
	if (hit_register_all(minirt, &data_tmp) == 0)
	{
		int width = minirt->scene.amb_light.skybox_t->hdr.width;
		int height = minirt->scene.amb_light.skybox_t->hdr.height;

		// Convertit uv [0,1] en coordonnées d’image
		int ix = (int)(uv.x * (float)width);
		int iy = (int)(uv.y * (float)height);

		// Clamp au cas où uv.x ou uv.y seraient à 1.0
		if (ix >= width) ix = width - 1;
		if (iy >= height) iy = height - 1;

		float pdf = minirt->scene.amb_light.pdf_joint[iy * width + ix];

		pdf = fmaxf(pdf, 1e-6f);
		// Pour éviter toute division par zéro
		t_fcolor radiance = get_background_color(minirt, *ray);
		radiance = multiply_scalar_fcolor(radiance, 1.0f / pdf);
		radiance = multiply_fcolor(radiance, *data.power);
		*data.accumulation = add_fcolor(*data.accumulation, radiance);
		return (1);
	}
	/*
	ray->dir = calc_inverse_transform_sampling_dir()

	if (hit_register_all == 0)
		on a rien tapé donc on renvoie la skybox;
	else
	{
		il y a quelque chose sur la trajectoire de la light, donc on reprend
		le systeme de base
		  |
		 \/
	}

	*/
	ray->dir = vec3_add(hit_record->normal, vec3_random_unit());
	if (fabs(ray->dir.x) < 1e-8 && fabs(ray->dir.y) < 1e-8
		&& fabs(ray->dir.z) < 1e-8)
		ray->dir = hit_record->normal;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 10:29:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "material.h"

static inline t_vec3	cos_weighted_sample_hemishphere(t_vec3 *normal)
{
	const double	r1 = random_double();
	const double	r2 = random_double();
	const double	r2_sqrt = sqrt(r2);
	const double	phi = 2 * PI_D * r1;
	t_vec3			local_dir;

	local_dir.x = cos(phi) * r2_sqrt;
	local_dir.y = sqrt(1 - r2);
	local_dir.z = sin(phi) * r2_sqrt;

	t_vec3 w = vec3_unit(*normal);
	t_vec3 a = (fabs(w.y) < 0.999) ? (t_vec3){0, 1, 0} : (t_vec3){1, 0, 0};
	t_vec3 u = vec3_unit(vec3_cross(a, w));
	t_vec3 v = vec3_cross(w, u);

	t_vec3 global_dir = vec3_add(
		vec3_add(
			vec3_multiply_scalar(u, local_dir.x),
			vec3_multiply_scalar(w, local_dir.y)
		),
		vec3_multiply_scalar(v, local_dir.z)
	);

	return (vec3_unit(global_dir));
}



inline void	default_mat(t_minirt *minirt, t_ray *ray, t_hit_record *hit_record,
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
		float costheta = vec3_dot(hit_record->normal, ray->dir);
		float pdf = minirt->scene.amb_light.pdf_joint[(int)(uv.y * (minirt->scene.amb_light.skybox_t->hdr.height - 1) * minirt->scene.amb_light.skybox_t->hdr.width + uv.x * (minirt->scene.amb_light.skybox_t->hdr.width - 1))];
		if (pdf > 0)
		{
			t_fcolor radiance = get_background_color(minirt, *ray);
			if (hit_record->mat)
				radiance = multiply_scalar_fcolor(radiance, hit_record->mat->ao_value);
			radiance = multiply_fcolor(radiance, *data.power);
			radiance = multiply_scalar_fcolor(radiance, costheta / (pdf));
			*data.accumulation = add_fcolor(*data.accumulation, radiance);
			*data.power = multiply_scalar_fcolor(*data.power, 1 - costheta);
		}
	}

	ray->dir = cos_weighted_sample_hemishphere(&hit_record->normal);
}

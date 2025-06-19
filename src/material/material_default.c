/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_default.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:14 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/19 16:30:12 by lilefebv         ###   ########lyon.fr   */
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
		int ix = (int)(uv.x * (float)(width - 1));
		int iy = (int)(uv.y * (float)(height - 1));

		float costheta = vec3_dot(hit_record->normal, ray->dir);

		// t_rgbe rgbe = minirt->scene.amb_light.skybox_t->hdr.pixels[iy * width + ix];
		// printf("r:%u g:%u b:%u e:%u\n", rgbe.r, rgbe.g, rgbe.b, rgbe.e);
		
		if (costheta > random_double())
		{
			float pdf = fmaxf(minirt->scene.amb_light.pdf_joint[iy * width + ix], 1e-6f);
			t_fcolor radiance = get_background_color(minirt, *ray);
			if (hit_record->mat)
				radiance = multiply_scalar_fcolor(radiance, hit_record->mat->ao_value);
			radiance = multiply_fcolor(radiance, *data.power);
			// if (radiance.r > 1.0)
			// printf("AV %f %f %f, PDF: %f, CT: %f\n", radiance.r, radiance.g, radiance.b, pdf, costheta);
			radiance = multiply_scalar_fcolor(radiance, 1 / (pdf));
			// printf("NX %f %f %f\n", radiance.r, radiance.g, radiance.b);
			// printf("PDF : %f\n", pdf);
			*data.accumulation = add_fcolor(*data.accumulation, radiance);
			return (1);
		}

		//printf("%f %f\n", pdf, pdf);
		
	}

	ray->dir = cos_weighted_sample_hemishphere(&hit_record->normal);
	return (0);
}

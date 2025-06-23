/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micrort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 10:52:16 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 18:36:43 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "hit_register.h"

void	hit_sphere_micrort(t_minirt *minirt, t_ray *ray,
	t_hit_record *hit_record, t_ray_data *data)
{
	hit_record->mat = minirt->controls.ui_infos.selected_material;
	apply_all_map(hit_record, 0);
	if (hit_record->mat->color_tex)
		hit_record->color = get_tex_color(hit_record->mat->color_tex,
				hit_record->u, hit_record->v, hit_record->point);
	else
		hit_record->color = color_to_fcolor(hit_record->mat->color_value);
	ray->orig = hit_record->point;
	material_manager_v3(minirt, ray, hit_record, *data);
}

t_fcolor	path_trace_micrort(t_minirt *minirt, t_ray ray, int max_depth)
{
	t_hit_record	hit_record;
	t_fcolor		ac;
	t_fcolor		po;
	t_ray_data		data;

	ac = (t_fcolor){0.0, 0.0, 0.0};
	po = (t_fcolor){1.0, 1.0, 1.0};
	data.accumulation = &ac;
	data.power = &po;
	hit_record.mat = NULL;
	while (--max_depth >= 0)
	{
		if (hit_sphere(&minirt->micrort.sphere, &ray, &hit_record))
			hit_sphere_micrort(minirt, &ray, &hit_record, &data);
		else
		{
			ac = add_skybox(minirt, &ray, &hit_record, data);
			break ;
		}
	}
	return (ac);
}

void	calc_one_sample_micrort(t_minirt *minirt, t_vec3 offset)
{
	t_fcolor		color;
	const t_uint	tpi = minirt->micrort.viewport.render_w
		* minirt->micrort.viewport.render_h;
	t_uint			i;
	t_ray			ray;

	i = 0;
	while (i < tpi)
	{
		if (minirt->micrort.camera.defocus_angle <= 0)
			ray.orig = minirt->micrort.camera.position;
		else
			ray.orig = defocus_disk_sample(minirt);
		ray.dir \
	= vec3_subtract(vec3_add(vec3_add(minirt->micrort.viewport.pixel00_loc, \
		vec3_multiply_scalar(minirt->micrort.viewport.pixel_delta_u, \
		(i % minirt->micrort.viewport.render_w) + offset.x)), \
		vec3_multiply_scalar(minirt->micrort.viewport.pixel_delta_v, \
		(i / minirt->micrort.viewport.render_w) + offset.y)), ray.orig);
		color = path_trace_micrort(minirt, ray, 3);
		minirt->micrort.render[i].r += color.r;
		minirt->micrort.render[i].g += color.g;
		minirt->micrort.render[i].b += color.b;
		i++;
	}
}

void	render_micrort(t_minirt *minirt)
{
	t_vec3	offset;

	if (minirt->controls.ui_infos.tab_selected != 1
		|| !minirt->controls.ui_infos.selected_material)
		return ;
	if (minirt->micrort.sample == 0)
		ft_bzero(minirt->micrort.render, sizeof(t_fcolor) \
	* minirt->micrort.viewport.render_w * minirt->micrort.viewport.render_h);
	if (minirt->micrort.sample < minirt->micrort.max_sample)
	{
		offset = vec3_random(&minirt->rand);
		calc_one_sample_micrort(minirt, offset);
		minirt->micrort.sample++;
	}
}

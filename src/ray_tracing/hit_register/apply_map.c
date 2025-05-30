/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:59:40 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/30 17:55:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hit_register.h"
#include "maths.h"

void	apply_all_map(t_hit_register_data *data)
{
	if (data->hit_record.mat == NULL)
		return ;
	if (data->hit_record.mat->transmission_tex != NULL)
		data->hit_record.mat->transmission_value = get_tex_color(data->hit_record.mat->transmission_tex, data->hit_record.u, data->hit_record.v, data->hit_record.point).r;
	if (data->is_light == 1)
		return ;
	if (data->hit_record.mat->roughness_tex != NULL)
		data->hit_record.mat->roughness_value = get_tex_color(data->hit_record.mat->roughness_tex, data->hit_record.u, data->hit_record.v, data->hit_record.point).r;
	if (data->hit_record.mat->metallic_tex != NULL)
		data->hit_record.mat->metallic_value = get_tex_color(data->hit_record.mat->metallic_tex, data->hit_record.u, data->hit_record.v, data->hit_record.point).r;
	if (data->hit_record.mat->ao_tex != NULL)	
		data->hit_record.mat->ao_value = get_tex_color(data->hit_record.mat->ao_tex, data->hit_record.u, data->hit_record.v, data->hit_record.point).r;
	if (data->hit_record.mat->emission_strength_tex != NULL)
		data->hit_record.mat->emission_strength = get_tex_color(data->hit_record.mat->emission_strength_tex, data->hit_record.u, data->hit_record.v, data->hit_record.point).r;
	if (data->hit_record.mat->emission_color_tex != NULL)
		data->hit_record.mat->emission_color = get_tex_color(data->hit_record.mat->emission_color_tex, data->hit_record.u, data->hit_record.v, data->hit_record.point);
}

void	apply_normal_map(t_hit_record *hit)
{
	t_color	map;

	if (hit->mat == NULL || hit->mat->normal == NULL || hit->mat->normal->img.pixel_data == NULL)
		return ;
	map = hit->mat->normal->img.pixel_data[(hit->mat->normal->img.width)
		* (int)(hit->v * (hit->mat->normal->img.height - 1)) + 
			(int)(hit->u * (hit->mat->normal->img.width - 1))];
	
	t_vec3 normal_map;
	normal_map.x = (map.r / 127.5f) - 1.0f;
	normal_map.y = (map.g / 127.5f) - 1.0f;
	normal_map.z = (map.b / 127.5f) - 1.0f;

	double	tbn[3][3];
	t_vec3	tangent;
	t_vec3	bi_tangent;
	
	tangent = vec3_cross(hit->normal, (t_vec3){0, -1, 0});
	if (vec3_length(tangent) == 0)
		tangent = vec3_cross(hit->normal, (t_vec3){0, 0, 1});
	tangent = vec3_unit(tangent);
	bi_tangent = vec3_unit(vec3_cross(hit->normal, tangent));
	tbn[0][0] = tangent.x;
	tbn[1][0] = tangent.y;
	tbn[2][0] = tangent.z;
	tbn[0][1] = bi_tangent.x;
	tbn[1][1] = bi_tangent.y;
	tbn[2][1] = bi_tangent.z;
	tbn[0][2] = hit->normal.x;
	tbn[1][2] = hit->normal.y;
	tbn[2][2] = hit->normal.z;
	if (hit->mat->normal_intensity == 1.0)
		hit->normal = vec3_unit(matrix3_dot_vec3(tbn, normal_map));
	else
	{
		t_vec3 mapped_normal = vec3_unit(matrix3_dot_vec3(tbn, normal_map));
		hit->normal = vec3_unit(vec3_lerp(hit->normal, mapped_normal, hit->mat->normal_intensity));
	}
}

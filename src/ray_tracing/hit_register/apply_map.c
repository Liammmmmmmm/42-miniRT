/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:59:40 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/17 12:27:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hit_register.h"
#include "maths.h"

static void	init_tbn(double tbn[3][3], t_hit_record *hit)
{
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
}

static void	apply_normal_map(t_hit_record *hit)
{
	t_rgba	map;
	double	tbn[3][3];
	t_vec3	normal_map;

	if (hit->mat == NULL || hit->mat->normal == NULL
		|| hit->mat->normal->img.rgba == NULL)
		return ;
	map = hit->mat->normal->img.rgba[(hit->mat->normal->img.width)
		* (int)(hit->v * (hit->mat->normal->img.height - 1))
		+ (int)(hit->u * (hit->mat->normal->img.width - 1))];
	normal_map.x = (map.r / 127.5f) - 1.0f;
	normal_map.y = (map.g / 127.5f) - 1.0f;
	normal_map.z = (map.b / 127.5f) - 1.0f;
	init_tbn(tbn, hit);
	if (hit->mat->normal_intensity == 1.0)
		hit->normal = vec3_unit(matrix3_dot_vec3(tbn, normal_map));
	else
		hit->normal = vec3_unit(vec3_lerp(hit->normal, \
	vec3_unit(matrix3_dot_vec3(tbn, normal_map)), hit->mat->normal_intensity));
}

void	apply_all_map(t_hit_record *hr, char is_light)
{
	if (hr->mat == NULL)
		return ;
	if (hr->mat->normal != NULL)
		apply_normal_map(hr);
	if (hr->mat->transmission_tex != NULL)
		hr->mat->transmission_value = get_tex_color(hr->mat->transmission_tex,
				hr->u, hr->v, hr->point).r;
	if (is_light == 1)
		return ;
	if (hr->mat->roughness_tex != NULL)
		hr->mat->roughness_value = get_tex_color(hr->mat->roughness_tex,
				hr->u, hr->v, hr->point).r;
	if (hr->mat->metallic_tex != NULL)
		hr->mat->metallic_value = get_tex_color(hr->mat->metallic_tex, hr->u,
				hr->v, hr->point).r;
	if (hr->mat->ao_tex != NULL)
		hr->mat->ao_value = get_tex_color(hr->mat->ao_tex, hr->u, hr->v,
				hr->point).r;
	if (hr->mat->emission_strength_tex != NULL)
		hr->mat->emission_strength = get_tex_color(
				hr->mat->emission_strength_tex, hr->u, hr->v, hr->point).r;
	if (hr->mat->emission_color_tex != NULL)
		hr->mat->emission_color = get_tex_color(hr->mat->emission_color_tex,
				hr->u, hr->v, hr->point);
}

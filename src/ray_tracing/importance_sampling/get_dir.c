/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:37:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/18 11:51:30 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec2 calc_inverse_transform_sampling_uv(t_scene *scene) {
	const t_vec2 r = (t_vec2){random_double(), random_double()};
	t_vec2 uv;

	int height = scene->amb_light.skybox_t->hdr.height;
	int width = scene->amb_light.skybox_t->hdr.width;
	
	int y = (int)(r.y * (height - 1));
	float v = scene->amb_light.cdf_marginal_inverse[y];
	
	int x = (int)(r.x * (width - 1));
	float u = scene->amb_light.cdf_conditional_inverse[y * width + x];
	
	uv.x = u;
	uv.y = v;
	return (uv);
}

t_vec3    calc_inverse_transform_sampling_dir(t_vec2 *uv)
{
	t_vec3            n;
	const float        theta = (uv->y - 0.5) * PI_D;
	const float        phi = (0.5 - uv->x) * 2 * PI_D;

	n = (t_vec3){
		cos(theta) * cos(phi),
		sin(theta),
		cos(theta) * sin(phi)
	};
	return (n);
}
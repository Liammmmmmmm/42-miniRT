/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:37:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 19:16:11 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec2	calc_inverse_transform_sampling_uv(t_scene *scene, uint64_t *rand)
{
	const t_vec2	r = (t_vec2){random_double(rand), random_double(rand)};
	t_vec2			n;

	n.y = scene->amb_light.cdf_marginal_inverse[
		(int)(r.x * (scene->amb_light.skybox_t->hdr.height - 1))];
	n.x = scene->amb_light.cdf_conditional_inverse[
		(int)(n.y * (scene->amb_light.skybox_t->hdr.height - 1)
			* scene->amb_light.skybox_t->hdr.width + r.y
			* (scene->amb_light.skybox_t->hdr.width - 1))];
	return (n);
}

t_vec3	calc_inverse_transform_sampling_dir(const t_vec2 *uv)
{
	t_vec3		n;
	const float	theta = (uv->y - 0.5) * PI_D;
	const float	phi = (0.5 - uv->x) * 2 * PI_D;

	n = (t_vec3){
		cos(theta) * cos(phi),
		sin(theta),
		cos(theta) * sin(phi)
	};
	return (n);
}

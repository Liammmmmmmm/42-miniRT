/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   focus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 20:25:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/01 18:01:23 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "maths.h"

t_vec3	random_in_unit_disk(void)
{
	t_vec3	p;

	p.z = 0;
	while (1)
	{
		p.x = random_double_in_interval(-1, 1);
		p.y = random_double_in_interval(-1, 1);
		if (vec3_length_squared(p) < 1)
			return (p);
	}
	return (p);
}

t_vec3	defocus_disk_sample(t_minirt *minirt)
{
	t_vec3	random_point;

	random_point = random_in_unit_disk();
	return (vec3_add(\
		minirt->scene.camera.position, \
		vec3_add(vec3_multiply_scalar(minirt->viewport.defocus_disk_u, \
			random_point.x), \
		vec3_multiply_scalar(minirt->viewport.defocus_disk_v, random_point.y) \
	)));
}

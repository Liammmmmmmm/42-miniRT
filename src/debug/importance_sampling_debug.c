/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   importance_sampling_debug.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:05:40 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 12:58:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "importance_sampling.h"

void	simulate_importance_sampling_debug(t_scene *scene, int width,
	int height)
{
	int		z;
	t_vec2	aa;

	z = 0;
	while (z < 1000000)
	{
		aa = calc_inverse_transform_sampling_uv(scene);
		scene->amb_light.DEBUG_INVERSE_SAMPLING[(int)(aa.y * (height - 1)
				* width + aa.x * (width -1))] += 1;
		z++;
	}
}

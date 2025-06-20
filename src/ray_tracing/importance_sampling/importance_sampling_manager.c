/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   importance_sampling_manager.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:32:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 12:59:25 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "importance_sampling.h"

static void	calc_column_average(t_scene *scene, int height)
{
	int		i;
	double	sum;

	i = 0;
	sum = 0;
	while (i < height)
		sum += scene->amb_light.raw_average[i++];
	scene->amb_light.column_average = sum / height;
}

static void	init_sky_box_size(t_scene *scene, int *width, int *height)
{
	if (scene->amb_light.skybox_t->type == HDR)
	{
		*height = scene->amb_light.skybox_t->hdr.height;
		*width = scene->amb_light.skybox_t->hdr.width;
	}
	else
	{
		*height = scene->amb_light.skybox_t->img.height;
		*width = scene->amb_light.skybox_t->img.width;
	}
}

void	make_importance_sampling_map(t_scene *scene)
{
	int		height;
	int		width;

	if (!(scene->amb_light.skybox_t && ((scene->amb_light.skybox_t->type == HDR
					&& scene->amb_light.skybox_t->hdr.pixels)
				|| (scene->amb_light.skybox_t->type == IMAGE
					&& scene->amb_light.skybox_t->img.rgba))))
		return ;
	init_sky_box_size(scene, &width, &height);
	if (init_all_malloc_importance_sampling(scene, width, height) == 1)
		return ;
	calc_gray_scale(scene, width, height);
	calc_raw_average(scene, width, height);
	calc_column_average(scene, height);
	calc_pdfs(scene, width, height);
	calc_cdf_marginal_inv(scene, height);
	calc_cdf_conditional_inv(scene, width, height);
	free_unused_malloc_importance_sampling(scene);
}

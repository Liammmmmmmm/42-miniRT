/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grey_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:01:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/13 17:41:24 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	get_gray_value(t_scene *scene, size_t i)
{
	double	scale;

	if (scene->amb_light.skybox_t->type == HDR)
	{
		scale = ldexpf(1.0f, scene->amb_light.skybox_t->hdr.pixels[i].e - 128);
		return (0.2126f * (double)((scene->amb_light.skybox_t->hdr.pixels[i].r
			* scale) / 255.0f) + 0.7152f * \
			(double)((scene->amb_light.skybox_t->hdr.pixels[i].g * \
			scale) / 255.0f) + 0.0722f * (double) \
			((scene->amb_light.skybox_t->hdr.pixels[i].b * scale) / 255.0f));
	}
	else
		return ( \
	0.2126f * (double)(scene->amb_light.skybox_t->img.pixel_data[i].r / 255) \
	+ 0.7152f * (double)(scene->amb_light.skybox_t->img.pixel_data[i].g / 255) \
	+ 0.0722f * (double)(scene->amb_light.skybox_t->img.pixel_data[i].b / 255));
}

void	make_gray_color_map(t_scene *scene)
{
	size_t	i;
	size_t	size;

	if (scene->amb_light.skybox_t->type == HDR)
		size = scene->amb_light.skybox_t->hdr.height * \
			scene->amb_light.skybox_t->hdr.width;
	else
		size = scene->amb_light.skybox_t->img.height * \
			scene->amb_light.skybox_t->img.width;
	i = 0;
	while (i < size)
	{
		scene->amb_light.gray_scale[i] = get_gray_value(scene, i);
		i++;
	}
	
}

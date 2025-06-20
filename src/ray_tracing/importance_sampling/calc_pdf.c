/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_pdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:07:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 13:33:28 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "importance_sampling.h"

double	get_grey_value(t_scene *scene, size_t i)
{
	double	scale;

	if (scene->amb_light.skybox_t->type == IMAGE)
		return (
			0.2126 * (scene->amb_light.skybox_t->img.rgba[i].r / 255.0)
			+ 0.7152 * (scene->amb_light.skybox_t->img.rgba[i].g / 255.0)
			+ 0.0722 * (scene->amb_light.skybox_t->img.rgba[i].b / 255.0)
		);
	if (scene->amb_light.skybox_t->hdr.exposure == 0)
		scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128);
	else
		scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128)
			* powf(2.0, scene->amb_light.skybox_t->hdr.exposure);
	return (
		0.2126 * (scene->amb_light.skybox_t->hdr.pixels[i].r * scale)
		+ 0.7152 * (scene->amb_light.skybox_t->hdr.pixels[i].g * scale)
		+ 0.0722 * (scene->amb_light.skybox_t->hdr.pixels[i].b * scale)
	);
}

void	calc_gray_scale(t_scene *scene, int width, int height)
{
	size_t	size;
	size_t	i;

	size = height * width;
	i = 0;
	while (i < size)
	{
		scene->amb_light.gray_scale[i] = get_grey_value(scene, i);
		i++;
	}
}

void	calc_raw_average(t_scene *scene, int width, int height)
{
	int		i;
	int		j;
	double	sum;

	i = 0;
	while (i < height)
	{
		j = 0;
		sum = 0;
		while (j < width)
		{
			sum += scene->amb_light.gray_scale[i * width + j];
			j++;
		}
		scene->amb_light.raw_average[i] = sum / width;
		i++;
	}
}

static void	calc_pdf_conditional(t_scene *scene, int width, int height)
{
	int		i;
	int		j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			scene->amb_light.pdf_conditional[i * width + j]
				= scene->amb_light.gray_scale[i * width + j]
				/ (scene->amb_light.raw_average[i] / 255);
			j++;
		}
		i++;
	}
}

void	calc_pdfs(t_scene *scene, int width, int height)
{
	int		size;
	int		i;

	i = 0;
	size = height * width;
	while (i < size)
	{
		scene->amb_light.pdf_joint[i] = scene->amb_light.gray_scale[i]
			/ (scene->amb_light.column_average);
		i++;
	}
	i = 0;
	while (i < height)
	{
		scene->amb_light.pdf_marginal[i] = scene->amb_light.raw_average[i]
			/ (scene->amb_light.column_average / 255);
		i++;
	}
	calc_pdf_conditional(scene, width, height);
}

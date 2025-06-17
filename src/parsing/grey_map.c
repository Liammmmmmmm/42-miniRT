/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grey_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:01:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/17 15:01:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	get_grey_value(t_scene *scene, size_t i)
{
	double	scale;

	if (scene->amb_light.skybox_t->type == HDR)
	{
		if (scene->amb_light.skybox_t->hdr.exposure == 0)
			scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128);
		else
			scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128) * powf(2.0, scene->amb_light.skybox_t->hdr.exposure);
		return (
			0.2126f * (double)(scene->amb_light.skybox_t->hdr.pixels[i].r * scale) +
			0.7152f * (double)(scene->amb_light.skybox_t->hdr.pixels[i].g * scale) +
			0.0722f * (double)(scene->amb_light.skybox_t->hdr.pixels[i].b * scale));		
	}
	else
		return ( \
	0.2126f * (double)(scene->amb_light.skybox_t->img.rgba[i].r / 255) \
	+ 0.7152f * (double)(scene->amb_light.skybox_t->img.rgba[i].g / 255) \
	+ 0.0722f * (double)(scene->amb_light.skybox_t->img.rgba[i].b / 255));
}

void calc_cdf_marginal_inv(t_scene *scene)
{
	int height = scene->amb_light.skybox_t->hdr.height;
	scene->amb_light.cdf_marginal_inverse = ft_calloc(height, sizeof(double));

	double *cdf = malloc(height * sizeof(double));
	double sum = 0.0;
	int i;

	// Étape 1 : construire la CDF marginale non normalisée
	for (i = 0; i < height; i++) {
		sum += scene->amb_light.pdf_marginal[i];
		cdf[i] = sum;
	}

	// Étape 2 : normaliser la CDF
	for (i = 0; i < height; i++) {
		cdf[i] /= sum;
	}

	// Étape 3 : construire l'inverse de la CDF
	for (i = 0; i < height; i++) {
		double u = (double)i / (height - 1);
		int y;
		for (y = 0; y < height; y++) {
			if (cdf[y] >= u) {
				scene->amb_light.cdf_marginal_inverse[i] = (double)y / (height - 1);
				break;
			}
		}
	}

	free(cdf);

}

void calc_cdf_conditional_inv(t_scene *scene)
{
	int width = scene->amb_light.skybox_t->hdr.width;
	int height = scene->amb_light.skybox_t->hdr.height;
	
	scene->amb_light.cdf_conditional_inverse = calloc(height * width, sizeof(double));
	
	for (int y = 0; y < height; y++) {
		double *cdf = malloc(width * sizeof(double));
		double sum = 0.0;
	
		// Construire la CDF de la ligne y
		for (int x = 0; x < width; x++) {
			int index = y * width + x;
			sum += scene->amb_light.pdf_conditional[index] / width;
			cdf[x] = sum;
		}
	
		// Normaliser la CDF
		for (int x = 0; x < width; x++) {
			cdf[x] /= sum;
		}
	
		// Construire l'inverse de la CDF dans le tableau 1D
		for (int i = 0; i < width; i++) {
			double u = (double)i / (width - 1);
			for (int x = 0; x < width; x++) {
				if (cdf[x] >= u) {
					scene->amb_light.cdf_conditional_inverse[y * width + i] = (double)x / (width - 1);
					break;
				}
			}
		}
	
		free(cdf);
	}
	
}

void	make_grey_map(t_scene *scene)
{
	size_t	i;
	size_t	j;
	double	sum;
	size_t	size;

	scene->amb_light.gray_scale = ft_calloc(scene->amb_light.skybox_t->hdr.width * scene->amb_light.skybox_t->hdr.height, sizeof(double));
	if (scene->amb_light.gray_scale == NULL)
		return ;
	if (scene->amb_light.skybox_t->type == HDR)
		size = scene->amb_light.skybox_t->hdr.height * \
			scene->amb_light.skybox_t->hdr.width;
	else
		size = scene->amb_light.skybox_t->img.height * \
			scene->amb_light.skybox_t->img.width;
	i = 0;
	while (i < size)
	{
		scene->amb_light.gray_scale[i] = get_grey_value(scene, i);
		i++;
	}
	scene->amb_light.raw_average = ft_calloc(scene->amb_light.skybox_t->hdr.height, sizeof(double));
	if (scene->amb_light.raw_average == NULL)
		return ;
	i = 0;
	while (i < scene->amb_light.skybox_t->hdr.height)
	{	
		j = 0;
		sum = 0;
		while (j < scene->amb_light.skybox_t->hdr.width)
		{
			sum += scene->amb_light.gray_scale[i * scene->amb_light.skybox_t->hdr.width + j];
			j++;
		}
		scene->amb_light.raw_average[i] = sum / scene->amb_light.skybox_t->hdr.width;
		i++;
	}
	i = 0;
	sum = 0;
	while (i < scene->amb_light.skybox_t->hdr.height)
	{
		sum += scene->amb_light.raw_average[i++];
		
	}
	scene->amb_light.column_average = sum / scene->amb_light.skybox_t->hdr.height;
	scene->amb_light.pdf_joint = ft_calloc(scene->amb_light.skybox_t->hdr.width * scene->amb_light.skybox_t->hdr.height, sizeof(double));
	if (scene->amb_light.raw_average == NULL)
		return ;
		while (i < size)
	{
		scene->amb_light.pdf_joint[i] = scene->amb_light.gray_scale[i] / (scene->amb_light.column_average / 255);
		i++;
	}
	scene->amb_light.pdf_marginal = ft_calloc(scene->amb_light.skybox_t->hdr.height, sizeof(double));
	if (scene->amb_light.pdf_marginal == NULL)
		return ;
	i = 0;
	while (i < scene->amb_light.skybox_t->hdr.height)
	{
		scene->amb_light.pdf_marginal[i] = scene->amb_light.raw_average[i] / (scene->amb_light.column_average / 255);
		i++;
	}
	scene->amb_light.pdf_conditional = ft_calloc(scene->amb_light.skybox_t->hdr.width * scene->amb_light.skybox_t->hdr.height, sizeof(double));
	if (scene->amb_light.pdf_conditional == NULL)
		return ;
	i = 0;
	while (i < scene->amb_light.skybox_t->hdr.height)
	{	
		j = 0;
		while (j < scene->amb_light.skybox_t->hdr.width)
		{
			scene->amb_light.pdf_conditional[i * scene->amb_light.skybox_t->hdr.width + j] = scene->amb_light.gray_scale[i * scene->amb_light.skybox_t->hdr.width + j] / (scene->amb_light.raw_average[i] / 255);
			j++;
		}
		i++;
	}
	calc_cdf_marginal_inv(scene);
	calc_cdf_conditional_inv(scene);
}

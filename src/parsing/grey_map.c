/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grey_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:01:12 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/19 12:18:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	get_grey_value(t_scene *scene, size_t i)
{
	double scale;

	if (scene->amb_light.skybox_t->type == HDR) {
		if (scene->amb_light.skybox_t->hdr.exposure == 0)
			scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128);
		else
			scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128) * powf(2.0, scene->amb_light.skybox_t->hdr.exposure);
		
		return (
			0.2126 * (scene->amb_light.skybox_t->hdr.pixels[i].r * scale) +
			0.7152 * (scene->amb_light.skybox_t->hdr.pixels[i].g * scale) +
			0.0722 * (scene->amb_light.skybox_t->hdr.pixels[i].b * scale)
		);
	} else {
		return (
			0.2126 * (scene->amb_light.skybox_t->img.rgba[i].r / 255.0) +
			0.7152 * (scene->amb_light.skybox_t->img.rgba[i].g / 255.0) +
			0.0722 * (scene->amb_light.skybox_t->img.rgba[i].b / 255.0)
		);
	}
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
	
	scene->amb_light.cdf_conditional_inverse = ft_calloc(height * width, sizeof(double));
	
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

void make_importance_map(t_scene *scene)
{
	int width = scene->amb_light.skybox_t->hdr.width;
	int height = scene->amb_light.skybox_t->hdr.height;
	int size = width * height;

	// Étape 1 : Calculer la luminance (gray scale)
	scene->amb_light.gray_scale = ft_calloc(size, sizeof(double));
	double total_luminance = 0.0;

	for (int i = 0; i < size; i++) {
		scene->amb_light.gray_scale[i] = get_grey_value(scene, i);
		total_luminance += scene->amb_light.gray_scale[i];
	}

	// Étape 2 : Importance map = normalisation
	scene->amb_light.importance_map = ft_calloc(size, sizeof(double));
	for (int i = 0; i < size; i++) {
		scene->amb_light.importance_map[i] = scene->amb_light.gray_scale[i] / total_luminance;
	}

	// Étape 3 : PDF marginale (somme par ligne)
	scene->amb_light.pdf_marginal = ft_calloc(height, sizeof(double));
	for (int y = 0; y < height; y++) {
		double sum = 0;
		for (int x = 0; x < width; x++) {
			sum += scene->amb_light.importance_map[y * width + x];
		}
		scene->amb_light.pdf_marginal[y] = sum;
	}

	// Étape 4 : PDF conditionnelle (ligne normalisée)
	scene->amb_light.pdf_conditional = ft_calloc(size, sizeof(double));
	for (int y = 0; y < height; y++) {
		double marginal = scene->amb_light.pdf_marginal[y];
		for (int x = 0; x < width; x++) {
			int idx = y * width + x;
			scene->amb_light.pdf_conditional[idx] = (marginal > 0) ? (scene->amb_light.importance_map[idx] / marginal) : 0;
		}
	}

	// Étape 5 : construire les inverses de CDF
	calc_cdf_marginal_inv(scene);
	calc_cdf_conditional_inv(scene);
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
		scene->amb_light.pdf_joint[i] = scene->amb_light.gray_scale[i] / (scene->amb_light.column_average);
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

	scene->amb_light.DEBUG_INVERSE_SAMPLING = ft_calloc(scene->amb_light.skybox_t->hdr.height * scene->amb_light.skybox_t->hdr.width, sizeof(double));
	
	int z = 0;
	while (z < 100000)
	{
		t_vec2 aa = calc_inverse_transform_sampling_uv(scene);
		scene->amb_light.DEBUG_INVERSE_SAMPLING[(int)(aa.y * (scene->amb_light.skybox_t->hdr.height - 1) * scene->amb_light.skybox_t->hdr.width + aa.x * (scene->amb_light.skybox_t->hdr.width -1))] += 1;
		z++;
	}
	
}

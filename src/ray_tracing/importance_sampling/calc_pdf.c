/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_pdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:07:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/20 11:33:32 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	get_grey_value(t_scene *scene, size_t i)
{
	double scale;

	if (scene->amb_light.skybox_t->type == IMAGE)
		return (
			0.2126 * (scene->amb_light.skybox_t->img.rgba[i].r / 255.0) +
			0.7152 * (scene->amb_light.skybox_t->img.rgba[i].g / 255.0) +
			0.0722 * (scene->amb_light.skybox_t->img.rgba[i].b / 255.0)
		);
	if (scene->amb_light.skybox_t->hdr.exposure == 0)
		scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128);
	else
		scale = ldexp(1.0, scene->amb_light.skybox_t->hdr.pixels[i].e - 128)
			* powf(2.0, scene->amb_light.skybox_t->hdr.exposure);
	
	return (
		0.2126 * (scene->amb_light.skybox_t->hdr.pixels[i].r * scale) +
		0.7152 * (scene->amb_light.skybox_t->hdr.pixels[i].g * scale) +
		0.0722 * (scene->amb_light.skybox_t->hdr.pixels[i].b * scale)
	);
}

int	calc_gray_scale(t_scene *scene, int width, int height)
{
	size_t	size;
	size_t	i;

	scene->amb_light.gray_scale = ft_calloc(width * height, sizeof(double));
	if (scene->amb_light.gray_scale == NULL)
		return (0);
	size = height * width;
	i = 0;
	while (i < size)
	{
		scene->amb_light.gray_scale[i] = get_grey_value(scene, i);
		i++;
	}
	return (1);
}

int	calc_raw_average(t_scene *scene, int width, int height)
{
	size_t	i;
	size_t	j;
	double	sum;

	scene->amb_light.raw_average = ft_calloc(height, sizeof(double));
	if (scene->amb_light.raw_average == NULL)
		return (0);
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
	return (1);
}

int	calc_pdfs(t_scene *scene, int width, int height)
{
	scene->amb_light.pdf_joint = ft_calloc(width * height, sizeof(double));
	if (scene->amb_light.raw_average == NULL)
		return ;
		while (i < size)
	{
		scene->amb_light.pdf_joint[i] = scene->amb_light.gray_scale[i] / (scene->amb_light.column_average);
		i++;
	}
	scene->amb_light.pdf_marginal = ft_calloc(height, sizeof(double));
	if (scene->amb_light.pdf_marginal == NULL)
		return ;
	i = 0;
	while (i < height)
	{
		scene->amb_light.pdf_marginal[i] = scene->amb_light.raw_average[i] / (scene->amb_light.column_average / 255);
		i++;
	}
	scene->amb_light.pdf_conditional = ft_calloc(width * height, sizeof(double));
	if (scene->amb_light.pdf_conditional == NULL)
		return ;
	i = 0;
	while (i < height)
	{	
		j = 0;
		while (j < width)
		{
			scene->amb_light.pdf_conditional[i * width + j] = scene->amb_light.gray_scale[i * width + j] / (scene->amb_light.raw_average[i] / 255);
			j++;
		}
		i++;
	}
}

void	make_grey_map(t_scene *scene)
{
	size_t	i;
	size_t	j;
	double	sum;
	size_t	size;

	int	height;
	int width;

	// Secure 
	if (!(scene->amb_light.skybox_t && ((scene->amb_light.skybox_t->type == HDR && scene->amb_light.skybox_t->hdr.pixels)
		|| (scene->amb_light.skybox_t->type == IMAGE && scene->amb_light.skybox_t->img.rgba))))
		return ;
	if (scene->amb_light.skybox_t->type == HDR)
	{
		height = scene->amb_light.skybox_t->hdr.height;
		width = scene->amb_light.skybox_t->hdr.width;
	}
	else
	{
		height = scene->amb_light.skybox_t->img.height;
		width = scene->amb_light.skybox_t->img.width;
	}
	if (!calc_gray_scale(scene, width, height))
		return ;
	if (!calc_raw_average(scene, width, height))
	{
		free(scene->amb_light.gray_scale);
		scene->amb_light.gray_scale = NULL;
		return ;
	}
	i = 0;
	sum = 0;
	while (i < height)
		sum += scene->amb_light.raw_average[i++];
	scene->amb_light.column_average = sum / height;
	
	

	calc_cdf_marginal_inv(scene);
	calc_cdf_conditional_inv(scene);

	scene->amb_light.DEBUG_INVERSE_SAMPLING = ft_calloc(height * width, sizeof(double));
	
	int z = 0;
	while (z < 1000000)
	{
		t_vec2 aa = calc_inverse_transform_sampling_uv(scene);
		scene->amb_light.DEBUG_INVERSE_SAMPLING[(int)(aa.y * (height - 1) * width + aa.x * (width -1))] += 1;
		z++;
	}
	
}


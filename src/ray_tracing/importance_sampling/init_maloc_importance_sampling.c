/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_maloc_importance_sampling.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:57:51 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 13:14:42 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"

void	free_unused_malloc_importance_sampling(t_scene *scene)
{
	free(scene->amb_light.gray_scale);
	free(scene->amb_light.raw_average);
	free(scene->amb_light.pdf_marginal);
	free(scene->amb_light.pdf_conditional);
	free(scene->amb_light.cdf_h);
	free(scene->amb_light.cdf_w);
	scene->amb_light.gray_scale = NULL;
	scene->amb_light.raw_average = NULL;
	scene->amb_light.pdf_marginal = NULL;
	scene->amb_light.pdf_conditional = NULL;
	scene->amb_light.cdf_h = NULL;
	scene->amb_light.cdf_w = NULL;
}

static void	set_to_null_all_tab(t_scene *scene)
{
	scene->amb_light.gray_scale = NULL;
	scene->amb_light.raw_average = NULL;
	scene->amb_light.pdf_joint = NULL;
	scene->amb_light.pdf_marginal = NULL;
	scene->amb_light.pdf_conditional = NULL;
	scene->amb_light.cdf_conditional_inverse = NULL;
	scene->amb_light.cdf_marginal_inverse = NULL;
	scene->amb_light.cdf_h = NULL;
	scene->amb_light.cdf_w = NULL;
}

void	free_importance_sampling_malloc(t_scene *scene)
{
	free(scene->amb_light.gray_scale);
	free(scene->amb_light.raw_average);
	free(scene->amb_light.pdf_joint);
	free(scene->amb_light.pdf_marginal);
	free(scene->amb_light.pdf_conditional);
	free(scene->amb_light.cdf_conditional_inverse);
	free(scene->amb_light.cdf_marginal_inverse);
	free(scene->amb_light.cdf_h);
	free(scene->amb_light.cdf_w);
	set_to_null_all_tab(scene);
}

char	init_all_malloc_importance_sampling(t_scene *scene, int width,
	int height)
{
	set_to_null_all_tab(scene);
	scene->amb_light.gray_scale = ft_calloc(width * height, sizeof(double));
	scene->amb_light.raw_average = ft_calloc(height, sizeof(double));
	scene->amb_light.pdf_joint = ft_calloc(width * height, sizeof(double));
	scene->amb_light.pdf_marginal = ft_calloc(height, sizeof(double));
	scene->amb_light.pdf_conditional = ft_calloc(width * height,
			sizeof(double));
	scene->amb_light.cdf_conditional_inverse = ft_calloc(height * width,
			sizeof(double));
	scene->amb_light.cdf_marginal_inverse = ft_calloc(height, sizeof(double));
	scene->amb_light.cdf_h = ft_calloc(height, sizeof(double));
	scene->amb_light.cdf_w = ft_calloc(width, sizeof(double));
	if (!scene->amb_light.gray_scale || !scene->amb_light.raw_average
		|| !scene->amb_light.pdf_joint || !scene->amb_light.pdf_marginal
		|| !scene->amb_light.pdf_conditional || !scene->amb_light.cdf_h
		|| !scene->amb_light.cdf_conditional_inverse || !scene->amb_light.cdf_w
		|| !scene->amb_light.cdf_marginal_inverse)
	{
		free_importance_sampling_malloc(scene);
		print_error("malloc fail in importance sampling");
		return (1);
	}
	return (0);
}

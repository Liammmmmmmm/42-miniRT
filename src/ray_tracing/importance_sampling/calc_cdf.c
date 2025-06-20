/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_cdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:52:34 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 13:33:57 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"

static void	build_and_normalize_marginal_cdf(t_scene *scene, int height)
{
	double	sum;
	int		i;

	i = 0;
	sum = 0.0;
	while (i < height)
	{
		sum += scene->amb_light.pdf_marginal[i];
		scene->amb_light.cdf_h[i] = sum;
		i++;
	}
	if (sum > 0.0)
	{
		i = 0;
		while (i < height)
		{
			scene->amb_light.cdf_h[i] /= sum;
			i++;
		}
	}
}

void	calc_cdf_marginal_inv(t_scene *scene, int height)
{
	int		i;
	int		y;
	double	u;

	build_and_normalize_marginal_cdf(scene, height);
	i = 0;
	while (i < height)
	{
		u = (double)i / (height - 1);
		y = 0;
		while (y < height)
		{
			if (scene->amb_light.cdf_h[y] >= u)
			{
				scene->amb_light.cdf_marginal_inverse[i]
					= (double)y / (height - 1);
				break ;
			}
			y++;
		}
		i++;
	}
}

static void	set_cdf_conditional_row(t_scene *scene, int y, int width,
	double *row_cdf_buffer)
{
	int		i;
	int		x;
	double	u;

	i = -1;
	while (++i < width)
	{
		u = (double)i / (double)(width - 1);
		x = -1;
		while (++x < width)
		{
			if (row_cdf_buffer[x] >= u)
			{
				scene->amb_light.cdf_conditional_inverse[y * width + i]
					= (double)x / (double)(width - 1);
				break ;
			}
		}
	}
}

static void	process_conditional_row(t_scene *scene, int y, int width,
	double *row_cdf_buffer)
{
	double	sum;
	int		x;

	sum = 0.0;
	x = 0;
	while (x < width)
	{
		sum += scene->amb_light.pdf_conditional[y * width + x];
		row_cdf_buffer[x] = sum;
		x++;
	}
	if (sum > 0.0)
	{
		x = -1;
		while (++x < width)
			row_cdf_buffer[x] /= sum;
	}
	set_cdf_conditional_row(scene, y, width, row_cdf_buffer);
}

void	calc_cdf_conditional_inv(t_scene *scene, int width, int height)
{
	int	y;

	y = 0;
	while (y < height)
	{
		process_conditional_row(scene, y, width, scene->amb_light.cdf_w);
		y++;
	}
}

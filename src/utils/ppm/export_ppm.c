/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_ppm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:22:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/15 14:19:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ppm.h"
#include "minirt.h"

void export_ppm_p6(const char *filename, int *pixels, int width, int height)
{
	int	fd;
	int	i;
	int	tpx;
	char	tmp;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf("Error creating file\n");
		return ;
	}
	ft_dprintf(fd, "P6\n%d %d\n255\n", width, height);
	tpx = width * height;
	i = -1;
	while (++i < tpx)
	{
		tmp = (pixels[i] >> 16) & 0xFF;
		write(fd, &tmp, 1);
		tmp = (pixels[i] >> 8) & 0xFF;
		write(fd, &tmp, 1);
		tmp = pixels[i] & 0xFF;
		write(fd, &tmp, 1);
	}
	close(fd);
}

void export_ppm_p6_minirt(const char *filename, t_minirt *minirt)
{
	int		fd;
	int		i;
	int		tpx;
	char	tmp[3];
	int		divide;
	double	gamma_corr;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf("Error creating file\n");
		return ;
	}
	gamma_corr = 1.0 / (minirt->controls.values.gamma / 10.0);
	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	ft_dprintf(fd, "P6\n%d %d\n255\n", minirt->scene.render_width, minirt->scene.render_height);
	tpx = minirt->scene.render_width * minirt->scene.render_height;
	i = -1;
	while (++i < tpx)
	{
		tmp[0] = pow(clamp_double(minirt->screen.float_render[i].r / divide), gamma_corr) * 255;
		tmp[1] = pow(clamp_double(minirt->screen.float_render[i].g / divide), gamma_corr) * 255;
		tmp[2] = pow(clamp_double(minirt->screen.float_render[i].b / divide), gamma_corr) * 255;
		write(fd, &tmp, 3);
	}
	close(fd);
}

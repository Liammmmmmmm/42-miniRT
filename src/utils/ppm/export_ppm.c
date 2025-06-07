/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_ppm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:22:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/06 14:27:44 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ppm.h"
#include "minirt.h"

void	export_ppm_p6(const char *filename, int *pixels, int width, int height)
{
	int		fd;
	int		i;
	int		tpx;
	char	tmp[3];

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
		tmp[0] = (pixels[i] >> 16) & 0xFF;
		tmp[1] = (pixels[i] >> 8) & 0xFF;
		tmp[2] = pixels[i] & 0xFF;
		(void)!write(fd, tmp, 3);
	}
	close(fd);
}

static void	put_image_to_buf(int tpx, int fd, t_minirt *minirt,
	double gamma_corr)
{
	t_uchar	*buf;
	int		i;
	int		divide;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	buf = malloc(tpx * 3 * sizeof(t_uchar));
	if (!buf)
		return ;
	i = -1;
	while (++i < tpx)
	{
		buf[i * 3] = clamp_double(pow(minirt->screen.float_render[i].r / \
			divide, gamma_corr)) * 255;
		buf[i * 3 + 1] = clamp_double(pow(minirt->screen.float_render[i].g / \
			divide, gamma_corr)) * 255;
		buf[i * 3 + 2] = clamp_double(pow(minirt->screen.float_render[i].b / \
			divide, gamma_corr)) * 255;
	}
	(void)!write(fd, (char *)buf, tpx * 3);
}

void	export_ppm_p6_minirt(const char *filename, t_minirt *minirt)
{
	int		fd;
	int		tpx;
	double	gamma_corr;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_printf("Error creating file\n");
		return ;
	}
	gamma_corr = 1.0 / minirt->viewport.gamma;
	ft_dprintf(fd, "P6\n%d %d\n255\n", minirt->scene.render_width,
		minirt->scene.render_height);
	tpx = minirt->scene.render_width * minirt->scene.render_height;
	put_image_to_buf(tpx, fd, minirt, gamma_corr);
	close(fd);
}

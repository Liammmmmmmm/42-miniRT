/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_display.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:16:16 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 13:25:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*get_time_dhmsms(ssize_t time)
{
	t_uint	day;
	t_uint	hour;
	t_uint	min;
	t_uint	s;
	t_uint	ms;

	ms = time % 1000;
	s = time / 1000;
	min = s / 60;
	s = s % 60;
	hour = min / 60;
	min = min % 60;
	day = hour / 24;
	hour = hour % 24;
	if (day)
		return (ft_sprintf("%ud %uh %um %us %ums", day, hour, min, s, ms));
	if (hour)
		return (ft_sprintf("%uh %um %us %ums", hour, min, s, ms));
	if (min)
		return (ft_sprintf("%um %us %ums", min, s, ms));
	if (s)
		return (ft_sprintf("%us %ums", s, ms));
	return (ft_sprintf("%ums", ms));
}

static void	sample_infos(t_minirt *minirt)
{
	ssize_t	time;
	char	*tmp;
	char	*tmp2;
	char	*txt;

	time = get_cpu_time();
	tmp = get_time_dhmsms(time - minirt->screen.first_sample_time);
	tmp2 = get_time_dhmsms(((time - minirt->screen.first_sample_time)
				/ minirt->screen.sample)
			* (minirt->screen.spp - minirt->screen.sample));
	txt = ft_sprintf("Elapsed time %s\nAverage: %ums/sample\nEstimated left"
			" time: %s", tmp, (t_uint)((time
					- minirt->screen.first_sample_time)
				/ minirt->screen.sample), tmp2);
	free(tmp);
	free(tmp2);
	draw_string(&minirt->mlx.img, &minirt->controls.font[0], txt,
		(t_point2){20, 90});
	free(txt);
}

void	no_display_infos(t_minirt *minirt)
{
	char	*txt;

	setcolor(&minirt->mlx.img, 0x0);
	minirt->controls.font[0].size = 40;
	minirt->controls.font[0].color = 0xFFFFFF;
	txt = ft_sprintf("Sample %d/%d", minirt->screen.sample, minirt->screen.spp);
	draw_string(&minirt->mlx.img, &minirt->controls.font[0], txt,
		(t_point2){20, 50});
	free(txt);
	minirt->controls.font[0].size = 28;
	if (minirt->screen.sample)
		sample_infos(minirt);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
}

static void	sample_infos_anim(t_minirt *minirt)
{
	ssize_t	time;
	char	*tmp;
	char	*tmp2;
	char	*txt;

	time = get_cpu_time();
	tmp = get_time_dhmsms(time - minirt->screen.first_sample_time);
	tmp2 = get_time_dhmsms(((time - minirt->screen.first_sample_time)
				/ minirt->screen.sample_total_anim)
			* (minirt->screen.spp * minirt->options.anim.frames
				- minirt->screen.sample_total_anim));
	txt = ft_sprintf("Elapsed time %s\nAverage: %ums/sample\nEstimated left"
			" time: %s", tmp, (t_uint)((time
					- minirt->screen.first_sample_time)
				/ minirt->screen.sample_total_anim), tmp2);
	free(tmp);
	free(tmp2);
	draw_string(&minirt->mlx.img, &minirt->controls.font[0], txt,
		(t_point2){20, 90});
	free(txt);
}

void	no_display_infos_anim(t_minirt *minirt)
{
	char	*txt;

	setcolor(&minirt->mlx.img, 0x0);
	minirt->controls.font[0].size = 40;
	minirt->controls.font[0].color = 0xFFFFFF;
	txt = ft_sprintf("Frame %u/%u  Sample %d/%d",
			minirt->options.anim.frame_i + 1, minirt->options.anim.frames,
			minirt->screen.sample, minirt->screen.spp);
	draw_string(&minirt->mlx.img, &minirt->controls.font[0], txt,
		(t_point2){20, 50});
	free(txt);
	minirt->controls.font[0].size = 28;
	if (minirt->screen.sample_total_anim)
		sample_infos_anim(minirt);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
}

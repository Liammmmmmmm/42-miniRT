/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 10:33:49 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/21 18:29:28 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	render_float_to_int(t_fcolor *color, int divide)
{
	return (((int)(((int)(clamp_double(color->r / divide) * 255) << 16)
		+ ((int)(clamp_double(color->g / divide) * 255) << 8)
		+ (int)(clamp_double(color->b / divide) * 255))));
}

t_color	render_float_to_color(t_fcolor *color, int divide)
{
	return ((t_color){(int)(clamp_double(color->r / divide) * 255),
		(int)(clamp_double(color->g / divide) * 255),
		(int)(clamp_double(color->b / divide) * 255)});
}

int	render_float_to_int_gamma(t_fcolor *color, double gamma_corr, int divide)
{
	return ((int)(
		((int)(clamp_double(pow(color->r / divide, gamma_corr)) * 255) << 16)
		+ ((int)(clamp_double(pow(color->g / divide, gamma_corr)) * 255) << 8)
		+ (int)(clamp_double(pow(color->b / divide, gamma_corr)) * 255)));
}

void	copy_buff_to_image(t_minirt *minirt)
{
	int	i;
	int	tpx;

	if (minirt->options.no_display)
		return ;
	i = -1;
	tpx = minirt->scene.win_height * minirt->scene.win_width;
	while (++i < tpx)
		((int *)minirt->mlx.img.img_str)[i] = minirt->screen.render[i];
	if (minirt->controls.selected_x != -1)
	{
		put_pixel_image(&minirt->mlx.img, minirt->controls.selected_x + 1, minirt->controls.selected_y, 0xFF0000);
		put_pixel_image(&minirt->mlx.img, minirt->controls.selected_x - 1, minirt->controls.selected_y, 0xFF0000);
		put_pixel_image(&minirt->mlx.img, minirt->controls.selected_x, minirt->controls.selected_y - 1, 0xFF0000);
		put_pixel_image(&minirt->mlx.img, minirt->controls.selected_x, minirt->controls.selected_y + 1, 0xFF0000);
	}
}

void	init_upscale_struct(t_upscale_data *d, t_minirt *minirt)
{
	d->minirt = minirt;
	d->sw = minirt->viewport.render_w;
	d->sh = minirt->viewport.render_h;
	d->dw = minirt->scene.win_width;
	d->dh = minirt->scene.win_height;
	d->divide = minirt->screen.last_sample_am;
	if (d->divide == 0)
		d->divide = 1;
	d->scaleX = (float)(d->sw - 1) / (d->dw - 1);
	d->scaleY = (float)(d->sh - 1) / (d->dh - 1);
}

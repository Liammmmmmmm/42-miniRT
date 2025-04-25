/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/25 18:15:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"

void	put_ext_image_to_frame(t_tex_img *img, t_img *img_buff)
{
	int i;
	int total;
	t_sc_point	pt;
	const int red_ratio = 2;

	i = 0;
	total = img->height * img->width;
	while (i < total)
	{
		pt.x = (i % img->width) / red_ratio;
		pt.y = (img->height - i / img->width) / red_ratio;
		pt.color = img->pixel_data[i];
		put_sp_image(img_buff, &pt);
		i++;
	}
}

void	put_render_to_frame(t_minirt *minirt)
{
	int			i;
	int			tpix;
	t_sc_point	point;
	int			divide;

	divide = minirt->screen.last_sample_am;
	if (divide == 0)
		divide = 1;
	i = 0;
	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	point.z = 0;
	while (i < tpix)
	{
		point.x = minirt->screen.render[i].x;
		point.y = minirt->screen.render[i].y;
		point.color.r = minirt->screen.render[i].color.r / divide;
		point.color.g = minirt->screen.render[i].color.g / divide;
		point.color.b = minirt->screen.render[i].color.b / divide;
		put_sp_image(&minirt->mlx.img, &point);
		i++;
	}
}

void render_frame(t_minirt *minirt)
{
	render(minirt);
	render_bvh(minirt);

	render_ui(minirt);
  
	draw_selected_object(minirt);

	minirt->stats.frame += 1;
}

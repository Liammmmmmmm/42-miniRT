/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/26 19:55:17 by madelvin         ###   ########.fr       */
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
	if (minirt->controls.values.upscaling_ratio == 100)
		no_upscaling(minirt);
	if (minirt->controls.values.upscaling_selected == 0)
		neighbor_upscale(minirt);
	if (minirt->controls.values.upscaling_selected == 1)
		bilinear_upscale(minirt);
	if (minirt->controls.values.upscaling_selected == 2)
		bicubic_upscale(minirt);

}

void render_frame(t_minirt *minirt)
{
	render(minirt);
	render_bvh(minirt);
	render_ui(minirt);
	draw_selected_object(minirt);

	minirt->stats.frame += 1;
}

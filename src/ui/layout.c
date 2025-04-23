/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:53:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/23 11:58:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_vertical_line(t_img *img, uint32_t x, uint32_t y1, uint32_t y2)
{
	while (y1 < y2)
	{
		put_pixel_image(img, x, y1, UI_SEP_COLOR);
		y1++;
	}
}

void	draw_horizontal_line(t_img *img, uint32_t y, uint32_t x1, uint32_t x2)
{
	while (x1 < x2)
	{
		put_pixel_image(img, x1, y, UI_SEP_COLOR);
		x1++;
	}
}

void	draw_box_2d(t_img *img, t_point2 p1, t_point2 p2, int color)
{
	int	i;

	while (p1.y < p2.y)
	{
		i = p1.x - 1;
		while (++i < p2.x)
			put_pixel_image(img, i, p1.y, color);
		p1.y++;
	}
}

void	setcolor(t_img *img, uint32_t color)
{
	int i;
	int	total;

	total = img->width * img->height;
	i = 0;
	while (i < total)
	{
		((uint32_t *)img->img_str)[i] = color;
		i++;
	}
}

void	init_layout(t_img *img, t_minirt *minirt)
{
	setcolor(img, UI_BACK_COLOR);
	draw_vertical_line(img, 300, 0, img->height);
	draw_horizontal_line(img, 30, 300, img->width);

	minirt->controls.font[0].size = 30;
	minirt->controls.font[0].color = 0xFFFFFF;
	
	if (minirt->controls.ui_infos.tab_selected == 0)
	{
		draw_horizontal_line(img, 300, 300, img->width);
		draw_horizontal_line(img, 450, 300, img->width);
		// draw_string(img, &minirt->controls.font[0], "Liste objets", (t_point2){350, 170});
		draw_string(img, &minirt->controls.font[0], "Ajouter un objet", (t_point2){330, 380});

		draw_list_objects(minirt);
		
		if (minirt->controls.ui_infos.selected_object)
		{
			draw_string(img, &minirt->controls.font[0], "Props de l'objet", (t_point2){330, 750});
			display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
		}
	}
	else if (minirt->controls.ui_infos.tab_selected == 1)
	{
		draw_horizontal_line(img, 300, 300, img->width);
		draw_horizontal_line(img, 780, 300, img->width);
		draw_string(img, &minirt->controls.font[0], "Liste materiaux", (t_point2){330, 170});
		draw_string(img, &minirt->controls.font[0], "Preview mat\n(microRT)", (t_point2){370, 930});
		
		if (minirt->controls.ui_infos.selected_material)
		{
			draw_string(img, &minirt->controls.font[0], "Prop du materiau", (t_point2){320, 540});
		}
	}
	draw_box_2d(img, (t_point2){301, 0}, (t_point2){600, 30}, UI_BACK_COLOR);
}

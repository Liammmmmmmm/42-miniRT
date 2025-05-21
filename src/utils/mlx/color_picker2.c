/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_picker2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:29:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 16:34:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include "maths.h"

void	color_to_hex(char *dest, t_color color)
{
	dest[0] = '#';
	dest[1] = "0123456789ABCDEF"[color.r / 16];
	dest[2] = "0123456789ABCDEF"[color.r % 16];
	dest[3] = "0123456789ABCDEF"[color.g / 16];
	dest[4] = "0123456789ABCDEF"[color.g % 16];
	dest[5] = "0123456789ABCDEF"[color.b / 16];
	dest[6] = "0123456789ABCDEF"[color.b % 16];
	dest[7] = '\0';
}

/**
 * Why * 1.290322 ?
 * x * 1.290322 == x / 279 * 360, juste optimization
 */
void	draw_hue_colors(t_img *img, t_color_picker *cp)
{
	int	x;
	int	y;
	int	color;

	x = cp->btn.x;
	while (++x < cp->btn.x + 280)
	{
		y = cp->btn.y + 150;
		color = hsv_to_rgb((x - cp->btn.x - 1) * 1.290322, 1.0, 1.0);
		while (++y < cp->btn.y + 190)
			put_pixel_image(img, x, y, color);
	}
	draw_vertical_line(img, cp->btn.x + 279 * cp->hsv.hue / 360 + 1,
		cp->btn.y + 150, cp->btn.y + 190);
	draw_horizontal_line(img, cp->btn.y + 151, imax(cp->btn.x + 279 * cp->\
		hsv.hue / 360 - 1, cp->btn.x), cp->btn.x + 279 * cp->hsv.hue / 360 + 3);
	draw_horizontal_line(img, cp->btn.y + 152, cp->btn.x + 279 * cp->hsv.hue \
		/ 360, cp->btn.x + 279 * cp->hsv.hue / 360 + 2);
	draw_horizontal_line(img, cp->btn.y + 188, cp->btn.x + 279 * cp->hsv.hue / \
		360, cp->btn.x + 279 * cp->hsv.hue / 360 + 2);
	draw_horizontal_line(img, cp->btn.y + 189, imax(cp->btn.x + 279 * cp->\
		hsv.hue / 360 - 1, cp->btn.x), cp->btn.x + 279 * cp->hsv.hue / 360 + 3);
}

/**
 * x * 0.003584 = x / 279
 * x * 0.006711 = x / 149
 */
void	draw_sat_val_colors(t_img *img, t_color_picker *cp)
{
	int	x;
	int	y;

	x = cp->btn.x;
	while (++x < cp->btn.x + 280)
	{
		y = cp->btn.y;
		while (++y < cp->btn.y + 150)
			put_pixel_image(img, x, y, hsv_to_rgb(cp->hsv.hue,
					(x - cp->btn.x - 1) * 0.003584, 1 - ((y - cp->btn.y - 1)
						* 0.006711)));
	}
	circle_bres((t_circle){cp->btn.x + 1 + 279 * cp->hsv.sat, cp->btn.y + 150
		- 149 * cp->hsv.val, 3}, img, 0xFFFFFF);
}

void	display_color_picker(t_img *im, t_color_picker *cp, t_ttf *ttf)
{
	int	x;
	int	y;

	display_button(im, cp->btn, NULL);
	if (!cp->active)
		return ;
	draw_horizontal_line(im, cp->btn.y, cp->btn.x, cp->btn.x + 280);
	draw_horizontal_line(im, cp->btn.y + 150, cp->btn.x, cp->btn.x + 280);
	draw_vertical_line(im, cp->btn.x, cp->btn.y, cp->btn.y + 220);
	draw_vertical_line(im, cp->btn.x + 280, cp->btn.y, cp->btn.y + 221);
	draw_hue_colors(im, cp);
	draw_sat_val_colors(im, cp);
	ttf->size = 20;
	display_text_input(im, &cp->text_input, ttf);
	x = cp->btn.x + 240;
	while (++x < 280 + cp->btn.x)
	{
		y = cp->btn.y + 190;
		while (++y < 220 + cp->btn.y)
			put_pixel_image(im, x, y, cp->btn.background_color);
	}
	draw_vertical_line(im, cp->btn.x + 240, cp->btn.y + 190, cp->btn.y + 221);
	draw_horizontal_line(im, cp->btn.y + 190, cp->btn.x + 240, cp->btn.x + 280);
	draw_horizontal_line(im, cp->btn.y + 220, cp->btn.x + 240, cp->btn.x + 280);
}

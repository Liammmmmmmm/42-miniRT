/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_dropdown.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 09:56:17 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/04 11:33:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "ui.h"

static void	draw_dropdown_select_box_def(t_img *img, t_dropdown *d, \
	t_minirt *m)
{
	int	i;
	int	i1;
	int	i2;
	int	tmp;
	t_point2	p;

	draw_dropdown_select_box(img, d);
	i = -1;
	while (++i < d->val_amount)
	{
		if (dropdown_common_define(d, &i1, &i2, i - 1) == 0)
			return ;
		tmp = img->height;
		img->height = d->y + d->deployed_height;
		draw_horizontal_line(img, i2, d->x, d->x + d->width);
		p = (t_point2){d->x + d->width, i2};
		if (d->selected && d->values[i].ref == *d->selected)
			draw_box_2d(img, (t_point2){d->x + 1, i1}, p, 0x353535);
		else
			draw_box_2d(img, (t_point2){d->x + 1, i1}, p, 0x0);
		p = (t_point2){d->x + 3, i2 - 13
			+ ((get_height(&m->controls.font[0])) * 0.5)};
		draw_string(img, &m->controls.font[0], d->values[i].text, p);
		img->height = tmp;
	}
}

const char	*get_selected_text(t_dropdown *dropdown)
{
	int i = 0;
	
	while (i < dropdown->val_amount)
	{
		if (dropdown->values[i].ref == *dropdown->selected)
			return (dropdown->values[i].text);
		i++;
	}
	return (NULL);
}

void	display_dropdown(t_minirt *minirt, t_dropdown *dropdown)
{
	minirt->controls.font[0].size = 20;
	minirt->controls.font[0].color = 0xFFFFFF;
	if (dropdown->active)
		draw_dropdown_select_box_def(&minirt->mlx.img_controls, dropdown, minirt);
	draw_main_box(&minirt->mlx.img_controls, dropdown);
	if (!dropdown->selected || !*dropdown->selected)
		draw_string(&minirt->mlx.img_controls, &minirt->controls.font[0],
			"None", (t_point2){dropdown->x + 3, dropdown->y
			+ dropdown->height - 3});
	else
		draw_string(&minirt->mlx.img_controls, &minirt->controls.font[0],
			get_selected_text(dropdown), (t_point2){dropdown->x + 3,
				dropdown->y + dropdown->height - 3});
}

static void	get_selected_dropdown(t_dropdown *dropdown, \
	int mouse_y)
{
	int	i;
	int	i1;
	int	i2;

	dropdown->active = 0;
	if (!dropdown->selected || !dropdown->values)
		return ;
	i = -1;
	while (++i < dropdown->val_amount)
	{
		i1 = dropdown->y + dropdown->height + 1 + i * 25
			- dropdown->scroll_offset;
		i2 = dropdown->y + dropdown->height + 25 + i * 25
			- dropdown->scroll_offset;
		if (mouse_y > i1 && mouse_y < i2)
		{
			*dropdown->selected = dropdown->values[i].ref;
			return ;
		}
	}
}

int	mouse_down_dropdown(int key, t_point2 pos, t_dropdown *d)
{
	const int	max_scroll = imax(25 * (d->val_amount)
			- (d->deployed_height - d->height), 0);

	if (toggle_dropdown(key, pos, d))
		return (1);
	if (!d->active)
		return (0);
	if (pos.x > d->x && pos.y > d->y + d->height && pos.x < d->x + d->width
		&& pos.y < d->y + d->deployed_height)
	{
		if (key == SCROLL_DOWN)
			d->scroll_offset = imin(d->scroll_offset + 15, max_scroll);
		else if (key == SCROLL_UP)
			d->scroll_offset = imax(d->scroll_offset - 15, 0);
		else if (key == LEFT_CLICK)
			get_selected_dropdown(d, pos.y);
	}
	else if (key == LEFT_CLICK)
		d->active = 0;
	return (1);
}

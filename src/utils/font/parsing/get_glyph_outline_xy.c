/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_outline_xy.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:08:19 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 13:12:59 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static int	get_curr_co(t_bin *bin, size_t *i, int16_t *cur, uint8_t flag)
{
	uint8_t	cur8;

	if (flag == 0)
	{
		if (read_int16_move(bin, i, cur) == -1)
			return (-1);
	}
	else if (flag == 1)
		*cur = 0;
	else if (flag == 2)
	{
		if (read_uint8_move(bin, i, &cur8) == -1)
			return (-1);
		*cur = -cur8;
	}
	else if (flag == 3)
	{
		if (read_uint8_move(bin, i, &cur8) == -1)
			return (-1);
		*cur = cur8;
	}
	return (0);
}

int	init_co_x(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index)
{
	int		y;
	int16_t	prev_coordinate;
	int16_t	current_coordinate;
	uint8_t	flag_combined;

	y = 0;
	prev_coordinate = 0;
	while (y < last_index + 1)
	{
		flag_combined = o->flags[y].x_short << 1 | o->flags[y].x_short_pos;
		if (get_curr_co(bin, i, &current_coordinate, flag_combined) == -1)
			return (-1);
		o->x_coordinates[y] = current_coordinate + prev_coordinate;
		prev_coordinate = o->x_coordinates[y];
		y++;
	}
	return (0);
}

int	init_co_y(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index)
{
	int		y;
	int16_t	prev_coordinate;
	int16_t	current_coordinate;
	uint8_t	flag_combined;

	y = 0;
	prev_coordinate = 0;
	while (y < last_index + 1)
	{
		flag_combined = o->flags[y].y_short << 1 | o->flags[y].y_short_pos;
		if (get_curr_co(bin, i, &current_coordinate, flag_combined) == -1)
			return (-1);
		o->y_coordinates[y] = current_coordinate + prev_coordinate;
		prev_coordinate = o->y_coordinates[y];
		y++;
	}
	return (0);
}

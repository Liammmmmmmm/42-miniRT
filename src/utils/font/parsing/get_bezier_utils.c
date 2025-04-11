/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bezier_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:09:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 13:09:46 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static void	count_basics_lines(int n_contours, t_glyph_outline *o, int pts_am,
	int *res)
{
	int	i;

	i = -1;
	while (++i < pts_am - 1)
	{
		if (i == o->end_pts_of_contours[n_contours]
			&& n_contours < o->number_of_contours)
		{
			n_contours++;
			continue ;
		}
		if (!o->flags[i + 1].on_curve)
		{
			if (!(get_tmp_next(i, o, n_contours) < pts_am))
				continue ;
		}
		else if (!(o->flags[i].on_curve && o->flags[i + 1].on_curve))
			continue ;
		(*res)++;
	}
}

int	count_bezier_lines(t_glyph_outline *o, int pts_am)
{
	int	res;
	int	n_contours;
	int	tmp;

	res = 0;
	count_basics_lines(0, o, pts_am, &res);
	n_contours = -1;
	while (++n_contours < o->number_of_contours)
	{
		if (n_contours < 1)
			tmp = 0;
		else
			tmp = o->end_pts_of_contours[n_contours - 1] + 1;
		if (!o->flags[o->end_pts_of_contours[n_contours]].on_curve
			&& o->flags[tmp].on_curve)
			continue ;
		res++;
	}
	return (res);
}

static int32_t	x_average(t_glyph_outline *o, int i1, int i2)
{
	return ((o->x_coordinates[i1] + o->x_coordinates[i2]) / 2);
}

static int32_t	y_average(t_glyph_outline *o, int i1, int i2)
{
	return ((o->y_coordinates[i1] + o->y_coordinates[i2]) / 2);
}

t_point2	p2co(t_glyph_outline *o, int i1, int i2)
{
	if (i2 == -1)
		return ((t_point2){.x = o->x_coordinates[i1], .y = o->y_coordinates[i1]}
		);
	else
		return ((t_point2){.x = x_average(o, i1, i2), .y = y_average(o, i1, i2)}
		);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bezier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:37:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 13:08:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	get_tmp_next(int i, t_glyph_outline *o, int n_contours)
{
	if (i + 1 == o->end_pts_of_contours[n_contours] && n_contours > 0)
		return (o->end_pts_of_contours[n_contours - 1] + 1);
	else if (i + 1 == o->end_pts_of_contours[n_contours] && n_contours == 0)
		return (0);
	else
		return (i + 2);
}

static void	add_contours_lines(t_glyph_outline *o, int y, int n)
{
	int	tmp;

	while (++n < o->number_of_contours && y < o->bezier_amount)
	{
		tmp = 0;
		if (n >= 1)
			tmp = o->end_pts_of_contours[n - 1] + 1;
		if (!o->flags[o->end_pts_of_contours[n]].on_curve
			&& o->flags[tmp].on_curve)
			continue ;
		if (o->flags[o->end_pts_of_contours[n]].on_curve)
			o->bezier_lines[y].p1 = p2co(o, o->end_pts_of_contours[n], -1);
		else
			o->bezier_lines[y].p1 = p2co(o, o->end_pts_of_contours[n], tmp);
		if (o->flags[tmp].on_curve)
			o->bezier_lines[y].p2 = p2co(o, tmp, -1);
		else
		{
			o->bezier_lines[y].p2 = p2co(o, tmp, tmp + 1);
			o->bezier_lines[y].pc = p2co(o, tmp, -1);
			o->bezier_lines[y].have_control = 1;
		}
		y++;
	}
}

static void	add_normal_line(t_glyph_outline *o, int *y, int i, int n_contours)
{
	int	tmp;

	if (o->flags[i].on_curve && o->flags[i + 1].on_curve)
	{
		o->bezier_lines[*y].p1 = p2co(o, i, -1);
		o->bezier_lines[*y].p2 = p2co(o, i + 1, -1);
		(*y)++;
	}
	else if (!o->flags[i + 1].on_curve)
	{
		tmp = get_tmp_next(i, o, n_contours);
		if (!(tmp < o->end_pts_of_contours[o->number_of_contours - 1] + 1))
			return ;
		o->bezier_lines[*y].pc = p2co(o, i + 1, -1);
		o->bezier_lines[*y].have_control = 1;
		if (o->flags[i].on_curve)
			o->bezier_lines[*y].p1 = p2co(o, i, -1);
		else
			o->bezier_lines[*y].p1 = p2co(o, i, i + 1);
		if (o->flags[tmp].on_curve)
			o->bezier_lines[*y].p2 = p2co(o, tmp, -1);
		else
			o->bezier_lines[*y].p2 = p2co(o, tmp, i + 1);
		(*y)++;
	}
}

void	save_bezier_lines(t_glyph_outline *o, int pts_am)
{
	int	y;
	int	i;
	int	n_contours;

	i = -1;
	y = 0;
	n_contours = 0;
	while (++i < pts_am - 1 && y < o->bezier_amount)
	{
		if (i == o->end_pts_of_contours[n_contours]
			&& n_contours < o->number_of_contours)
		{
			n_contours++;
			continue ;
		}
		add_normal_line(o, &y, i, n_contours);
	}
	add_contours_lines(o, y, -1);
}

int	get_bezier(t_glyph_outline *o, int pts_am)
{
	o->bezier_amount = count_bezier_lines(o, pts_am);
	if (o->bezier_amount == -1)
		return (-2);
	o->bezier_lines = ft_calloc(o->bezier_amount, sizeof(t_bezier));
	if (!o->bezier_lines)
		return (-1);
	save_bezier_lines(o, pts_am);
	return (0);
}

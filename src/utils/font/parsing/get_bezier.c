/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_bezier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:37:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 11:39:37 by lilefebv         ###   ########lyon.fr   */
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

int	count_bezier_lines(t_glyph_outline *o, int pts_am)
{
	int	res;
	int i;
	int	n_contours;
	int	tmp;

	res = 0;
	i = -1;
	n_contours = 0;
	while (++i < pts_am - 1)
	{
		if (i == o->end_pts_of_contours[n_contours] && n_contours < o->number_of_contours)
		{
			n_contours++;
			continue ;
		}
		if (o->flags[i].on_curve && !o->flags[i + 1].on_curve)
		{
			if (!(get_tmp_next(i, o, n_contours) < pts_am))
				continue ;
		}
		else if (!o->flags[i].on_curve && !o->flags[i + 1].on_curve)
		{
			if (!(get_tmp_next(i, o, n_contours) < pts_am))
				continue ;
		}
		else if (!(o->flags[i].on_curve && o->flags[i + 1].on_curve))
			continue ;
		res++;
	}
	n_contours = -1;
	while (++n_contours < o->number_of_contours)
	{
		if (n_contours < 1)
			tmp = 0;
		else
			tmp = o->end_pts_of_contours[n_contours - 1] + 1;
		if (!o->flags[o->end_pts_of_contours[n_contours]].on_curve && o->flags[tmp].on_curve)
			continue;
		res++;
	}
	return (res);
}

void	save_bezier_lines(t_glyph_outline *o, int pts_am)
{
	int y;
	int i;
	int	n_contours;
	int	tmp;

	i = 0;
	y = 0;
	n_contours = 0;
	while (i < pts_am - 1 && y < o->bezier_amount)
	{
		if (i == o->end_pts_of_contours[n_contours] && n_contours < o->number_of_contours)
		{
			n_contours++;
			i++;
			continue ;
		}
		if (o->flags[i].on_curve && o->flags[i + 1].on_curve)
		{
			o->bezier_lines[y].p1 = (t_point2){.x = o->x_coordinates[i], .y = o->y_coordinates[i]};
			o->bezier_lines[y].p2 = (t_point2){.x = o->x_coordinates[i + 1], .y = o->y_coordinates[i + 1]};
			y++;
		}
		else if (o->flags[i].on_curve && !o->flags[i + 1].on_curve)
		{
			tmp = get_tmp_next(i, o, n_contours);
			if (!(tmp < pts_am))
			{
				i++;
				continue ;
			}
			o->bezier_lines[y].p1 = (t_point2){.x = o->x_coordinates[i], .y = o->y_coordinates[i]};
			o->bezier_lines[y].pc = (t_point2){.x = o->x_coordinates[i + 1], .y = o->y_coordinates[i + 1]};
			o->bezier_lines[y].have_control = 1;
			if (o->flags[tmp].on_curve)
				o->bezier_lines[y].p2 = (t_point2){.x = o->x_coordinates[tmp], .y = o->y_coordinates[tmp]};
			else
				o->bezier_lines[y].p2 = (t_point2){.x = (o->x_coordinates[i + 1] + o->x_coordinates[tmp]) / 2, (o->y_coordinates[i + 1] + o->y_coordinates[tmp]) / 2};
			y++;
		}
		else if (!o->flags[i].on_curve && !o->flags[i + 1].on_curve)
		{
			tmp = get_tmp_next(i, o, n_contours);
			if (!(tmp < pts_am))
			{
				i++;
				continue ;
			}
			o->bezier_lines[y].have_control = 1;
			o->bezier_lines[y].pc = (t_point2){.x = o->x_coordinates[i + 1], .y = o->y_coordinates[i + 1]};
			if (o->flags[tmp].on_curve)
				o->bezier_lines[y].p2 = (t_point2){.x = o->x_coordinates[tmp], .y = o->y_coordinates[tmp]};
			else
				o->bezier_lines[y].p2 = (t_point2){.x = (o->x_coordinates[i + 1] + o->x_coordinates[tmp]) / 2, (o->y_coordinates[i + 1] + o->y_coordinates[tmp]) / 2};		
			o->bezier_lines[y].p1 = (t_point2){.x = (o->x_coordinates[i] + o->x_coordinates[i + 1]) / 2, (o->y_coordinates[i] + o->y_coordinates[i + 1]) / 2};
			y++;
		}
		i++;
	}
	n_contours = 0;
	while (n_contours < o->number_of_contours && y < o->bezier_amount)
	{
		if (n_contours < 1)
			tmp = 0;
		else
			tmp = o->end_pts_of_contours[n_contours - 1] + 1;
		if (!o->flags[o->end_pts_of_contours[n_contours]].on_curve && o->flags[tmp].on_curve)
		{
			n_contours++;
			continue;
		}
		if (o->flags[o->end_pts_of_contours[n_contours]].on_curve)
			o->bezier_lines[y].p1 = (t_point2){.x = o->x_coordinates[o->end_pts_of_contours[n_contours]], .y = o->y_coordinates[o->end_pts_of_contours[n_contours]]};
		else
			o->bezier_lines[y].p1 = (t_point2){.x = (o->x_coordinates[o->end_pts_of_contours[n_contours]] + o->x_coordinates[tmp]) / 2, .y = (o->y_coordinates[o->end_pts_of_contours[n_contours]] + o->y_coordinates[tmp]) / 2};
		if (o->flags[tmp].on_curve)
			o->bezier_lines[y].p2 = (t_point2){.x = o->x_coordinates[tmp], .y = o->y_coordinates[tmp]};
		else
		{
			o->bezier_lines[y].p2 = (t_point2){.x = (o->x_coordinates[tmp] + o->x_coordinates[tmp + 1]) / 2, .y = (o->y_coordinates[tmp] + o->y_coordinates[tmp + 1]) / 2};
			o->bezier_lines[y].pc = (t_point2){.x = o->x_coordinates[tmp], .y = o->y_coordinates[tmp]};
			o->bezier_lines[y].have_control = 1;
		}
		
		n_contours++;
		y++;
	}
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

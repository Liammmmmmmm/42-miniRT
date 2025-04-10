/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_bezier_res.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:48:45 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 15:44:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static void	count_segments(t_ttf *ttf, t_uchar c)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (i < ttf->glyph256[c].bezier_amount)
	{
		if (ttf->glyph256[c].bezier_lines[i].have_control == 0)
			res++;
		else
			res += ttf->bezier_resolution;
		i++;
	}
	ttf->glyph256[c].segments_amount = res;
}

static void	get_segments_char(t_ttf *ttf, t_uchar c)
{
	const int	lsb = ttf->glyph256[c].left_side_bearing - ttf->glyph256[c].xmin
		;
	int			i;
	uint32_t	y;
	int			n;

	i = 0;
	y = 0;
	count_segments(ttf, c);
	if (ttf->glyph256[c].segments_amount == 0)
		return ;
	free(ttf->glyph256[c].segments);
	ttf->glyph256[c].segments = ft_calloc(ttf->glyph256[c].segments_amount, sizeof(t_segment));
	if (!ttf->glyph256[c].segments)
		return ;
	ttf->glyph256[c].xmin += lsb;
	ttf->glyph256[c].xmax += lsb;
	while (i < ttf->glyph256[c].bezier_amount && y < ttf->glyph256[c].segments_amount)
	{
		if (ttf->glyph256[c].bezier_lines[i].have_control == 0)
		{
			ttf->glyph256[c].segments[y].p1.x = ttf->glyph256[c].bezier_lines[i].p1.x + lsb;
			ttf->glyph256[c].segments[y].p1.y = ttf->glyph256[c].bezier_lines[i].p1.y;
			ttf->glyph256[c].segments[y].p2.x = ttf->glyph256[c].bezier_lines[i].p2.x + lsb;
			ttf->glyph256[c].segments[y].p2.y = ttf->glyph256[c].bezier_lines[i].p2.y;
			y++;
		}
		else
		{
			n = -1;
			tessellate_bezier(ttf->r_data.points_buff, ttf->bezier_resolution, &ttf->glyph256[c].bezier_lines[i]);
			while (++n < ttf->bezier_resolution)
			{
				ttf->glyph256[c].segments[y].p1.x = (ttf->r_data.points_buff[n].x + lsb);
				ttf->glyph256[c].segments[y].p1.y = ttf->r_data.points_buff[n].y;
				ttf->glyph256[c].segments[y].p2.x = (ttf->r_data.points_buff[n + 1].x + lsb);
				ttf->glyph256[c].segments[y].p2.y = ttf->r_data.points_buff[n + 1].y;
				y++;
			}
		}
		i++;
	}
}

int	set_bezier_res(t_ttf *ttf, uint8_t bezier_res)
{
	int	i;

	ttf->bezier_resolution = bezier_res;
	ttf->r_data.points_buff = ft_calloc(ttf->bezier_resolution + 1,
		sizeof(t_point2));
	if (!ttf->r_data.points_buff)
		return (-1);
	i = 0;
	while (i < 256)
	{
		get_segments_char(ttf, i);
		i++;
	}	
	free(ttf->r_data.points_buff);
	return (0);
}
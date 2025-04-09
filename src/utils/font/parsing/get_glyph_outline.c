/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_outline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:33:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 16:20:02 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static int	init_base(t_bin *bin, size_t *i, t_glyph_outline *o)
{
	size_t	y;

	if (read_int16_move(bin, i, &o->number_of_contours) == -1
		|| read_int16_move(bin, i, &o->xmin) == -1
		|| read_int16_move(bin, i, &o->ymin) == -1
		|| read_int16_move(bin, i, &o->xmax) == -1
		|| read_int16_move(bin, i, &o->ymax) == -1)
		return (-1);
	if (o->number_of_contours < 1)
		return (-2);
	o->end_pts_of_contours = ft_calloc(o->number_of_contours, sizeof(int16_t));
	if (!o->end_pts_of_contours)
		return (-1);
	y = 0;
	while (y < (size_t)o->number_of_contours)
	{
		if (read_uint16_move(bin, i, &o->end_pts_of_contours[y]) == -1)
		{
			free(o->end_pts_of_contours);
			return (-1);
		}
		y++;
	}
	return (0);
}

static int	init_instuct_and_mem(t_bin *bin, size_t *i, t_glyph_outline *o,
	int last_index)
{
	if (read_uint16_move(bin, i, &o->instruction_length) == -1)
		return (-1);
	o->instructions = malloc(o->instruction_length * sizeof(uint8_t));
	if (!o->instructions)
		return (-1);
	ft_memcpy(o->instructions, bin->data + *i, o->instruction_length);
	*i += o->instruction_length;
	o->flags = ft_calloc(last_index + 1, sizeof(uint8_t));
	o->x_coordinates = ft_calloc(last_index + 1, sizeof(uint16_t));
	o->y_coordinates = ft_calloc(last_index + 1, sizeof(uint16_t));
	if (!o->flags || !o->x_coordinates || !o->y_coordinates)
	{
		free(o->flags);
		free(o->x_coordinates);
		free(o->y_coordinates);
		free(o->instructions);
		return (-1);
	}
	return (0);
}

static int	init_flags(t_bin *bin, size_t *i, t_glyph_outline *o, int last_index
	)
{
	int		y;
	uint8_t	repeat_count;

	y = 0;
	while (y < last_index + 1)
	{
		if (read_uint8_move(bin, i, &o->flags[y].flag) == -1)
			return (-1);
		if (o->flags[y].repeat)
		{
			if (read_uint8_move(bin, i, &repeat_count) == -1)
				return (-1);
			while (repeat_count-- > 0 && y + 1 < last_index + 1)
			{
				y++;
				o->flags[y] = o->flags[y - 1];
			}
		}
		y++;
	}
	return (0);
}

int	get_glyph_outline(t_bin *bin, t_ttf *ttf, uint32_t glyph_index,
	t_glyph_outline *outline)
{
	size_t	i;
	int		last_index;

	i = get_glyph_offset(bin, ttf, glyph_index) + ttf->r_data.glyf_offset;
	if (init_base(bin, &i, outline) == -1)
		return (-1);
	if (outline->number_of_contours < 1)
		return (-2);
	last_index = outline->end_pts_of_contours[outline->number_of_contours - 1];
	if (init_instuct_and_mem(bin, &i, outline, last_index) == -1)
	{
		free(outline->end_pts_of_contours);
		return (-1);
	}
	if (init_flags(bin, &i, outline, last_index) == -1)
		return (free_glyph(outline));
	if (init_co_x(bin, &i, outline, last_index) == -1)
		return (free_glyph(outline));
	if (init_co_y(bin, &i, outline, last_index) == -1)
		return (free_glyph(outline));
	if (get_bezier(outline, last_index + 1) == -1)
		return (free_glyph(outline));
	return (0);
}

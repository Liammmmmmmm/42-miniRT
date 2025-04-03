/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_glyph_outline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:33:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/03 17:13:23 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static int	init_base(t_bin *bin, size_t *i, t_glyph_outline *o)
{
	size_t	y;

	if (read_int16(bin, i, &o->number_of_contours) == -1
		|| read_int16(bin, i, &o->xmin) == -1
		|| read_int16(bin, i, &o->ymin) == -1
		|| read_int16(bin, i, &o->xmax) == -1
		|| read_int16(bin, i, &o->ymax) == -1)
		return (-1);
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

static int	init_inst_and_flags(t_bin *bin, size_t *i, t_glyph_outline *o)
{
	if (read_int16(bin, i, &o->instruction_length) == -1)
		return (-1);
	o->instructions = malloc(o->instruction_length * sizeof(uint8_t));
	if (!o->instructions)
		return (-1);
	ft_memcpy(o->instructions, bin->data + *i, o->instruction_length);
	*i += o->instruction_length;
	
}

int	get_glyph_outline(t_bin *bin, t_ttf* ttf, uint32_t glyph_index,
	t_glyph_outline *outline)
{
	size_t	i;
	int		last_index;

	i = get_glyph_offset(bin, ttf, glyph_index) + ttf->r_data.glyf_offset;
	if (init_base(bin, &i, outline) == -1)
		return (-1);
	last_index = outline->end_pts_of_contours[outline->number_of_contours - 1];
	if (init_inst_and_flags(bin, &i, outline) == -1)
	{
		free(outline->end_pts_of_contours);
		return (-1);
	}
	
	return (0);
}
typedef struct s_glyph_outline
{

	uint16_t		instruction_length;
	uint8_t			*instructions;
	t_glyph_flag	*flags;
	int16_t			*x_coordinates;
	int16_t			*y_coordinates;
	uint16_t		*end_pts_of_contours;
}	t_glyph_utline;

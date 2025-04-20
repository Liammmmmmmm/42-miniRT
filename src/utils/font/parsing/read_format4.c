/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_format4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:37:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/20 15:56:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	init_struct_basics(t_bin *bin, size_t *i, t_format4 *f, t_uint length)
{
	int	is_valid;

	is_valid = 0;
	is_valid += read_uint16_move(bin, i, &f->format);
	is_valid += read_uint16_move(bin, i, &f->length);
	is_valid += read_uint16_move(bin, i, &f->language);
	is_valid += read_uint16_move(bin, i, &f->seg_count_x2);
	is_valid += read_uint16_move(bin, i, &f->search_range);
	is_valid += read_uint16_move(bin, i, &f->entry_selector);
	is_valid += read_uint16_move(bin, i, &f->range_shift);
	if (is_valid != 0 || f->seg_count_x2 % 2 != 0
		|| length - sizeof(uint16_t) * 8 <= f->seg_count_x2 * 4)
		return (-1);
	f->end_code = (uint16_t *)((uint8_t *)f + sizeof(t_format4));
	f->start_code = f->end_code + (f->seg_count_x2 >> 1);
	f->id_delta = f->start_code + (f->seg_count_x2 >> 1);
	f->id_range_offset = f->id_delta + (f->seg_count_x2 >> 1);
	f->glyph_id_array = f->id_range_offset + (f->seg_count_x2 >> 1);
	return (0);
}

int	init_arrays(t_bin *bin, size_t *i, t_format4 *f)
{
	const size_t	start_code_start = *i + f->seg_count_x2 + sizeof(uint16_t);
	const size_t	id_delta_start = *i + f->seg_count_x2 * 2 + sizeof(uint16_t)
		;
	const size_t	id_range_start = *i + f->seg_count_x2 * 3 + sizeof(uint16_t)
		;
	uint16_t		y;

	y = 0;
	while (y < f->seg_count_x2 * 0.5)
	{
		if (read_uint16(bin, *i + y * 2, f->end_code + y) == -1 \
		|| read_uint16(bin, start_code_start + y * 2, f->start_code + y) == -1
			|| read_uint16(bin, id_delta_start + y * 2, f->id_delta + y) == -1 \
	|| read_uint16(bin, id_range_start + y * 2, f->id_range_offset + y) == -1)
			return (-1);
		y++;
	}
	*i += f->seg_count_x2 * 4 + sizeof(uint16_t);
	return (0);
}

int	init_remaining_bytes(t_bin *bin, size_t *i, t_format4 *f, t_ttf *ttf)
{
	const int	remaining_bytes = f->length - (*i - (ttf->r_data.cmap_offset + \
		ttf->r_data.uni_f4_offset));
	int			y;

	y = 0;
	while (y < remaining_bytes * 0.5)
	{
		if (read_uint16_move(bin, i, &f->glyph_id_array[y]) == -1)
			return (-1);
		y++;
	}
	return (0);
}

int	read_format4(t_bin *bin, t_ttf *ttf)
{
	uint16_t	length;
	t_format4	*f;
	size_t		i;

	length = 0;
	i = ttf->r_data.cmap_offset + ttf->r_data.uni_f4_offset;
	if (read_uint16(bin, i + 2, &length) == -1)
		return (-1);
	if (length % 2 != 0 || length < sizeof(t_format4))
		return (-1);
	f = ft_calloc(1, length + sizeof(t_format4) - sizeof(uint16_t) * 8);
	if (!f)
		return (-1);
	if (init_struct_basics(bin, &i, f, length) == -1)
		return (-1);
	if (read_uint16(bin, i + f->seg_count_x2, &f->reserved_pad) == -1
		|| f->reserved_pad != 0)
		return (-1);
	if (init_arrays(bin, &i, f) == -1)
		return (-1);
	if (init_remaining_bytes(bin, &i, f, ttf) == -1)
		return (-1);
	ttf->cmap.format4 = f;
	return (0);
}

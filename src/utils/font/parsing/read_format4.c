/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_format4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:37:33 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/02 18:10:45 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static int	free_format_ret(t_format4 *f)
{
	free(f);
	return (-1);
}

int	init_struct_basics(t_bin *bin, size_t *i, t_format4 *f, t_uint length)
{
	int	is_valid;

	is_valid = 0;
	is_valid += read_uint16_move(bin, &i, &f->format);
	is_valid += read_uint16_move(bin, &i, &f->length);
	is_valid += read_uint16_move(bin, &i, &f->language);
	is_valid += read_uint16_move(bin, &i, &f->seg_count_x2);
	is_valid += read_uint16_move(bin, &i, &f->search_range);
	is_valid += read_uint16_move(bin, &i, &f->entry_selector);
	is_valid += read_uint16_move(bin, &i, &f->range_shift);
	if (is_valid != 0 || f->seg_count_x2 % 2 != 0
		|| length - sizeof(uint16_t) * 8 <= f->seg_count_x2 * 4)
		return (-1);
	f->end_code = (uint16_t *)((uint8_t *)f + sizeof(t_format4));
	f->start_code = f->end_code + f->seg_count_x2 / 2;
	f->id_delta = f->start_code + f->seg_count_x2 / 2;
	f->id_range_offset = f->id_delta + f->seg_count_x2 / 2;
	f->glyph_id_array = f->id_range_offset + f->seg_count_x2 / 2;
}

int	read_format4(t_bin *bin, t_ttf *ttf, t_format4 **format4)
{
	t_uint		length;
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
		return (free_format_ret(f));
}

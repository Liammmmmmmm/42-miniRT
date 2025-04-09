/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_head.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:43:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 15:24:20 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	read_head(t_bin *bin, t_ttf *ttf)
{
	size_t	i;

	i = ttf->r_data.head_offset;
	if (read_uint32_move(bin, &i, &ttf->head.version) == -1
		|| read_uint32_move(bin, &i, &ttf->head.font_revision) == -1
		|| read_uint32_move(bin, &i, &ttf->head.check_sum_adjustment) == -1
		|| read_uint32_move(bin, &i, &ttf->head.magic_number) == -1
		|| ttf->head.magic_number != MAGIC_NUMBER
		|| read_uint16_move(bin, &i, &ttf->head.flags) == -1
		|| read_uint16_move(bin, &i, &ttf->head.units_per_em) == -1
		|| read_int64_move(bin, &i, &ttf->head.created) == -1
		|| read_int64_move(bin, &i, &ttf->head.modified) == -1
		|| read_int16_move(bin, &i, &ttf->head.xmin) == -1
		|| read_int16_move(bin, &i, &ttf->head.ymin) == -1
		|| read_int16_move(bin, &i, &ttf->head.xmax) == -1
		|| read_int16_move(bin, &i, &ttf->head.ymax) == -1
		|| read_uint16_move(bin, &i, &ttf->head.mac_style) == -1
		|| read_uint16_move(bin, &i, &ttf->head.lowest_rec_ppem) == -1
		|| read_int16_move(bin, &i, &ttf->head.font_direction_hint) == -1
		|| read_int16_move(bin, &i, &ttf->head.index_to_loc_format) == -1
		|| read_int16_move(bin, &i, &ttf->head.glyph_data_format) == -1)
		return (-1);
	if (ttf->head.units_per_em == 0)
		ttf->head.units_per_em = 2048;
	return (0);
}

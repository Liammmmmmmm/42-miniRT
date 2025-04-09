/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_hhea.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:48:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 17:46:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	read_hhea(t_bin *bin, t_ttf *ttf)
{
	size_t	i;

	i = ttf->r_data.hhea_offset;
	if (read_uint32_move(bin, &i, &ttf->hhea.version) == -1
		|| read_int16_move(bin, &i, &ttf->hhea.ascent) == -1
		|| read_int16_move(bin, &i, &ttf->hhea.descent) == -1
		|| read_int16_move(bin, &i, &ttf->hhea.line_gap) == -1
		|| read_uint16(bin, ttf->r_data.hhea_offset + 34,
			&ttf->hhea.num_hmetrics))
		return (-1);
	return (0);
}

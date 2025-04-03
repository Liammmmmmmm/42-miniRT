/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_cmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:35:14 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/03 10:20:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static int	free_map_ret(t_cmap_encoding_subtable *map)
{
	free(map);
	return (-1);
}

int	read_cmap(t_bin *bin, size_t *i, t_cmap *cmap)
{
	t_cmap_encoding_subtable	*enc_s;
	uint16_t					y;

	if (read_uint16_move(bin, i, &cmap->version) == -1)
		return (-1);
	if (read_uint16_move(bin, i, &cmap->number_subtables) == -1)
		return (-1);
	cmap->subtables = ft_calloc(cmap->number_subtables,
			sizeof(t_cmap_encoding_subtable));
	if (!cmap->subtables)
		return (-1);
	y = 0;
	while (y < cmap->number_subtables)
	{
		enc_s = cmap->subtables + y;
		if (read_uint16_move(bin, i, &enc_s->platform_id) == -1)
			return (free_map_ret(cmap->subtables));
		if (read_uint16_move(bin, i, &enc_s->platform_specific_id) == -1)
			return (free_map_ret(cmap->subtables));
		if (read_uint32_move(bin, i, &enc_s->offset) == -1)
			return (free_map_ret(cmap->subtables));
		y++;
	}
	return (0);
}

int	font_unicode_format4(t_bin *bin, t_ttf *ttf, uint16_t cmap_offset)
{
	uint16_t	i;
	size_t		r;

	i = 0;
	while (i < ttf->cmap.number_subtables)
	{
		if (ttf->cmap.subtables[i].platform_id == 0)
			break ;
		i++;
	}
	if (i == ttf->cmap.number_subtables)
		return (-1);
	ttf->r_data.uni_f4_offset = ttf->cmap.subtables[i].offset;
	if (read_uint16(bin, (size_t)(cmap_offset + ttf->r_data.uni_f4_offset), &i)
		== -1)
		return (-1);
	if (i != 4)
		return (-1);
	return (1);
}

int	get_cmap(t_bin *bin, t_ttf *ttf)
{
	t_table_directory	*cmap_tbl;
	size_t				cmap_offset;

	cmap_tbl = get_table_directory(&ttf->ft_dir, "cmap");
	if (!cmap_tbl)
	{
		printf("cant find cmap table\n");
		return (-1);
	}
	cmap_offset = cmap_tbl->offset;
	ttf->r_data.cmap_offset = cmap_tbl->offset;
	if (read_cmap(bin, &cmap_offset, &ttf->cmap) == -1)
		return (-1);
	if (font_unicode_format4(bin, ttf, cmap_tbl->offset) == -1)
	{
		printf("Unsupported font format. Only support format4 unicode fonts.\n")
		;
		return (-1);
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_font_directory.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:07:16 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/03 10:18:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	read_offset_subtable(t_bin *bin, size_t *i, t_offset_subtable *off_sub)
{
	if (read_uint32_move(bin, i, &off_sub->scaler_type) == -1)
		return (-1);
	if (read_uint16_move(bin, i, &off_sub->num_tables) == -1)
		return (-1);
	if (read_uint16_move(bin, i, &off_sub->search_range) == -1)
		return (-1);
	if (read_uint16_move(bin, i, &off_sub->entry_selector) == -1)
		return (-1);
	if (read_uint16_move(bin, i, &off_sub->range_shift) == -1)
		return (-1);
	return (0);
}

int	free_tbl_ret(t_table_directory *tbl)
{
	free(tbl);
	return (-1);
}

int	read_table_directory(t_bin *bin, size_t *i, t_table_directory **tbl_dir,
		uint16_t tbl_size)
{
	uint16_t			y;
	t_table_directory	*tbl;

	*tbl_dir = ft_calloc(tbl_size, sizeof(t_table_directory));
	if (!tbl_dir)
		return (-1);
	y = 0;
	while (y < tbl_size)
	{
		tbl = *tbl_dir + y;
		if (read_uint32_move(bin, i, &tbl->tag) == -1)
			return (free_tbl_ret(*tbl_dir));
		if (read_uint32_move(bin, i, &tbl->check_sum) == -1)
			return (free_tbl_ret(*tbl_dir));
		if (read_uint32_move(bin, i, &tbl->offset) == -1)
			return (free_tbl_ret(*tbl_dir));
		if (read_uint32_move(bin, i, &tbl->length) == -1)
			return (free_tbl_ret(*tbl_dir));
		y++;
	}
	return (0);
}

int	read_font_directory(t_bin *bin, t_font_directoy *ft_dir)
{
	size_t	i;

	i = 0;
	if (read_offset_subtable(bin, &i, &ft_dir->off_sub) == -1)
		return (-1);
	if (read_table_directory(bin, &i, &ft_dir->tbl_dir,
			ft_dir->off_sub.num_tables) == -1)
		return (-1);
	return (0);
}

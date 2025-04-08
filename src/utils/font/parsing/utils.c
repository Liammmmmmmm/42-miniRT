/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:59:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 14:08:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	cmp_tbl_tag(char c[4], uint32_t tag)
{
	if (tag == ((uint32_t)(c[0] << 24) | (uint32_t)(c[1] << 16)
		| (uint32_t)(c[2] << 8) | (uint32_t)c[3]))
		return (1);
	return (0);
}

t_table_directory	*get_table_directory(t_font_directory *ft_dir, char c[4])
{
	uint16_t	i;

	i = 0;
	while (i < ft_dir->off_sub.num_tables)
	{
		if (cmp_tbl_tag(c, ft_dir->tbl_dir[i].tag))
			return (&ft_dir->tbl_dir[i]);
		i++;
	}
	return (NULL);
}

uint32_t	get_glyph_index(uint16_t code_point, t_format4 *f)
{
	int			i;
	uint16_t	*ptr;

	i = 0;
	while (i < f->seg_count_x2 / 2)
	{
		if (f->end_code[i] > code_point)
			break ;
		i++;
	}
	if (i == f->seg_count_x2 / 2)
		return (0);
	printf("INDEX 1 : %d\n", i);
	if (f->start_code[i] < code_point)
	{
		if (f->id_range_offset[i] == 0)
			return (code_point + (int16_t)f->id_delta[i]);
		ptr = f->id_range_offset + i + f->id_range_offset[i] / 2;
		ptr += code_point - f->start_code[i];
		if (*ptr == 0)
			return (0);
		return (*ptr + f->id_delta[i]);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 14:59:09 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/03 10:17:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

t_table_directory	*get_table_directory(t_font_directoy *ft_dir, char c[4])
{
	uint16_t	i;

	i = 0;
	while (i < ft_dir->off_sub.num_tables)
	{
		if (ft_dir->tbl_dir[i].tag_c[0] == c[3]
			&& ft_dir->tbl_dir[i].tag_c[1] == c[2]
			&& ft_dir->tbl_dir[i].tag_c[2] == c[1]
			&& ft_dir->tbl_dir[i].tag_c[3] == c[0])
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
	if (f->start_code[i] < code_point)
	{
		if (f->id_range_offset[i] == 0)
			return (code_point + f->id_delta[i]);
		ptr = f->id_range_offset + i + f->id_range_offset[i] / 2;
		ptr += code_point - f->start_code[i];
		if (*ptr == 0)
			return (0);
		return (*ptr + f->id_delta[i]);
	}
	return (0);
}

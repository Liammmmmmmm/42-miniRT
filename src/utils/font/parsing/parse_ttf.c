/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ttf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:35:05 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/03 15:54:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	get_ttf(t_bin *bin, t_ttf *ttf)
{
	if (read_font_directory(bin, &ttf->ft_dir, ttf) == -1)
		return (-1); // MSG: error invalid file format
	if (FONT_DEBUG)
		print_table_directory(ttf->ft_dir.tbl_dir, ttf->ft_dir.off_sub.num_tables);
	if (get_cmap(bin, ttf) == -1)
		return (-1); // MSG: invalid cmap table
	if (FONT_DEBUG)
		print_cmap(&ttf->cmap);
	if (read_format4(bin, ttf) == -1)
		return (-1); // error format4
	if (FONT_DEBUG)
		print_format4(ttf->cmap.format4);
	if (read_head(bin, ttf) == -1)
		return (-1); // corrupted ?
	uint16_t i = 'A';
	while (i <= 'Z')
	{
		printf("%c = %d, %d    ", i, get_glyph_index(i, ttf->cmap.format4), get_glyph_offset(bin, ttf, get_glyph_index(i, ttf->cmap.format4)));
		i++;
	}
	return (0);
}

int	get_font(t_ttf *ttf, char *filename)
{
	t_bin bin;

	if (!read_bin_file(&bin, filename))
		return (-1); // MSG: cant read font file
	if (get_ttf(&bin, ttf) == -1)
		return (-1);
	free(bin.data);
	return (0);
}

int	main(void)
{
	t_ttf	ttf;

	get_font(&ttf, "test3.ttf");
}

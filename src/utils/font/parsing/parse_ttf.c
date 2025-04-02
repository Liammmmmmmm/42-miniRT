/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ttf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:35:05 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/02 16:05:00 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	get_ttf(t_bin *bin, t_ttf *ttf)
{
	if (read_font_directory(bin, &ttf->ft_dir) == -1)
		return (-1); // MSG: error invalid file format
	if (FONT_DEBUG)
		print_table_directory(ttf->ft_dir.tbl_dir, ttf->ft_dir.off_sub.num_tables);
	if (get_cmap(bin, ttf) == -1)
		return (-1); // MSG: invalid cmap table
	if (FONT_DEBUG)
		print_cmap(&ttf->cmap);
	return (0);
}

int get_font(t_ttf *ttf, char *filename)
{
	t_bin bin;

	if (!read_bin_file(&bin, filename))
		return (-1); // MSG: cant read font file
	if (get_ttf(&bin, ttf) == -1)
		return (-1);
	return (0);
}

int main()
{
	t_ttf ttf;

	get_font(&ttf, "test2.ttf");
}

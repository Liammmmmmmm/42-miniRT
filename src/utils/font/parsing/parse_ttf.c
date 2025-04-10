/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ttf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:35:05 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 14:49:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

static int	free_print_msg(t_ttf *ttf, char *msg)
{
	free_ttf(ttf);
	return (print_err_ttf(msg));
}

int	get_ttf(t_bin *bin, t_ttf *ttf)
{
	if (read_font_directory(bin, ttf) == -1)
		return (print_err_ttf("Error reading font directory. Is it really a ttf"
				" file ?"));
	if (get_cmap(bin, ttf) == -1)
		return (free_ttf(ttf));
	if (read_format4(bin, ttf) == -1)
		return (free_print_msg(ttf, "Error reading font format4. It may not be "
				"compatible."));
	if (read_head(bin, ttf) == -1)
		return (free_print_msg(ttf, "Error reading font file (corrupted file)."));
	if (save_glyph256(bin, ttf) == -1)
		return (free_print_msg(ttf, "Error reading font (malloc failed)."));
	read_hhea(bin, ttf);
	read_hmtx(bin, ttf);
	read_maxp(bin, ttf);
	ttf->r_data.estimated_max_seg_intersect = 64;
	ttf->r_data.seg_intersec = ft_calloc(ttf->r_data.estimated_max_seg_intersect, sizeof(uint32_t));
	if (!ttf->r_data.seg_intersec)
		return (free_print_msg(ttf, "Error reading font (malloc failed)."));
	set_bezier_res(ttf, 5);
	return (0);
}

int	get_font(t_ttf *ttf, char *filename)
{
	t_bin	bin;
	int		ttf_res;

	if (!ttf)
		return (-1);
	if (!read_bin_file(&bin, filename))
		return (print_err_ttf("Error reading font file."));
	ft_bzero(ttf, sizeof(t_ttf));
	ttf_res = get_ttf(&bin, ttf);
	free(bin.data);
	ttf->size = 50;
	return (ttf_res);
}

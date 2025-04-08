/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ttf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 15:35:05 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 19:20:18 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	get_ttf(t_bin *bin, t_ttf *ttf)
{
	if (read_font_directory(bin, &ttf->ft_dir, ttf) == -1)
		return (print_err_ttf("Error reading font directory. Is it really a ttf"
			" file ?"));
	if (get_cmap(bin, ttf) == -1)
		return (free_ttf(ttf));
	if (read_format4(bin, ttf) == -1)
	{
		free_ttf(ttf);
		return (print_err_ttf("Error reading font format4. It may not be "
			"compatible."));
	}
	if (read_head(bin, ttf) == -1)
	{
		free_ttf(ttf);
		return (print_err_ttf("Error reading font file (corrupted file)."));
	}
	if (save_glyph256(bin, ttf) == -1)
	{
		free_ttf(ttf);
		return (print_err_ttf("Error reading font file (malloc failed)."));
	}
	return (0);
}

int	get_font(t_ttf *ttf, char *filename)
{
	t_bin	bin;
	int		ttf_res;

	if (!read_bin_file(&bin, filename))
		return (print_err_ttf("Error reading font file."));
	ft_bzero(&ttf, sizeof(t_ttf));
	ttf_res = get_ttf(&bin, ttf);
	free(bin.data);
	return (ttf_res);
}

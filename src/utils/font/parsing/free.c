/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:02:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 19:18:09 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	free_glyph(t_glyph_outline *o)
{
	free(o->end_pts_of_contours);
	free(o->flags);
	free(o->x_coordinates);
	free(o->y_coordinates);
	free(o->instructions);
	return (-1);
}

int	free_glyphs(t_glyph_outline *glyphs, int i)
{
	int	y;

	y = 0;
	while (y < i && glyphs)
	{
		free_glyph(&glyphs[y]);
		y++;
	}
	free(glyphs);
	return (-1);
}

int	free_ttf(t_ttf *ttf)
{
	free(ttf->ft_dir.tbl_dir);
	free(ttf->cmap.subtables);
	free(ttf->cmap.format4);
	free_glyphs(ttf->glyph256, 256);
	return (-1);
}

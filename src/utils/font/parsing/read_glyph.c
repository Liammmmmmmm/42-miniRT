/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_glyph.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:33:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/09 11:40:14 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	save_glyph256(t_bin *bin, t_ttf *ttf)
{
	int	i;

	ttf->glyph256 = ft_calloc(256, sizeof(t_glyph_outline));
	if (!ttf->glyph256)
		return (-1);
	i = 0;
	while (i < 256)
	{
		if (get_glyph_outline(bin, ttf, get_glyph_index(i, ttf->cmap.format4),
				&ttf->glyph256[i]) == -1)
			return (free_glyphs(ttf->glyph256, i));
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 09:02:25 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/04 09:05:30 by lilefebv         ###   ########lyon.fr   */
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

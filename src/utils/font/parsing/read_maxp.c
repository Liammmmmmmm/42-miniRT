/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_maxp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:05:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/10 12:14:35 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

int	read_maxp(t_bin *bin, t_ttf *ttf)
{
	if (read_uint16(bin, ttf->r_data.maxp_offset + 8, &ttf->maxp.max_contours))
		return (-1);
	return (0);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdr_set_pixel.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 15:15:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 15:31:12 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"

void	save_hdr_r(t_hdr *hdr, int x, int y, char r)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].r = r;
}

void	save_hdr_g(t_hdr *hdr, int x, int y, char g)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].g = g;
}

void	save_hdr_b(t_hdr *hdr, int x, int y, char b)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].b = b;
}

void	save_hdr_e(t_hdr *hdr, int x, int y, char e)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].e = e;
}

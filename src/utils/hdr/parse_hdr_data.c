/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:20:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 12:28:24 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"

void	save_pixel_hdr(t_hdr *hdr, int x, int y, t_rgbe pixel)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x] = pixel;
}

void	save_pixel_hdr_r(t_hdr *hdr, int x, int y, char r)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].r = r;
}

void	save_pixel_hdr_g(t_hdr *hdr, int x, int y, char g)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].g = g;
}

void	save_pixel_hdr_b(t_hdr *hdr, int x, int y, char b)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].b = b;
}

void	save_pixel_hdr_e(t_hdr *hdr, int x, int y, char e)
{
	if (hdr->dir_x == -1)
		x = hdr->width - 1 - x;
	if (hdr->dir_y == -1)
		y = hdr->height - 1 - y;
	hdr->pixels[y * hdr->width + x].e = e;
}



int	read_compressed_line(t_hdr *hdr, t_bin *bin, size_t *index, int line)
{
	uint16_t	len;

	if (read_uint16_move(bin, index, &len) == -1 || len != ((0x02 << 8) | 0x02))
		return (-1);
	if (read_uint16_move(bin, index, &len) == -1)
		return (-1);
	if ((int)len > hdr->width)
		len = hdr->width;

	uint16_t	i;
	uint8_t		count;
	uint8_t		y;
	uint8_t		tmp;
	i = 0;
	while (i < len)
	{
		if (read_uint8_move(bin, index, &count) == -1)
			return (-1);
		y = 0;
		if (count > 128) // run
		{
			count -= 128;
			if (read_uint8_move(bin, index, &tmp) == -1)
				return (-1);
			if (i + count > len) {
				ft_printf("Invalid RLE: packet overflow (i=%d + count=%d > len=%d)\n", i, count, len);
				return -1;
			}
			while (y < count)
			{
				save_pixel_hdr_r(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
		else if (count <= 128) // dump
		{
			if (i + count > len) {
				ft_printf("Invalid RLE: packet overflow (i=%d + count=%d > len=%d)\n", i, count, len);
				return -1;
			}
			while (y < count)
			{
				if (read_uint8_move(bin, index, &tmp) == -1)
					return (-1);
				save_pixel_hdr_r(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
	}

	i = 0;
	while (i < len)
	{
		if (read_uint8_move(bin, index, &count) == -1)
			return (-1);
		y = 0;
		if (count > 128) // run
		{
			count -= 128;
			if (read_uint8_move(bin, index, &tmp) == -1)
				return (-1);
			while (y < count && i < len)
			{
				save_pixel_hdr_g(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
		else if (count <= 128) // dump
		{
			while (y < count && i < len)
			{
				if (read_uint8_move(bin, index, &tmp) == -1)
					return (-1);
				save_pixel_hdr_g(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
	}

	i = 0;
	while (i < len)
	{
		if (read_uint8_move(bin, index, &count) == -1)
			return (-1);
		y = 0;
		if (count > 128) // run
		{
			count -= 128;
			if (read_uint8_move(bin, index, &tmp) == -1)
				return (-1);
			while (y < count && i < len)
			{
				save_pixel_hdr_b(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
		else if (count <= 128) // dump
		{
			while (y < count && i < len)
			{
				if (read_uint8_move(bin, index, &tmp) == -1)
					return (-1);
				save_pixel_hdr_b(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
	}

	i = 0;
	while (i < len)
	{
		if (read_uint8_move(bin, index, &count) == -1)
			return (-1);
		y = 0;
		if (count > 128) // run
		{
			count -= 128;
			if (read_uint8_move(bin, index, &tmp) == -1)
				return (-1);
			while (y < count && i < len)
			{
				save_pixel_hdr_e(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
		else if (count <= 128) // dump
		{
			while (y < count && i < len)
			{
				if (read_uint8_move(bin, index, &tmp) == -1)
					return (-1);
				save_pixel_hdr_e(hdr, i, line, tmp);
				i++;
				y++;
			}
		}
	}

	return (0);
}

int	read_hdr_data(t_hdr *hdr, t_bin *bin, size_t index)
{
	int i;

	i = 0;
	while (i < hdr->height)
	{
		if (read_compressed_line(hdr, bin, &index, i) == -1)
			return (-1);	
		i++;
	}
	return (0);
}

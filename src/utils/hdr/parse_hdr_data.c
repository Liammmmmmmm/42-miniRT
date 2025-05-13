/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_hdr_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 16:20:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 15:17:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "hdr_parsing.h"

static inline int	rle_run(t_hdr_tmp hdr_tmp, uint8_t count, uint16_t *i,
	void (*save_pixel_hdr)(t_hdr *, int, int, char))
{
	uint8_t	tmp;
	uint8_t	y;

	count -= 128;
	if (read_uint8_move(hdr_tmp.bin, hdr_tmp.index, &tmp) == -1)
		return (-1);
	if (*i + count > hdr_tmp.len)
		return (print_err_hdr("Invalid RLE: packet overflow"));
	y = -1;
	while (++y < count)
		save_pixel_hdr(hdr_tmp.hdr, (*i)++, hdr_tmp.line, tmp);
	return (0);
}

static inline int	rle_dump(t_hdr_tmp hdr_tmp, uint8_t count, uint16_t *i,
	void (*save_pixel_hdr)(t_hdr *, int, int, char))
{
	uint8_t	tmp;
	uint8_t	y;

	if (*i + count > hdr_tmp.len)
		return (print_err_hdr("Invalid RLE: packet overflow"));
	y = -1;
	while (++y < count)
	{
		if (read_uint8_move(hdr_tmp.bin, hdr_tmp.index, &tmp) == -1)
			return (-1);
		save_pixel_hdr(hdr_tmp.hdr, (*i)++, hdr_tmp.line, tmp);
	}
	return (0);
}

/**
 * Compression algorithm working with run and dump data.
 * How does it work ?
 * - Read one byte.
 * - If this byte is <= 128, dump with this byte
 * - If this byte is > 128, run with this byte - 128
 * 
 * Dump :
 * Dump is just reading raw bytes. If we have a dump 10, the next 10 bytes will
 * be raw pixels
 * Run :
 * Run is a repetition of a byte. Run 10 (first read byte of 138) means that the
 * 10 next pixels will be the same byte.
 */
int	rle_rl(t_hdr_tmp hdr_tmp, void (*save_pixel_hdr)(t_hdr *, int, int, char))
{
	uint16_t	i;
	uint8_t		count;

	i = 0;
	while (i < hdr_tmp.len)
	{
		if (read_uint8_move(hdr_tmp.bin, hdr_tmp.index, &count) == -1)
			return (-1);
		if (count > 128)
		{
			if (rle_run(hdr_tmp, count, &i, save_pixel_hdr) == -1)
				return (-1);
		}
		else if (count <= 128)
		{
			if (rle_dump(hdr_tmp, count, &i, save_pixel_hdr) == -1)
				return (-1);
		}
	}
	return (0);
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
	if (rle_rl((t_hdr_tmp){hdr, bin, index, len, line}, &save_hdr_r) == -1
		|| rle_rl((t_hdr_tmp){hdr, bin, index, len, line}, &save_hdr_g) == -1
		|| rle_rl((t_hdr_tmp){hdr, bin, index, len, line}, &save_hdr_b) == -1
		|| rle_rl((t_hdr_tmp){hdr, bin, index, len, line}, &save_hdr_e) == -1)
		return (-1);
	return (0);
}

int	read_hdr_data(t_hdr *hdr, t_bin *bin, size_t index)
{
	int	i;

	i = 0;
	while (i < hdr->height)
	{
		if (read_compressed_line(hdr, bin, &index, i) == -1)
			return (-1);
		i++;
	}
	return (0);
}

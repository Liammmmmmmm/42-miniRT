/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompress_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:07:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 12:13:37 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

/**
 * This file contains the knowledge of god.
 * From now, only me, god and an expert of the deflate compression can
 * understand what's appening here.
 * Here lie the sacred rites of Huffman and the dark art of bit peeking.
 * Proceed with reverence. And breakpoints.
 * 
 * According to the ancient lore of RFC 1951:
 * And on the 8th day, He said: "Let there be bitstreams," and lo, chaos was
 * born.
 * Good Luck brave warrior — abandon hope, ye who decode here.
 */

static inline int	repeat_code(t_bit_stream *stream, t_huffman_table *table,
	t_bin_cursor *out, int sym)
{
	uint32_t	length;
	int			dist_sym;
	uint32_t	distance;

	length = decode_length(sym, stream);
	if (out->cursor + length >= out->data.size)
		return (print_err_png("Corrupted png"));
	dist_sym = decode_symbol_huffman_table(stream, &table->hdist);
	if (dist_sym < 0)
		return (print_err_png("Invalid dist symbol"));
	distance = decode_distance(dist_sym, stream);
	if (distance > out->cursor)
		return (print_err_png("Invalid distance"));
	ft_memcpy(&out->data.data[out->cursor],
		&out->data.data[out->cursor - distance],
		length);
	out->cursor += length;
	return (0);
}

int	decompress_huffman_data(t_bit_stream *stream, t_huffman_table *table,
	t_bin_cursor *out)
{
	int			sym;

	while (1)
	{
		sym = decode_symbol_huffman_table(stream, &table->hlit);
		if (sym < 0)
			return (print_err_png("Invalid symbol"));
		if (sym < 256)
		{
			if (out->cursor >= out->data.size)
				return (print_err_png("Corrupted png"));
			out->data.data[out->cursor++] = (uint8_t)sym;
		}
		else if (sym == 256)
			break ;
		else if (sym <= 285)
		{
			if (repeat_code(stream, table, out, sym) == -1)
				return (-1);
		}
		else
			return (print_err_png("Symbol inconnu"));
	}
	return (0);
}

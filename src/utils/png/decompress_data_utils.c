/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompress_data_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:01:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 11:10:20 by lilefebv         ###   ########lyon.fr   */
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

uint32_t	decode_length(int sym, t_bit_stream *stream)
{
	static const uint16_t	length_base[29] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13,
		15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195,
		227, 258};
	static const uint8_t	length_extra_bits[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
	int						index;

	if (sym < 257 || sym > 285)
	{
		printf("Invalid length symbol: %d\n", sym);
		return (0);
	}
	index = sym - 257;
	if (length_extra_bits[index] == 0)
		return (length_base[index]);
	return (length_base[index] + read_bits(stream, length_extra_bits[index]));
}

uint32_t	decode_distance(int sym, t_bit_stream *stream)
{
	static const uint16_t	distance_base[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17,
		25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049,
		3073, 4097, 6145, 8193, 12289, 16385, 24577};
	static const uint8_t	distance_extra_bits[30] = {0, 0, 0, 0, 1, 1, 2, 2,
		3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
	};

	if (sym < 0 || sym > 29)
	{
		printf("Invalid distance symbol: %d\n", sym);
		return (0);
	}
	if (distance_extra_bits[sym] == 0)
		return (distance_base[sym]);
	return (distance_base[sym] + read_bits(stream, distance_extra_bits[sym]));
}

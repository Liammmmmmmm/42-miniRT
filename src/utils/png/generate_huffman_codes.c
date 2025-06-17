/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_huffman_codes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 17:57:23 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 11:09:59 by lilefebv         ###   ########lyon.fr   */
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

static inline void	count_code_lengths(const uint8_t *code_lengths,
	uint32_t num_symbols, uint16_t *bl_count, size_t max_bits)
{
	uint32_t	i;

	i = 0;
	while (i < num_symbols)
	{
		if (code_lengths[i] > 0)
		{
			if (code_lengths[i] > max_bits)
			{
				printf("Invalid code length: %u\n", code_lengths[i]);
				continue ;
			}
			bl_count[code_lengths[i]]++;
		}
		i++;
	}
}

void	generate_huffman_codes(const uint8_t *code_lengths, uint16_t *codes,
	uint32_t num_symbols, uint32_t max_bits)
{
	uint16_t	bl_count[16];
	uint16_t	next_code[16];
	uint16_t	code;
	uint32_t	i;
	uint8_t		len;

	ft_bzero(bl_count, sizeof(uint16_t) * 16);
	ft_bzero(next_code, sizeof(uint16_t) * 16);
	count_code_lengths(code_lengths, num_symbols, bl_count, max_bits);
	i = 1;
	code = 0;
	while (i <= max_bits)
	{
		code = (code + bl_count[i - 1]) << 1;
		next_code[i] = code;
		i++;
	}
	i = 0;
	while (i < num_symbols)
	{
		len = code_lengths[i];
		if (len)
			codes[i] = next_code[len]++;
		i++;
	}
}

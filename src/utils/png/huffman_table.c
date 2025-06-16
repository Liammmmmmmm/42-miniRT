/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   huffman_table.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:55:43 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/16 17:23:07 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

static inline uint16_t	reverse_bits(uint16_t code, uint8_t length)
{
	code = ((code & 0x5555) << 1) | ((code & 0xAAAA) >> 1);
	code = ((code & 0x3333) << 2) | ((code & 0xCCCC) >> 2);
	code = ((code & 0x0F0F) << 4) | ((code & 0xF0F0) >> 4);
	code = ((code & 0x00FF) << 8) | ((code & 0xFF00) >> 8);
	return (code >> (16 - length));
}

static inline void	fill_table_with_symbol(uint16_t code, uint8_t len,
	t_huffman_lookup_entry *table, uint32_t i)
{
	const uint16_t	base = code << (MAX_LOOKUP_BITS - len);
	const uint16_t	repeat_count = 1 << (MAX_LOOKUP_BITS - len);
	uint16_t		y;
	uint32_t		index;

	y = 0;
	while (y < repeat_count)
	{
		index = reverse_bits(base | y, MAX_LOOKUP_BITS);
		table[index].symbol = i;
		table[index].length = len;
		y++;
	}
}

void	build_huffman_lookup_table(t_huffman_data *data)
{
	const uint32_t	table_size = 1 << MAX_LOOKUP_BITS;
	uint32_t		i;
	uint8_t			len;
	uint16_t		base;
	uint16_t		repeat_count;

	i = 0;
	while (i < table_size)
	{
		data->table[i].symbol = -1;
		data->table[i].length = 0;
		i++;
	}
	i = (uint32_t)(-1);
	while (++i < data->num_symbols)
	{
		len = data->code_lengths[i];
		if (len == 0 || len > MAX_LOOKUP_BITS)
			continue ;
		fill_table_with_symbol(data->codes[i], len, data->table, i);
	}
}

static inline int	decode_symbol_huffman_fallback(t_bit_stream *bs,
	t_huffman_data *data, const uint32_t peek)
{
	uint16_t	code;
	uint8_t		length;
	uint8_t		bit;
	uint32_t	i;

	consume_bits(bs, MAX_LOOKUP_BITS);
	length = MAX_LOOKUP_BITS;
	code = reverse_bits(peek, MAX_LOOKUP_BITS);
	while (++length <= MAX_CODE_LENGTH_DATA)
	{
		bit = read_bits(bs, 1);
		code = (code << 1) | bit;
		i = 0;
		while (i < data->num_symbols)
		{
			if (data->code_lengths[i] == length && data->codes[i] == code)
				return ((int)i);
			i++;
		}
	}
	return (-1);
}

int	decode_symbol_huffman_table(t_bit_stream *bs, t_huffman_data *data)
{
	const uint32_t					peek = peek_bits(bs, MAX_LOOKUP_BITS);
	const t_huffman_lookup_entry	entry = data->table[peek];

	if (entry.length > 0)
	{
		consume_bits(bs, entry.length);
		return (entry.symbol);
	}
	return (decode_symbol_huffman_fallback(bs, data, peek));
}

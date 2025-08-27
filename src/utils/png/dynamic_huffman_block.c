/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_huffman_block.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:56:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/27 14:29:54 by lilefebv         ###   ########lyon.fr   */
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

static int	get_huffman_table(t_bit_stream *stream,
	t_huffman_table *huffman_table, const t_deflate_num_tables ntbl)
{
	uint8_t			code_length_codes[19];
	uint16_t		code_length_huffman_codes[19];
	t_huffman_data	hclen_data;
	uint8_t			*all_code_lengths;
	uint16_t		*all_code;

	read_code_lengh(code_length_codes, ntbl.hclen, stream);
	generate_huffman_codes(code_length_codes, code_length_huffman_codes, 19,
		MAX_CODE_LENGTH_TABLE);
	hclen_data = (t_huffman_data){.code_lengths = code_length_codes,
		.codes = code_length_huffman_codes, .num_symbols = 19, .table = NULL};
	all_code_lengths = malloc((ntbl.hlit + ntbl.hdist) * sizeof(uint8_t));
	all_code = malloc((ntbl.hlit + ntbl.hdist) * sizeof(uint16_t));
	if (decode_code_lengths(all_code_lengths, ntbl.hlit + ntbl.hdist,
			&hclen_data, stream) == -1)
	{
		free(all_code_lengths);
		free(all_code);
		return (print_err_png("Failed to decode code lengths"));
	}
	huffman_table->hlit = (t_huffman_data){.code_lengths = all_code_lengths,
		.codes = all_code, .num_symbols = ntbl.hlit, .table = ntbl.table_lit};
	huffman_table->hdist = (t_huffman_data){all_code_lengths + ntbl.hlit,
		all_code + ntbl.hlit, ntbl.hdist, ntbl.table_dist};
	return (0);
}

int	handle_dynamic_huffman_block(t_bit_stream *stream, t_bin_cursor *out_buf,
	t_huffman_lookup_entry *table_lit, t_huffman_lookup_entry *table_dist)
{
	const uint32_t	hlit = read_bits(stream, 5) + 257;
	const uint32_t	hdist = read_bits(stream, 5) + 1;
	const uint32_t	hclen = read_bits(stream, 4) + 4;
	t_huffman_table	huffman_table;

	if (get_huffman_table(stream, &huffman_table, (t_deflate_num_tables)
			{hlit, hdist, hclen, table_lit, table_dist}) == -1)
		return (-1);
	generate_huffman_codes(huffman_table.hlit.code_lengths, huffman_table
		.hlit.codes, huffman_table.hlit.num_symbols, MAX_CODE_LENGTH_DATA);
	generate_huffman_codes(huffman_table.hdist.code_lengths, huffman_table
		.hdist.codes, huffman_table.hdist.num_symbols, MAX_CODE_LENGTH_DATA);
	build_huffman_lookup_table(&huffman_table.hlit);
	build_huffman_lookup_table(&huffman_table.hdist);
	if (decompress_huffman_data(stream, &huffman_table, out_buf) == -1)
	{
		free(huffman_table.hlit.code_lengths);
		free(huffman_table.hlit.codes);
		return (print_err_png("Failed to decompress Huffman data"));
	}
	free(huffman_table.hlit.code_lengths);
	free(huffman_table.hlit.codes);
	return (0);
}

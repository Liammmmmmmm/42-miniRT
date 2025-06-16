/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 17:24:36 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/16 17:55:53 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

static inline int	decode_symbol_micro_table(const uint8_t *code_lengths,
	const uint16_t *codes, uint32_t num_symbols, t_bit_stream *stream)
{
	uint16_t	code;
	uint8_t		length;
	uint8_t		bit;
	uint32_t	i;

	length = 0;
	code = 0;
	while (++length <= MAX_CODE_LENGTH_TABLE)
	{
		bit = read_bits(stream, 1);
		if (bit > 1)
			return (print_err_png("T'as pas lu qu'un seul bit la"));
		code = (code << 1) | bit;
		i = 0;
		while (i < num_symbols)
		{
			if (code_lengths[i] == length && codes[i] == code)
				return ((int)i);
			i++;
		}
	}
	return (print_err_png("On est sorti de la boucle"));
}

static inline int	decode_symbole(t_decode_symbol_data *decode,
	uint8_t *code_lengths, uint32_t total_lengths, t_bit_stream *stream)
{
	int	repeat;
	int	j;

	if (decode->symbol == 16)
	{
		if (decode->i == 0)
			return (-1);
		repeat = read_bits(stream, 2) + 3;
		if (decode->i + repeat > total_lengths)
			return (-1);
		j = -1;
		while (++j < repeat)
			code_lengths[decode->i++] = decode->prev_length;
	}
	else if (decode->symbol == 17)
	{
		repeat = read_bits(stream, 3) + 3;
		if (decode->i + repeat > total_lengths)
			return (-1);
		j = -1;
		while (++j < repeat)
			code_lengths[decode->i++] = 0;
		decode->prev_length = 0;
	}
	return (0);
}

static inline int	decode_symbole_p2(t_decode_symbol_data *decode,
	uint8_t *code_lengths, uint32_t total_lengths, t_bit_stream *stream)
{
	int	repeat;
	int	j;

	if (decode->symbol == 18)
	{
		repeat = read_bits(stream, 7) + 11;
		if (decode->i + repeat > total_lengths)
			return (-1);
		j = -1;
		while (++j < repeat)
			code_lengths[decode->i++] = 0;
		decode->prev_length = 0;
	}
	return (0);
}

int	decode_code_lengths(uint8_t *code_lengths, uint32_t total_lengths,
	t_huffman_data *data, t_bit_stream *stream)
{
	t_decode_symbol_data	decode;

	decode.i = 0;
	decode.prev_length = 0;
	while (decode.i < total_lengths)
	{
		decode.symbol = decode_symbol_micro_table(data->code_lengths,
				data->codes, data->num_symbols, stream);
		if (decode.symbol < 0)
			return (-1);
		if (decode.symbol <= 15)
		{
			code_lengths[decode.i++] = (uint8_t)decode.symbol;
			decode.prev_length = (uint8_t)decode.symbol;
		}
		else if (decode_symbole(&decode, code_lengths, total_lengths, stream)
			== 0 && decode_symbole_p2(&decode, code_lengths, total_lengths,
				stream) == 0)
			;
		else
			return (-1);
	}
	return (0);
}

void	read_code_lengh(uint8_t *code_length_codes, const uint32_t hclen,
	t_bit_stream *stream)
{
	uint32_t				i;
	static const uint8_t	code_length_order[19] = {
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

	i = 0;
	while (i < 19)
	{
		code_length_codes[i] = 0;
		i++;
	}
	i = 0;
	while (i < hclen && i < 19)
	{
		code_length_codes[code_length_order[i]] = (uint8_t)read_bits(stream, 3);
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decompress_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:07:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/16 18:26:10 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int decompress_huffman_data(t_bit_stream *stream, t_huffman_table *table,
	t_bin *out, size_t *out_len)
{
	int			sym;
	int			dist_sym;
	uint32_t	length;
	uint32_t	distance;
	uint32_t	i;

	while (1)
	{
		sym = decode_symbol_huffman_table(stream, &table->hlit);
		if (sym < 0)
			return (print_err_png("Invalid symbol"));
		if (sym < 256)
		{
			if (*out_len >= out->size)
				return (print_err_png("Corrupted png"));
			out->data[(*out_len)++] = (uint8_t)sym;
		}
		else if (sym == 256)
			break ;
		else if (sym <= 285)
		{
			length = decode_length(sym, stream);
			if (*out_len + length >= out->size)
				return (print_err_png("Corrupted png"));
			dist_sym = decode_symbol_huffman_table(stream, &table->hdist);
			if (dist_sym < 0)
				return (print_err_png("Invalid dist symbol"));
			distance = decode_distance(dist_sym, stream);
			if (distance > *out_len)
				return (print_err_png("Invalid distance"));
			i = 0;
			while (i < length)
			{
				uint8_t c = out->data[*out_len - distance];
				out->data[*out_len] = c;
				(*out_len)++;
				i++;
			}
		}
		else
			return (print_err_png("Symbol inconnu"));
	}
	return 0;
}

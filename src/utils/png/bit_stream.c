/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_stream.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:35:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 14:16:40 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

void	init_bit_stream(t_bit_stream *bitstream, t_zlib_block *zlib)
{
	bitstream->data_stream = zlib->deflate_data;
	bitstream->size = zlib->deflate_size;
	bitstream->byte_index = 0;
	bitstream->bit_buffer = 0;
	bitstream->bits_remaining = 0;
}

void	fill_bits(t_bit_stream *bs)
{
	while (bs->bits_remaining <= 24 && bs->byte_index < bs->size)
	{
		bs->bit_buffer |= (uint32_t)(bs->data_stream[bs->byte_index])
			<< bs->bits_remaining;
		bs->bits_remaining += 8;
		bs->byte_index++;
	}
}

uint32_t	peek_bits(t_bit_stream *bs, uint32_t n)
{
	if (bs->bits_remaining < n)
		fill_bits(bs);
	if (bs->bits_remaining == 0)
		return (0);
	if (bs->bits_remaining < n)
		return (bs->bit_buffer & ((1U << bs->bits_remaining) - 1));
	return (bs->bit_buffer & ((1U << n) - 1));
}

void	consume_bits(t_bit_stream *bs, uint32_t n)
{
	if (bs->bits_remaining < n)
		fill_bits(bs);
	if (bs->bits_remaining < n)
	{
		bs->bit_buffer = 0;
		bs->bits_remaining = 0;
		return ;
	}
	bs->bit_buffer >>= n;
	bs->bits_remaining -= n;
}

uint32_t	read_bits(t_bit_stream *bs, uint32_t n)
{
	uint32_t	result;

	if (bs->bits_remaining < n)
		fill_bits(bs);
	if (bs->bits_remaining < n)
		return (0xFFFFFFFF);
	result = bs->bit_buffer & ((1U << n) - 1);
	bs->bit_buffer >>= n;
	bs->bits_remaining -= n;
	return (result);
}

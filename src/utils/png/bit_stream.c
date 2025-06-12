/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_stream.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:35:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 11:53:22 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

void	png_get_bits(t_png_bit_stream *bits, uint32_t bits_required)
{
	uint32_t	extra_bits_needed;
	uint32_t	bytes_to_read;
	uint32_t	i;
	uint32_t	byte;

	if (bits->bits_remaining > bits_required)
		extra_bits_needed = bits->bits_remaining - bits_required;
	else
		extra_bits_needed = bits_required - bits->bits_remaining;
	bytes_to_read = extra_bits_needed / 8;
	if(extra_bits_needed % 8)
		bytes_to_read++; 

	i = 0;
	while (i < bytes_to_read)
	{
		byte = *bits->data_stream++;
		bits->bit_buffer |= byte << (i * 8 + bits->bits_remaining);
		i++;
	}
	bits->bits_remaining += bytes_to_read * 8;
}

uint32_t	png_read_bits(t_png_bit_stream *bits, uint32_t bits_to_read)
{
	uint32_t	result;
	uint32_t	i;
	uint32_t	bit;

	result = 0;
	if(bits_to_read > bits->bits_remaining)
		png_get_bits(bits, bits_to_read);
	i = 0;
	while (i < bits_to_read)
	{
		bit = bits->bit_buffer & (1 << i);
		result |= bit;
		i++;
	}
	bits->bit_buffer >>= bits_to_read;
	bits->bits_remaining -= bits_to_read;
	return (result);
}

// Potentiellement bien plus efficace et correct :
// void	png_fill_bits(t_png_bit_stream *bs)
// {
// 	while (bs->bits_remaining <= 24)
// 	{
// 		bs->bit_buffer |= (*bs->data_stream++) << bs->bits_remaining;
// 		bs->bits_remaining += 8;
// 	}
// }

// uint32_t	png_read_bits(t_png_bit_stream *bs, uint32_t n)
// {
// 	uint32_t	result;

// 	if (bs->bits_remaining < n)
// 		png_fill_bits(bs);
// 	result = bs->bit_buffer & ((1U << n) - 1);
// 	bs->bit_buffer >>= n;
// 	bs->bits_remaining -= n;
// 	return (result);
// }
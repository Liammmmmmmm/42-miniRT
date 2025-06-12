/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png_chunk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:33:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 14:58:36 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	get_png_chunk(t_bin *bin, size_t *i, t_png_chunk *chunk)
{
	uint32_t	y;

	ft_bzero(chunk, sizeof(t_png_chunk));
	if (read_uint32_move(bin, i, &chunk->data_length) == -1)
		return (print_err_png(PNG_ERROR_GLOBAL));
	if (read_uint32_move_little(bin, i, &chunk->type) == -1)
		return (print_err_png(PNG_ERROR_GLOBAL));
	if (chunk->data_length)
		chunk->data = malloc(chunk->data_length * sizeof(uint8_t));
	if (chunk->data_length && !chunk->data)
		return (print_err_png(PNG_ERROR_MEM));
	y = 0;
	while (y < chunk->data_length)
	{
		if (read_uint8_move(bin, i, &chunk->data[y]) == -1)
			return (print_err_png(PNG_ERROR_CHUNK_DATA));
		y++;
	}
	if (read_uint32_move(bin, i, &chunk->crc32) == -1)
	{
		free(chunk->data);
		return (print_err_png(PNG_ERROR_GLOBAL));
	}
	return (0);
}


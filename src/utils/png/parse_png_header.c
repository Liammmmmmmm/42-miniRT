/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png_header.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:01:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 16:23:15 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	get_png_header(t_bin *bin, size_t *i)
{
	uint64_t	signature;

	if (read_uint64_move_little(bin, i, &signature) == -1)
		return (print_err_png("Error reading the png file"));
	if (signature != PNG_SIGNATURE)
		return (print_err_png("Wrong png signature"));	
	return (0);
}

static int	free_chunk_ret(t_png_chunk *png_chunk)
{
	free(png_chunk->data);
	return (-1);
}

int	read_first_png_chunk(t_bin *bin, size_t *i, t_png_info *png)
{
	t_png_chunk	png_chunk;
	t_bin		chunk_data;
	size_t		y;

	y = 0;
	if (get_png_chunk(bin, i, &png_chunk) == -1)
		return (-1);
	chunk_data.size = png_chunk.data_length;
	chunk_data.data = png_chunk.data;
	if (!is_chunk_name(png_chunk.type_c, "IHDR"))
		return (free_chunk_ret(&png_chunk));
	if (read_uint32_move(&chunk_data, &y, &png->width) == -1)
		return (free_chunk_ret(&png_chunk));
	if (read_uint32_move(&chunk_data, &y, &png->height) == -1)
		return (free_chunk_ret(&png_chunk));
	if (read_uint8_move(&chunk_data, &y, &png->depth) == -1)
		return (free_chunk_ret(&png_chunk));
	if (read_uint8_move(&chunk_data, &y, &png->color_type) == -1)
		return (free_chunk_ret(&png_chunk));
	if (read_uint8_move(&chunk_data, &y, &png->compression) == -1)
		return (free_chunk_ret(&png_chunk));
	if (read_uint8_move(&chunk_data, &y, &png->filter) == -1)
		return (free_chunk_ret(&png_chunk));
	if (read_uint8_move(&chunk_data, &y, &png->interlace) == -1)
		return (free_chunk_ret(&png_chunk));
	free(png_chunk.data);
	return (0);
}

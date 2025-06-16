/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png_idat.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:32:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/16 10:29:50 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	read_idat_chunk(t_bin *data, t_tex_img *img, t_png_info *infos)
{
	t_zlib_block		zlib;
	t_bit_stream	bitstream;

	printf("Debut de la decompression\n");
	if (get_zlib_block(data, &zlib) == -1)
	{
		free(data->data);
		return (-1);
	}
	free(data->data);
	if (check_zlib_flags(&zlib) == -1)
	{
		free(zlib.deflate_data);
		return (-1);
	}
	init_bit_stream(&bitstream, &zlib);
	if (read_deflate_headers(&bitstream, infos) == -1)
	{
		free(zlib.deflate_data);
		return (-1);
	}
	free(zlib.deflate_data);
	return (0);
}

int	ft_islowercase(int c)
{
	return (c >= 'a' && c <= 'z');
}

static int	while_chunks(t_bin *bin, size_t *i, t_png_chunk *png_chunk,
	t_bin *chunk_data)
{
	uint32_t	y;

	if (get_png_chunk(bin, i, png_chunk) == -1)
		return (-1);
	while (is_chunk_name(png_chunk->type_c, "IDAT")
		|| ft_islowercase(png_chunk->type_c[0]))
	{
		if (is_chunk_name(png_chunk->type_c, "IDAT"))
		{
			y = 0;
			while (y < png_chunk->data_length)
				chunk_data->data[chunk_data->size++] = png_chunk->data[y++];
		}
		free(png_chunk->data);
		if (get_png_chunk(bin, i, png_chunk) == -1)
			return (-1);
	}
	free(png_chunk->data);
	return (0);
}

static int	free_data_ret(t_bin *bin)
{
	free(bin->data);
	return (-1);
}

int	read_idat_chunks(t_bin *bin, size_t *i, t_tex_img *img, t_png_info *infos)
{
	t_png_chunk	png_chunk;
	t_bin		chunk_data;

	chunk_data.data = malloc(bin->size * sizeof(t_uchar));
	if (!chunk_data.data)
		return (-1);
	chunk_data.size = 0;
	if (while_chunks(bin, i, &png_chunk, &chunk_data) == -1)
		return (free_data_ret(&chunk_data));
	if (chunk_data.data == NULL)
		return (print_err_png(PNG_ERROR_CHUNK_DATA2));
	if (read_idat_chunk(&chunk_data, img, infos) == -1)
		return (print_err_png(PNG_ERROR_CHUNK_DATA2));
	if (!is_chunk_name(png_chunk.type_c, "IEND"))
		return (print_err_png(PNG_ERROR_UNSUPORTED_CHUNK));
	return (0);
}

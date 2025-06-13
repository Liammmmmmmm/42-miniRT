/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png_idat.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:32:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 16:45:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	read_idat_chunk(t_bin *data, t_tex_img *img, t_png_info *infos)
{
	t_zlib_block		zlib;
	t_png_bit_stream	bitstream;

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
	while (is_chunk_name(png_chunk->type_c, "IDAT")
		|| ft_islowercase(png_chunk->type_c[0]))
	{
		if (is_chunk_name(png_chunk->type_c, "IDAT"))
		{
			*chunk_data = join_bin(*chunk_data, (t_bin){.data = png_chunk->data,
					.size = png_chunk->data_length});
			if (chunk_data->data == NULL)
				return (print_err_png(PNG_ERROR_MEM));
		}
		else
			free(png_chunk->data);
		if (get_png_chunk(bin, i, png_chunk) == -1)
			return (-1);
	}
	return (0);
}

int	read_idat_chunks(t_bin *bin, size_t *i, t_tex_img *img, t_png_info *infos)
{
	t_png_chunk	png_chunk;
	t_bin		chunk_data;

	chunk_data.data = NULL;
	chunk_data.size = 0;
	if (get_png_chunk(bin, i, &png_chunk) == -1)
		return (-1);
	if (while_chunks(bin, i, &png_chunk, &chunk_data) == -1)
		return (-1);
	if (chunk_data.data == NULL)
	{
		free(png_chunk.data);
		return (print_err_png(PNG_ERROR_CHUNK_DATA2));
	}
	if (read_idat_chunk(&chunk_data, img, infos) == -1)
		return (print_err_png(PNG_ERROR_CHUNK_DATA2));
	if (!is_chunk_name(png_chunk.type_c, "IEND"))
	{
		free(png_chunk.data);
		return (print_err_png(PNG_ERROR_UNSUPORTED_CHUNK));
	}
	free(png_chunk.data);
	return (0);
}

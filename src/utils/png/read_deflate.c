/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_deflate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 11:05:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 11:06:25 by lilefebv         ###   ########lyon.fr   */
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

static int	free_tables_and_data(char *err, t_huffman_lookup_entry *table_lit,
	t_huffman_lookup_entry *table_dist, t_uchar *data)
{
	free(table_lit);
	free(table_dist);
	free(data);
	return (print_err_png(err));
}

static int	reading_loop(t_bit_stream *stream,
	t_huffman_lookup_entry *table_lit, t_huffman_lookup_entry *table_dist,
	t_bin_cursor *out_buf)
{
	uint8_t																bfinal;
	uint8_t																btype;

	bfinal = 0;
	while (bfinal == 0)
	{
		bfinal = read_bits(stream, 1);
		btype = read_bits(stream, 2);
		if (btype == 0)
			printf("[UNSUPPORTED] uncompressed bitstream\n");
		else if (btype == 1)
			printf("[UNSUPPORTED] fixed huffman block\n");
		else if (btype == 2)
		{
			if (handle_dynamic_huffman_block(stream, out_buf, table_lit,
					table_dist) == -1)
				return (free_tables_and_data(PNG_ERROR_MEM, table_lit,
						table_dist, out_buf->data.data));
		}
		else
			return (free_tables_and_data(PNG_ERROR_MEM, table_lit, table_dist,
					out_buf->data.data));
	}
	return (0);
}

t_rgba	*decoded_to_rgba(t_png_info *infos, t_bin_cursor *out_buf)
{
	const int		bpp = get_bytes_per_pixel(infos->color_type, infos->depth);
	const uint32_t	stride = 1 + infos->width * bpp;
	t_rgba			*res;

	if (bpp == -1)
	{
		free(out_buf->data.data);
		print_err_png("Unsupported color type or bit depth");
		return (NULL);
	}
	apply_png_filter(out_buf->data.data, infos, bpp, stride);
	res = extract_rgba(out_buf->data.data, infos, bpp, stride);
	free(out_buf->data.data);
	if (!res)
		print_err_png(PNG_ERROR_MEM);
	return (res);
}

int	read_deflate_headers(t_bit_stream *stream, t_png_info *infos,
	t_tex_img *img)
{
	t_bin_cursor			out_buf;
	t_huffman_lookup_entry	*table_lit;
	t_huffman_lookup_entry	*table_dist;
	const int				bytes_per_line = \
	calculate_scanline_bytes(infos->width, infos->depth, infos->color_type);

	if (bytes_per_line < 0)
		return (print_err_png("Unsupported color type"));
	out_buf.cursor = 0;
	out_buf.data.size = infos->height * (1 + bytes_per_line);
	out_buf.data.data = malloc(infos->height * (1 + bytes_per_line));
	table_lit = malloc((1 << MAX_LOOKUP_BITS) * sizeof(t_huffman_lookup_entry));
	table_dist = malloc((1 << MAX_LOOKUP_BITS) * sizeof(t_huffman_lookup_entry))
		;
	if (!table_lit || !table_dist || !out_buf.data.data)
		return (free_tables_and_data(PNG_ERROR_MEM, table_lit, table_dist,
				out_buf.data.data));
	if (reading_loop(stream, table_lit, table_dist, &out_buf) == -1)
		return (-1);
	free(table_lit);
	free(table_dist);
	img->rgba = decoded_to_rgba(infos, &out_buf);
	if (!img->rgba)
		return (-1);
	return (0);
}

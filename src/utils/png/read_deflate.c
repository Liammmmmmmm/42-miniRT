/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_deflate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 11:05:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/16 18:27:07 by lilefebv         ###   ########lyon.fr   */
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

int	calculate_scanline_bytes(uint32_t width, uint8_t bit_depth, uint8_t color_type)
{
	int channels;

	if (color_type == 0)
		channels = 1;
	else if (color_type == 2)
		channels = 3;
	else if (color_type == 3)
		channels = 1;
	else if (color_type == 4)
		channels = 2;
	else if (color_type == 6)
		channels = 4;
	else
		return (-1);

	int bits_per_pixel = channels * bit_depth;
	return (bits_per_pixel * width + 7) / 8;
}

int	handle_dynamic_huffman_block(t_bit_stream *stream, t_png_info *infos, t_bin *out_buf, size_t *output_len, t_huffman_lookup_entry *table_lit, t_huffman_lookup_entry *table_dist)
{
	const uint32_t			hlit = read_bits(stream, 5) + 257;
	const uint32_t			hdist = read_bits(stream, 5) + 1;
	const uint32_t			hclen = read_bits(stream, 4) + 4;
	uint8_t					code_length_codes[19];
	uint16_t				code_length_huffman_codes[19];

	//printf("HLIT=%u HDIST=%u HCLEN=%u\n", hlit, hdist, hclen);
	read_code_lengh(code_length_codes, hclen, stream);
	generate_huffman_codes(code_length_codes, code_length_huffman_codes, 19, MAX_CODE_LENGTH_TABLE);
	
	t_huffman_data	hclen_data = (t_huffman_data){
		.code_lengths = code_length_codes,
		.codes = code_length_huffman_codes,
		.num_symbols = 19,
		.table = NULL
	};

	uint8_t	all_code_lengths[hlit + hdist]; // Je ferais un joli malloc quand j'aurais la foie

	if (decode_code_lengths(all_code_lengths, hlit + hdist, &hclen_data, stream) == -1)
		return print_err_png("Failed to decode code lengths");

	uint16_t codes_litlen[hlit]; // Pareil je ferais les malloc quand j'aurais la foie
    uint16_t codes_dist[hdist];

	t_huffman_table huffman_table;

	huffman_table.hlit = (t_huffman_data){
		.code_lengths = all_code_lengths,
		.codes = codes_litlen,
		.num_symbols = hlit,
		.table = table_lit
	};
	
	huffman_table.hdist = (t_huffman_data){
		.code_lengths = all_code_lengths + hlit,
		.codes = codes_dist,
		.num_symbols = hdist,
		.table = table_dist
	};

	generate_huffman_codes(all_code_lengths, codes_litlen, hlit, MAX_CODE_LENGTH_DATA);
    generate_huffman_codes(all_code_lengths + hlit, codes_dist, hdist, MAX_CODE_LENGTH_DATA);
	build_huffman_lookup_table(&huffman_table.hlit);
	build_huffman_lookup_table(&huffman_table.hdist);
	if (decompress_huffman_data(stream, &huffman_table, out_buf,
			output_len) == -1)
		return (print_err_png("Failed to decompress Huffman data"));
	
	return (0);
}

int	get_bytes_per_pixel(uint8_t color_type, uint8_t bit_depth)
{
	if (bit_depth != 8)
		return (-1);
	if (color_type == 0)
		return (1);
	else if (color_type == 2)
		return (3);
	else if (color_type == 3)
		return (1);
	else if (color_type == 4)
		return (2);
	else if (color_type == 6)
		return (4);
	else
		return (-1);
}

void	apply_png_filter(uint8_t *image_data, uint32_t width, uint32_t height,
	uint8_t bpp, uint32_t stride)
{
	uint32_t	y;

	y = (uint32_t)(-1);
	while (++y < height)
	{
		uint8_t *scanline = image_data + y * stride;
		uint8_t filter_type = scanline[0];
		uint8_t *cur = scanline + 1;
		uint8_t *prev = (y == 0) ? NULL : (scanline - stride + 1);

		if (filter_type == 0)
			continue ;
		else if (filter_type == 1)
			png_filter_sub(cur, width * bpp, bpp);
		else if (filter_type == 2)
			png_filter_up(cur, prev, width * bpp);
		else if (filter_type == 3)
			png_filter_average(cur, prev, width * bpp, bpp);
		else if (filter_type == 4)
			png_filter_paeth(cur, prev, width * bpp, bpp);
		else
			printf("Invalid png filter type: %d\n", filter_type);
	}
}

#include <stdio.h>

int export_ppm(const char *filename, const t_rgba *pixels, uint32_t width, uint32_t height)
{
    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        perror("fopen");
        return -1;
    }

    // En-tête PPM binaire
    fprintf(f, "P6\n%u %u\n255\n", width, height);

    // Écriture des pixels : r,g,b (ignore alpha)
    for (uint32_t i = 0; i < width * height; i++)
    {
        fputc(pixels[i].r, f);
        fputc(pixels[i].g, f);
        fputc(pixels[i].b, f);
    }

    fclose(f);
    return 0;
}


t_rgba *extract_rgba(const uint8_t *image_data, uint32_t width, uint32_t height,
	uint8_t bpp, uint32_t stride)
{
	if (bpp == 3)
		printf("Bpp 3 -> On gere");
	else if (bpp == 4)
		printf("Bpp 4 -> On gere");
	else
	{
		fprintf(stderr, "extract_rgba: unsupported bpp %d\n", bpp);
		return NULL;
	}

	t_rgba *out = malloc(width * height * sizeof(t_rgba));
	if (!out)
	{
		perror("malloc");
		return NULL;
	}

	for (uint32_t y = 0; y < height; y++)
	{
		const uint8_t *scanline = image_data + y * stride;
		const uint8_t *pixels = scanline + 1;  // Skip filter byte
		for (uint32_t x = 0; x < width; x++)
		{
			out[y * width + x].r = pixels[x * bpp + 0];
			out[y * width + x].g = pixels[x * bpp + 1];
			out[y * width + x].b = pixels[x * bpp + 2];
			if (bpp == 4)
				out[y * width + x].a = pixels[x * bpp + 3];
			else
				out[y * width + x].a = 255;
		}
	}
	return out;
}



int	read_deflate_headers(t_bit_stream *stream, t_png_info *infos)
{
	uint8_t	bfinal;
	uint8_t	btype;
	t_bin	out_buf;
	
	t_huffman_lookup_entry *table_lit = malloc((1 << MAX_LOOKUP_BITS) * sizeof(t_huffman_lookup_entry));
	t_huffman_lookup_entry *table_dist = malloc((1 << MAX_LOOKUP_BITS) * sizeof(t_huffman_lookup_entry));

	int	bytes_per_line = calculate_scanline_bytes(infos->width, infos->depth, infos->color_type);
	if (bytes_per_line < 0)
		return (print_err_png("Unsupported color type"));

	out_buf.size = infos->height * (1 + bytes_per_line);
	out_buf.data = malloc(infos->height * (1 + bytes_per_line));
	if (!out_buf.data)
		return (print_err_png("Memory allocation failed"));

	size_t	output_len = 0;

	bfinal = 0;
	while (bfinal == 0)
	{
		bfinal = read_bits(stream, 1);
		btype = read_bits(stream, 2);
		//printf("Read block : %d %d\n", bfinal, btype);
		if (btype == 0)
			printf("[UNSUPPORTED FOR NOW] uncompressed bitstream\n");
		else if (btype == 1)
			printf("[UNSUPPORTED FOR NOW] fixed huffman block\n");
		else if (btype == 2)
		{
			if (handle_dynamic_huffman_block(stream, infos, &out_buf, &output_len, table_lit, table_dist) == -1)
			{
				free(out_buf.data);
				free(table_lit);
				free(table_dist);
				return (print_err_png("failed to read dynamic huffman block"));
			}
		}
		else
		{
			free(out_buf.data);
			free(table_lit);
			free(table_dist);
			return (print_err_png("Invalid BTYPE"));
		}
	}

	free(table_lit);
	free(table_dist);

	int bpp = get_bytes_per_pixel(infos->color_type, infos->depth);
	if (bpp == -1)
		return print_err_png("Unsupported color type or bit depth");	

	uint32_t stride = 1 + infos->width * bpp;
	apply_png_filter(out_buf.data, infos->width, infos->height, bpp, stride);


	t_rgba *res = extract_rgba(out_buf.data, infos->width, infos->height, bpp, stride);
	
	if (res)
		export_ppm("test.ppm", res, infos->width, infos->height);

	return (0);
}

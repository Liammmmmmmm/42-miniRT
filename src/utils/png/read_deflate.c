/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_deflate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 11:05:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 16:47:53 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

/**
 * This file contains the knowledge of god.
 * Dont ask me to explain anything here, I have now idea of what I'm actually
 * doing and the few things I understand will vanish of my head in a few hours.
 * Good Luck brave warrior.
 */

void	read_code_lengh(uint8_t *code_length_codes, const uint32_t hclen,
	t_png_bit_stream *stream)
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

void	count_code_lengths(const uint8_t *code_lengths, size_t num_symbols,
	uint16_t *bl_count, size_t max_bits)
{
	size_t	i;

	i = 0;
	while (i < num_symbols)
	{
		if (code_lengths[i] > 0)
		{
			if (code_lengths[i] > max_bits)
			{
				printf("Invalid code length: %u\n", code_lengths[i]);
				continue ;
			}
			bl_count[code_lengths[i]]++;
		}
		i++;
	}
}

void generate_huffman_codes(const uint8_t *code_lengths, uint16_t *codes,
	size_t num_symbols, size_t max_bits)
{
	uint16_t	bl_count[16];
	uint16_t	next_code[16];
	uint16_t	code;
	size_t		i;
	uint8_t		len;

	ft_bzero(bl_count, sizeof(uint16_t) * 16);
	ft_bzero(next_code, sizeof(uint16_t) * 16);
	count_code_lengths(code_lengths, num_symbols, bl_count, max_bits);
	i = 1;
	code = 0;
	while (i <= max_bits)
	{
		code = (code + bl_count[i - 1]) << 1;
		next_code[i] = code;
		i++;
	}
	i = 0;
	while (i < num_symbols)
	{
		len = code_lengths[i];
		if (len)
			codes[i] = next_code[len]++;
		i++;
	}
}

int	decode_symbol(const uint8_t *code_lengths, const uint16_t *codes, size_t num_symbols, t_png_bit_stream *stream, uint8_t max_code_length)
{
	uint16_t	code;
	uint8_t		length;
	uint8_t		bit;
	size_t		i;

	length = 0;
	code = 0;
	while (++length <= max_code_length)
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

int decode_code_lengths(uint8_t *code_lengths, size_t total_lengths,
	const uint8_t *code_length_codes,
	const uint16_t *code_length_huffman_codes,
	t_png_bit_stream *stream)
{
	size_t	i;
	uint8_t	prev_length;
	int		symbol;
	int		repeat;
	int		j;

	i = 0;
	prev_length = 0;
	while (i < total_lengths)
	{
		symbol = decode_symbol(code_length_codes, code_length_huffman_codes, 19, stream, 7);
		if (symbol < 0)
			return (-1);
		if (symbol <= 15)
		{
			code_lengths[i++] = (uint8_t)symbol;
			prev_length = (uint8_t)symbol;
		}
		else if (symbol == 16)
		{
			if (i == 0)
				return (-1);
			repeat = read_bits(stream, 2) + 3;
			if (i + repeat > total_lengths)
				return (-1);
			j = -1;
			while (++j < repeat)
				code_lengths[i++] = prev_length;
		}
		else if (symbol == 17)
		{
			repeat = read_bits(stream, 3) + 3;
			if (i + repeat > total_lengths)
				return (-1);
			j = -1;
			while (++j < repeat)
				code_lengths[i++] = 0;
			prev_length = 0;
		}
		else if (symbol == 18)
		{
			repeat = read_bits(stream, 7) + 11;
			if (i + repeat > total_lengths)
				return (-1);
			j = -1;
			while (++j < repeat)
				code_lengths[i++] = 0;
			prev_length = 0;
		}
		else
			return (-1);
	}
	return (0);
}

uint32_t		decode_length(int sym, t_png_bit_stream *stream)
{
	static const uint16_t	length_base[29] = {
		3, 4, 5, 6, 7, 8, 9, 10,
		11, 13, 15, 17,
		19, 23, 27, 31,
		35, 43, 51, 59,
		67, 83, 99, 115,
		131, 163, 195, 227, 258
	};
	static const uint8_t	length_extra_bits[29] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3,
		4, 4, 4, 4,
		5, 5, 5, 5, 0
	};

	if (sym < 257 || sym > 285)
	{
		printf("Invalid length symbol: %d\n", sym);
		return (0);
	}
	int index = sym - 257;
	uint32_t base = length_base[index];
	uint8_t extra = length_extra_bits[index];
	if (extra == 0)
		return (base);
	uint32_t extra_bits = read_bits(stream, extra);
	return (base + extra_bits);
}

uint32_t decode_distance(int sym, t_png_bit_stream *stream)
{
	static const uint16_t	distance_base[30] = {
		1, 2, 3, 4, 5, 7, 9, 13,
		17, 25, 33, 49, 65, 97, 129, 193,
		257, 385, 513, 769, 1025, 1537, 2049, 3073,
		4097, 6145, 8193, 12289, 16385, 24577
	};
	static const uint8_t	distance_extra_bits[30] = {
		0, 0, 0, 0, 1, 1, 2, 2,
		3, 3, 4, 4, 5, 5, 6, 6,
		7, 7, 8, 8, 9, 9, 10, 10,
		11, 11, 12, 12, 13, 13
	};

	if (sym < 0 || sym > 29)
	{
		printf("Invalid distance symbol: %d\n", sym);
		return (0);
	}
	uint32_t base = distance_base[sym];
	uint8_t extra = distance_extra_bits[sym];
	if (extra == 0)
		return (base);
	uint32_t extra_bits = read_bits(stream, extra);
	return (base + extra_bits);
}

int decompress_huffman_data(t_png_bit_stream *stream,
	const uint8_t *litlen_lengths, const uint16_t *litlen_codes, size_t hlit,
	const uint8_t *dist_lengths, const uint16_t *dist_codes, size_t hdist,
	uint8_t *out, size_t *out_len)
{
	size_t		o;
	int			sym;
	int			dist_sym;
	uint32_t	length;
	uint32_t	distance;
	uint32_t	i;

	o = *out_len;
	while (1)
	{
		sym = decode_symbol(litlen_lengths, litlen_codes, hlit, stream, 15);
		if (sym < 0)
			return (print_err_png("Invalid symbol"));
		if (sym < 256)
			out[o++] = (uint8_t)sym;
		else if (sym == 256)
			break ;
		else if (sym <= 285)
		{
			length = decode_length(sym, stream);
			dist_sym = decode_symbol(dist_lengths, dist_codes, hdist, stream, 15);
			if (dist_sym < 0)
				return (print_err_png("Invalid dist symbol"));
			distance = decode_distance(dist_sym, stream);
			if (distance > o)
				return (print_err_png("Invalid distance"));
			i = 0;
			while (i < length)
			{
				uint8_t c = out[o - distance];
				out[o] = c;
				o++;
				i++;
			}
		}
		else
			return (print_err_png("Symbol inconnu"));
	}
	*out_len = o;
	return 0;
}

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

int	handle_dynamic_huffman_block(t_png_bit_stream *stream, t_png_info *infos, uint8_t *out_buf, size_t *output_len)
{
	const uint32_t			hlit = read_bits(stream, 5) + 257;
	const uint32_t			hdist = read_bits(stream, 5) + 1;
	const uint32_t			hclen = read_bits(stream, 4) + 4;
	uint8_t					code_length_codes[19];
	uint16_t				code_length_huffman_codes[19];

	//printf("HLIT=%u HDIST=%u HCLEN=%u\n", hlit, hdist, hclen);
	read_code_lengh(code_length_codes, hclen, stream);
	generate_huffman_codes(code_length_codes, code_length_huffman_codes, 19, 7);
	
	uint8_t	all_code_lengths[hlit + hdist]; // Je ferais un joli malloc quand j'aurais la foie

	if (decode_code_lengths(all_code_lengths, hlit + hdist, code_length_codes, code_length_huffman_codes, stream) == -1)
		return print_err_png("Failed to decode code lengths");

	uint16_t codes_litlen[hlit]; // Pareil je ferais les malloc quand j'aurais la foie
    uint16_t codes_dist[hdist];

	generate_huffman_codes(all_code_lengths, codes_litlen, hlit, 15);
    generate_huffman_codes(all_code_lengths + hlit, codes_dist, hdist, 15);

	if (decompress_huffman_data(stream, all_code_lengths, codes_litlen, hlit,
		all_code_lengths + hlit, codes_dist, hdist, out_buf, output_len) == -1)
		return print_err_png("Failed to decompress Huffman data");
	
	return (0);
}

int get_bytes_per_pixel(uint8_t color_type, uint8_t bit_depth)
{
	if (bit_depth != 8)
		return -1;

	switch (color_type)
	{
		case 0: return 1;
		case 2: return 3;
		case 3: return 1;
		case 4: return 2;
		case 6: return 4;
		default: return -1;
	}
}

void apply_png_filter(uint8_t *image_data, uint32_t width, uint32_t height,
	uint8_t bpp, uint32_t stride)
{
	for (uint32_t y = 0; y < height; y++)
	{
		uint8_t *scanline = image_data + y * stride;
		uint8_t filter_type = scanline[0];
		uint8_t *cur = scanline + 1;
		uint8_t *prev = (y == 0) ? NULL : (scanline - stride + 1);

		switch (filter_type)
		{
			case 0: break;
			case 1: png_filter_sub(cur, width * bpp, bpp); break;
			case 2: png_filter_up(cur, prev, width * bpp); break;
			case 3: png_filter_average(cur, prev, width * bpp, bpp); break;
			case 4: png_filter_paeth(cur, prev, width * bpp, bpp); break;
			default: printf("Invalid filter type: %d\n", filter_type); break;
		}
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



int	read_deflate_headers(t_png_bit_stream *stream, t_png_info *infos)
{
	uint8_t	bfinal;
	uint8_t	btype;
	uint8_t	*out_buf;

	int	bytes_per_line = calculate_scanline_bytes(infos->width, infos->depth, infos->color_type);
	if (bytes_per_line < 0)
		return print_err_png("Unsupported color type");

	size_t	out_buf_size = infos->height * (1 + bytes_per_line);
	out_buf = malloc(out_buf_size);
	if (!out_buf)
		return print_err_png("Memory allocation failed");

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
			if (handle_dynamic_huffman_block(stream, infos, out_buf, &output_len) == -1)
			{
				free(out_buf);
				return (print_err_png("failed to read dynamic huffman block"));
			}
		}
		else
		{
			free(out_buf);
			return (print_err_png("Invalid BTYPE"));
		}
	}

	int bpp = get_bytes_per_pixel(infos->color_type, infos->depth);
	if (bpp == -1)
		return print_err_png("Unsupported color type or bit depth");	

	uint32_t stride = 1 + infos->width * bpp;
	apply_png_filter(out_buf, infos->width, infos->height, bpp, stride);


	t_rgba *res = extract_rgba(out_buf, infos->width, infos->height, bpp, stride);
	
	if (res)
		export_ppm("test.ppm", res, infos->width, infos->height);

	return (0);
}

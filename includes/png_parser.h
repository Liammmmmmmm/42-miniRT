/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:45:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 16:48:18 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PNG_PARSER_H
# define PNG_PARSER_H

# include "structs.h"
# include "libft.h"

# define PNG_SIGNATURE 727905341920923785

# define PNG_ERROR_GLOBAL "Error reading the png file"
# define PNG_ERROR_CHUNK_DATA "Error reading the png chunk data. Is the file corrupted ?"
# define PNG_ERROR_CHUNK_DATA2 "Error reading the png chunk data."
# define PNG_ERROR_MEM "Memory allocation error"
# define PNG_ERROR_GET_ZLIB "Error while reading a zlib chunk"
# define PNG_ERROR_GET_IHDR "Error while reading the IHDR chunk"
# define PNG_ERROR_UNSUPORTED_CHUNK "Unsuported png format"

typedef struct s_png_info
{
	uint32_t	width;
	uint32_t	height;
	uint8_t		depth;
	uint8_t		color_type;
	uint8_t		compression;
	uint8_t		filter;
	uint8_t		interlace;
}	t_png_info;

typedef struct s_png_chunk
{
	uint32_t	data_length;
	union
	{
		char		type_c[4];
		uint32_t	type;
	};
	uint8_t		*data;
	uint32_t	crc32;
}	t_png_chunk;

typedef struct s_zlib_block
{
	uint8_t		cmf;
	uint8_t		flg;
	uint32_t	deflate_size;
	uint8_t		*deflate_data;
	uint32_t	adler32;
}	t_zlib_block;

typedef struct s_png_bit_stream {
	uint8_t		*data_stream;
	size_t		size;
	size_t		byte_index;
	uint32_t	bit_buffer;
	uint8_t		bits_remaining;
}	t_png_bit_stream;


void	png_filter_paeth(uint8_t *data, const uint8_t *prev, size_t len,
	uint8_t bpp);
void	png_filter_average(uint8_t *data, const uint8_t *prev, size_t len,
	uint8_t bpp);
void	png_filter_up(uint8_t *data, const uint8_t *prev, size_t len);
void	png_filter_sub(uint8_t *data, size_t len, uint8_t bpp);

void	print_ihdr_chunk(t_png_info *png);
void	debug_print_all_code_lengths(const uint8_t *all_code_lengths,
	size_t hlit, size_t hdist);


int	print_err_png(char *err);
int	get_png_header(t_bin *bin, size_t *i);
int	read_first_png_chunk(t_bin *bin, size_t *i, t_png_info *png);
int	get_png_chunk(t_bin *bin, size_t *i, t_png_chunk *chunk);
int	get_zlib_block(t_bin *data, t_zlib_block *zlib_block);
int	check_zlib_flags(t_zlib_block *zlib_block);
int	is_chunk_name(char c[4], char t[4]);
int	read_idat_chunks(t_bin *bin, size_t *i, t_tex_img *img, t_png_info *infos);

void	init_bit_stream(t_png_bit_stream *bitstream, t_zlib_block *zlib);

uint32_t	read_bits(t_png_bit_stream *bits, uint32_t bits_to_read);

int	read_deflate_headers(t_png_bit_stream *stream, t_png_info *infos);

#endif
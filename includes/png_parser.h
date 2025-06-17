/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 16:45:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/17 12:46:20 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PNG_PARSER_H
# define PNG_PARSER_H

# include "structs.h"
# include "libft.h"

# define PNG_SIGNATURE 727905341920923785

# define PNG_ERROR_GLOBAL "Error reading the png file"
# define PNG_ERROR_CHUNK_DATA "Error reading the png chunk data. Is the file \
corrupted ?"
# define PNG_ERROR_CHUNK_DATA2 "Error reading the png chunk data."
# define PNG_ERROR_MEM "Memory allocation error"
# define PNG_ERROR_GET_ZLIB "Error while reading a zlib chunk"
# define PNG_ERROR_GET_IHDR "Error while reading the IHDR chunk"
# define PNG_ERROR_UNSUPORTED_CHUNK "Unsuported png format"

# define MAX_CODE_LENGTH_TABLE 7
# define MAX_CODE_LENGTH_DATA 15

# define MAX_LOOKUP_BITS 12

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

typedef struct s_bit_stream
{
	uint8_t		*data_stream;
	size_t		size;
	size_t		byte_index;
	uint32_t	bit_buffer;
	uint8_t		bits_remaining;
}	t_bit_stream;

typedef struct s_huffman_lookup_entry
{
	int		symbol;
	uint8_t	length;
}	t_huffman_lookup_entry;

typedef struct s_huffman_data
{
	uint8_t					*code_lengths;
	uint16_t				*codes;
	uint32_t				num_symbols;
	t_huffman_lookup_entry	*table;
}	t_huffman_data;

typedef struct s_huffman_table
{
	t_huffman_data	hlit;
	t_huffman_data	hdist;
}	t_huffman_table;

typedef struct s_decode_symbol_data
{
	int			symbol;
	uint32_t	i;
	uint8_t		prev_length;
}	t_decode_symbol_data;

typedef struct s_bin_cursor
{
	t_bin	data;
	size_t	cursor;
}	t_bin_cursor;

typedef struct s_deflate_num_tables
{
	uint32_t				hlit;
	uint32_t				hdist;
	uint32_t				hclen;
	t_huffman_lookup_entry	*table_lit;
	t_huffman_lookup_entry	*table_dist;
}	t_deflate_num_tables;

void		png_filter_paeth(uint8_t *data, const uint8_t *prev, size_t len,
				uint8_t bpp);
void		png_filter_average(uint8_t *data, const uint8_t *prev, size_t len,
				uint8_t bpp);
void		png_filter_up(uint8_t *data, const uint8_t *prev, size_t len);
void		png_filter_sub(uint8_t *data, size_t len, uint8_t bpp);
void		apply_png_filter(uint8_t *image_data, t_png_info *infos,
				uint8_t bpp, uint32_t stride);

void		print_ihdr_chunk(t_png_info *png);
void		debug_print_all_code_lengths(const uint8_t *all_code_lengths,
				size_t hlit, size_t hdist);

void		generate_huffman_codes(const uint8_t *code_lengths, uint16_t *codes,
				uint32_t num_symbols, uint32_t max_bits);
void		build_huffman_lookup_table(t_huffman_data *data);
int			decode_symbol_huffman_table(t_bit_stream *bs, t_huffman_data *data);

int			decode_code_lengths(uint8_t *code_lengths, uint32_t total_lengths,
				t_huffman_data *data, t_bit_stream *stream);
void		read_code_lengh(uint8_t *code_length_codes, const uint32_t hclen,
				t_bit_stream *stream);

uint32_t	decode_distance(int sym, t_bit_stream *stream);
uint32_t	decode_length(int sym, t_bit_stream *stream);
int			decompress_huffman_data(t_bit_stream *stream,
				t_huffman_table *table, t_bin_cursor *out);

int			calculate_scanline_bytes(uint32_t width, uint8_t bit_depth,
				uint8_t color_type);
int			get_bytes_per_pixel(uint8_t color_type, uint8_t bit_depth);

int			print_err_png(char *err);
int			get_png_header(t_bin *bin, size_t *i);
int			read_first_png_chunk(t_bin *bin, size_t *i, t_png_info *png);
int			get_png_chunk(t_bin *bin, size_t *i, t_png_chunk *chunk);
int			get_zlib_block(t_bin *data, t_zlib_block *zlib_block);
int			check_zlib_flags(t_zlib_block *zlib_block);
int			is_chunk_name(char c[4], char t[4]);
int			read_idat_chunks(t_bin *bin, size_t *i, t_tex_img *img,
				t_png_info *infos);
t_rgba		*extract_rgba(uint8_t *image_data, t_png_info *infos,
				uint8_t bpp, uint32_t stride);

/**
 * This function initialize a bitstream struct from a zlib block.
 */
void		init_bit_stream(t_bit_stream *bitstream, t_zlib_block *zlib);

/**
 * This function will return the n next bits of the bitstream, and actually
 * move in the bitstream so that we don't read the sames bits again on the next
 * call
 */
uint32_t	read_bits(t_bit_stream *bits, uint32_t n);

/**
 * This function will return the n next bits of the bitstream like read bits,
 * but without changing the cursor on the bitstream. You read but don't move.
 */
uint32_t	peek_bits(t_bit_stream *bs, uint32_t n);

/**
 * This function is the opposite of peak_bits. This on only change the "cursor"
 * position in the bitstream.
 */
void		consume_bits(t_bit_stream *bs, uint32_t n);

int			read_deflate_headers(t_bit_stream *stream, t_png_info *infos,
				t_tex_img *img);
int			handle_dynamic_huffman_block(t_bit_stream *stream,
				t_bin_cursor *out_buf, t_huffman_lookup_entry *table_lit,
				t_huffman_lookup_entry *table_dist);

int			parse_png(char *filename, t_tex_img *img);

#endif
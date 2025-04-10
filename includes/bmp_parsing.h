/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:42:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/10 17:56:35 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	BMP_PARSING
# define BMP_PARSING

typedef struct s_bmp_header
{
	union
	{
		char		signature_c[2];
		uint16_t	signature;
	};
	uint32_t		file_size;
	uint16_t		reserved1;
	uint16_t		reserved2;
	uint32_t		pixel_data_offset;
}	t_bmp_header;

typedef struct s_bmp_info
{
	uint32_t		info_header_size;
	uint32_t		with;
	uint32_t		height;
	uint16_t		nb_planes;
	uint16_t		bpp;
	uint32_t		compression;
	uint32_t		image_size;
	uint32_t		x_px_m;
	uint32_t		y_px_m;
	uint32_t		nb_color;
	uint32_t		nb_important_colors;
}	t_bmp_info;

typedef struct s_bmp
{
	t_bmp_header	header;
	t_bmp_info		info;
	uint32_t		*pixel_data;
	uint32_t		*palette;  
}	t_bmp;

int	bmp_manager(t_bmp *bmp, char *file);
int	error_and_return(char *message);
int	extract_palette_pixel(t_bin *bin, size_t *i, uint32_t *value);
int	extract_16bpp_pixel(t_bin *bin, size_t *i, uint32_t *value);
int	extract_24bpp_pixel(t_bin *bin, size_t *i, uint32_t *value);
int	extract_32bpp_pixel(t_bin *bin, size_t *i, uint32_t *value);
int	extract_pixel(t_bin *bin, size_t *i, t_bmp *bmp, uint32_t *value);
int	error_and_return(char *message);
int	read_palette(t_bin* bin, size_t *i, t_bmp *bmp, size_t palette_entry_count);
int	read_header(t_bmp *bmp, size_t *i, t_bin *bin);
int	read_info(t_bmp *bmp, size_t *i, t_bin *bin);
int	extract_raw_pixels(t_bin *bin, size_t *i, t_bmp *bmp);

#endif
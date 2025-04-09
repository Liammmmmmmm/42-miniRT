/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_parsing.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:42:24 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/09 18:19:21 by madelvin         ###   ########.fr       */
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
	uint8_t			*pixel_data;
	uint32_t		*palette;  
}	t_bmp;

void	bmp_manager();

#endif
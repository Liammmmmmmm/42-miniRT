/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:31:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/09 18:30:03 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bmp_parsing.h"
#include "stdio.h"

void read_palette(t_bin* bin, size_t *i, t_bmp *bmp)
{
	size_t palette_size;
	size_t j;

	j = 0;
	if (bmp->info.bpp == 1)
		palette_size = 2 * sizeof(uint32_t);
	else if (bmp->info.bpp == 4)
		palette_size = 16 * sizeof(uint32_t);
	else if (bmp->info.bpp == 8)
		palette_size = 256 * sizeof(uint32_t);
	else
	{
		printf("Error: Unsupported BPP for palette\n");
		return;
	}
	bmp->palette = malloc(palette_size);
	if (bmp->palette == NULL)
	{
		printf("Error: unable to allocate memory for palette\n");
		return;
	}
	while (*i < palette_size)
	{
		if (read_uint32_move_little(bin, i, &bmp->palette[j]))
			printf("error: unable to read color\n");
		j++;
	}
}

void	bmp_manager()
{
	t_bmp	bmp;
	t_bin	bin;
	size_t	i;

	i = 0;
	if (read_bin_file(&bin, "assets/texture/test_simple.bmp") == 0)
	{
		printf("error: unable to read BMP file\n");
		return ;
	}
	if (read_uint16_move_little(&bin, &i, &bmp.header.signature) == -1)
		printf("error: unable to read signature\n");
	if (read_uint32_move_little(&bin, &i, &bmp.header.file_size) == -1)
		printf("error: unable to read file size\n");
	if (read_uint16_move_little(&bin, &i, &bmp.header.reserved1) == -1)
		printf("error: unable to read reserved1\n");
	if (read_uint16_move_little(&bin, &i, &bmp.header.reserved2) == -1)
		printf("error: unable to read reserved2\n");
	if (read_uint32_move_little(&bin, &i, &bmp.header.pixel_data_offset) == -1)
		printf("error: unable to read pixel data offset\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.info_header_size) == -1)
		printf("error: unable to read info header size\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.with) == -1)
		printf("error: unable to read width\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.height) == -1)
		printf("error: unable to read height\n");
	if (read_uint16_move_little(&bin, &i, &bmp.info.nb_planes) == -1)
		printf("error: unable to read nb_planes\n");
	if (read_uint16_move_little(&bin, &i, &bmp.info.bpp) == -1)
		printf("error: unable to read bpp\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.compression) == -1)
		printf("error: unable to read compression\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.image_size) == -1)
		printf("error: unable to read image size\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.x_px_m) == -1)
		printf("error: unable to read x_px_m\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.y_px_m) == -1)
		printf("error: unable to read y_px_m\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.nb_color) == -1)
		printf("error: unable to read nb_color\n");
	if (read_uint32_move_little(&bin, &i, &bmp.info.nb_important_colors) == -1)
		printf("error: unable to read nb_important_colors\n");
	if (bmp.info.bpp == 1 || bmp.info.bpp == 4 || bmp.info.bpp == 8)
	{
		read_palette(&bin, &i, &bmp);
		for (unsigned int j = 0; j < 16; ++j)
		{
			uint8_t blue = bmp.palette[j] & 0xFF;
			uint8_t green = (bmp.palette[j] >> 8) & 0xFF;
			uint8_t red = (bmp.palette[j] >> 16) & 0xFF;
			uint8_t alpha = (bmp.palette[j] >> 24) & 0xFF;   
			printf("Color %d - R: %d, G: %d, B: %d, A: %d\n", j, red, green, blue, alpha);
		}
	}
		// if (bmp.palette != NULL)
	// 	free(bmp.palette);
	// free(bmp.pixel_data);
}

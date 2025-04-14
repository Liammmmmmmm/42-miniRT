/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:31:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/14 10:23:41 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "bmp_parsing.h"
#include "stdio.h"

int	extract_bmp_data(t_bmp *bmp, char *file, t_bin *bin, size_t *i)
{
	size_t		palette_bytes;
	uint32_t	palette_entry_count;

	if (read_bin_file(bin, file) == 0)
		return (error_and_return("error: unable to read BMP file\n"));
	printf("%zu \n", bin->size);
	if (read_header(bmp, i, bin) == 1 || read_info(bmp, i, bin) == 1)
		return (1);
	if (bmp->info.bpp == 1 || bmp->info.bpp == 4 || bmp->info.bpp == 8)
	{
		palette_entry_count = bmp->info.nb_color;
		if (bmp->info.nb_color == 0 && (bmp->info.bpp == 1 || \
				bmp->info.bpp == 4 || bmp->info.bpp == 8))
			palette_entry_count = 1 << bmp->info.bpp;
		palette_bytes = palette_entry_count * sizeof(uint32_t);
		if (palette_entry_count > 0 && bmp->header.pixel_data_offset >= \
				*i + palette_bytes)
			read_palette(bin, i, bmp, palette_entry_count);
		else
			return (error_and_return("error: unable to read color palette\n"));
	}
	return (0);
}

int	init_data(t_bin *bin, t_bmp *bmp, size_t *i, char *file)
{
	*i = 0;
	bmp->palette = NULL;
	bin->data = NULL;
	if (extract_bmp_data(bmp, file, bin, i) == 1)
	{
		if (!bin->data)
			free(bin->data);
		return (1);
	}
	return (0);
}

int	parse_bmp(char *file, t_tex_img *img)
{
	t_bmp 		bmp;
	t_bin		bin;
	size_t		i;

	if (init_data(&bin, &bmp, &i, file) == 1)
		return (1);
	if (extract_raw_pixels(&bin, &i, &bmp) == 1)
	{
		free(bin.data);
		if (bmp.palette != NULL)
			free(bmp.palette);
		return (1);
	}
	free(bin.data);
	if (bmp.palette != NULL)
		free(bmp.palette);
	img->height = bmp.info.height;
	img->width = bmp.info.width;
	img->pixel_data = bmp.pixel_data;	
	return (0);
}

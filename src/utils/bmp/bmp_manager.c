/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:31:04 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/10 19:15:19 by madelvin         ###   ########.fr       */
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
		return(error_and_return("error: unable to read BMP file\n"));
	if (read_header(bmp, i, bin) == 1 || read_info(bmp, i, bin) == 1)
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

int	bmp_manager(t_bmp *bmp, char *file)
{
	t_bin		bin;
	size_t		i;

	i = 0;
	bmp->palette = NULL;
	bin.data = NULL;
	if (extract_bmp_data(bmp, file, &bin, &i) == 1)
	{
		free(bin.data);
		return (1);
	}
	if (extract_raw_pixels(&bin, &i, bmp) == 1)
	{
		free(bin.data);
		if (bmp->palette != NULL)
			free(bmp->palette);
		return (1);
	}
	free(bin.data);
	if (bmp->palette != NULL)
		free(bmp->palette);
	return (0);
}

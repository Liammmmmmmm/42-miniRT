/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_extract_header.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:51:35 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/11 15:22:12 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "bmp_parsing.h"
#include <stdio.h>

int	error_and_return(char *message)
{
	ft_dprintf(2, "%s", message);
	return (1);
}

int	read_palette(t_bin *bin, size_t *i, t_bmp *bmp, size_t palette_entry_count)
{
	size_t	j;
	size_t	palette_size;

	j = 0;
	palette_size = palette_entry_count * sizeof(uint32_t);
	bmp->palette = malloc(palette_size);
	if (bmp->palette == NULL)
	{
		return (\
		error_and_return ("Error: unable to allocate memory for palette\n"));
	}
	while (j < palette_entry_count && *i < bin->size)
	{
		if (read_uint32_move_little(bin, i, &bmp->palette[j]))
		{
			free(bmp->palette);
			return (error_and_return ("error: unable to read color\n"));
		}
		j++;
	}
	return (0);
}

int	read_header(t_bmp *bmp, size_t *i, t_bin *bin)
{
	if (read_uint16_move_little(bin, i, &bmp->header.signature) == -1)
		return (error_and_return ("error: unable to read signature\n"));
	if (read_uint32_move_little(bin, i, &bmp->header.file_size) == -1)
		return (error_and_return ("error: unable to read file size\n"));
	if (read_uint16_move_little(bin, i, &bmp->header.reserved1) == -1)
		return (error_and_return ("error: unable to read reserved1\n"));
	if (read_uint16_move_little(bin, i, &bmp->header.reserved2) == -1)
		return (error_and_return ("error: unable to read reserved2\n"));
	if (read_uint32_move_little(bin, i, &bmp->header.pixel_data_offset) == -1)
		return (error_and_return ("error: unable to read pixel data offset\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.info_header_size) == -1)
		return (error_and_return ("error: unable to read info header size\n"));
	return (0);
}

int	read_info(t_bmp *bmp, size_t *i, t_bin *bin)
{
	if (read_uint32_move_little(bin, i, &bmp->info.with) == -1)
		return (error_and_return ("error: unable to read width\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.height) == -1)
		return (error_and_return ("error: unable to read height\n"));
	if (read_uint16_move_little(bin, i, &bmp->info.nb_planes) == -1)
		return (error_and_return ("error: unable to read nb_planes\n"));
	if (read_uint16_move_little(bin, i, &bmp->info.bpp) == -1)
		return (error_and_return ("error: unable to read bpp\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.compression) == -1)
		return (error_and_return ("error: unable to read compression\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.image_size) == -1)
		return (error_and_return ("error: unable to read image size\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.x_px_m) == -1)
		return (error_and_return ("error: unable to read x_px_m\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.y_px_m) == -1)
		return (error_and_return ("error: unable to read y_px_m\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.nb_color) == -1)
		return (error_and_return ("error: unable to read nb_color\n"));
	if (read_uint32_move_little(bin, i, &bmp->info.nb_important_colors) == -1)
		return (error_and_return ("error: unable to read nb_important_colors\n"\
		));
	return (0);
}

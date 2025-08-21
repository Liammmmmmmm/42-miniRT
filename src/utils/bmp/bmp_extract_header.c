/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_extract_header.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:51:35 by madelvin          #+#    #+#             */
/*   Updated: 2025/08/21 20:47:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "bmp_parsing.h"
#include "error_message.h"
#include <stdio.h>

int	error_and_return(char *message)
{
	ft_dprintf(2, "%s", message);
	return (1);
}

static inline size_t	get_palette_bytes(size_t palette_end, size_t \
	palette_start)
{
	if (palette_end > palette_start)
		return (palette_end - palette_start);
	return (0);
}

int	read_palette(t_bin *bin, size_t *i, t_bmp *bmp, size_t palette_entry_count)
{
	size_t	j;
	size_t	palette_start;
	size_t	palette_end;

	j = 0;
	palette_start = 14 + bmp->info.info_header_size;
	palette_end = bmp->header.pixel_data_offset;
	if (get_palette_bytes(palette_end, palette_start) < palette_entry_count * 4)
		return (error_and_return(RED ERR_BMP_P"\n"));
	bmp->palette = malloc(palette_entry_count * sizeof(uint32_t));
	if (!bmp->palette)
		return (error_and_return(RED ERR_BMP_PATION"\n"));
	*i = palette_start;
	while (j < palette_entry_count && *i + 4 <= bin->size)
	{
		if (read_uint32_move_little(bin, i, &bmp->palette[j]))
		{
			free(bmp->palette);
			return (error_and_return(RED ERR_BMP_P_READ_COLOR"\n"));
		}
		j++;
	}
	return (0);
}

int	read_header(t_bmp *bmp, size_t *i, t_bin *bin)
{
	if (read_uint16_move_little(bin, i, &bmp->header.signature) == -1 || \
	!(bmp->header.signature_c[0] == 'B' && bmp->header.signature_c[1] == 'M'))
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
	if (read_uint32_move_little(bin, i, &bmp->info.width) == -1)
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

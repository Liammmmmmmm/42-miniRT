/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_png_idat.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:32:24 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 16:31:19 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	read_idat_chunk(t_bin *data, t_tex_img *img)
{
	printf("Decompresser\n");
	return (0);
}

int	read_idat_chunks(t_bin *bin, size_t *i, t_tex_img *img)
{
	t_png_chunk	png_chunk;
	t_bin		chunk_data;
	int			y;

	y = 0;
	chunk_data.data = NULL;
	chunk_data.size = 0;
	if (get_png_chunk(bin, i, &png_chunk) == -1)
		return (-1);
	printf("%c%c%c%c\n", png_chunk.type_c[0], png_chunk.type_c[1], png_chunk.type_c[2], png_chunk.type_c[3]);
	while (is_chunk_name(png_chunk.type_c, "IDAT"))
	{
		printf("Oui oui\n");
		y++;
		chunk_data = join_bin(chunk_data, (t_bin){.data = png_chunk.data, .size = png_chunk.data_length});
		if (chunk_data.data == NULL)
			return (print_err_png(PNG_ERROR_MEM));
		if (get_png_chunk(bin, i, &png_chunk) == -1)
			return (-1);
	}
	if (chunk_data.data == NULL || read_idat_chunk(&chunk_data, img) == -1)
	{
		free(png_chunk.data);
		return (print_err_png(PNG_ERROR_CHUNK_DATA2));
	}
	if (!is_chunk_name(png_chunk.type_c, "IEND"))
	{
		free(png_chunk.data);
		return (print_err_png(PNG_ERROR_UNSUPORTED_CHUNK));
	}
	free(png_chunk.data);
	return (0);
}

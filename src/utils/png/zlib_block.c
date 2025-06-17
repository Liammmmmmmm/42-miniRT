/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zlib_block.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:56:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 17:06:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

static int	print_err_free(t_zlib_block *z, char *err)
{
	free(z->deflate_data);
	return (print_err_png(err));
}

int	get_zlib_block(t_bin *data, t_zlib_block *zlib_block)
{
	size_t		i;
	uint32_t	y;

	i = 0;
	if (read_uint8_move(data, &i, &zlib_block->cmf) == -1)
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	if (read_uint8_move(data, &i, &zlib_block->flg) == -1)
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	if (data->size < 6)
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	zlib_block->deflate_size = data->size - 6;
	zlib_block->deflate_data = malloc((data->size - 6) * sizeof(uint8_t));
	if (!zlib_block->deflate_data)
		return (print_err_png(PNG_ERROR_MEM));
	y = (uint32_t)(-1);
	while (++y < zlib_block->deflate_size)
		if (read_uint8_move(data, &i, &zlib_block->deflate_data[y]) == -1)
			return (print_err_free(zlib_block, PNG_ERROR_GET_ZLIB));
	if (read_uint32_move(data, &i, &zlib_block->adler32) == -1)
		return (print_err_free(zlib_block, PNG_ERROR_GET_ZLIB));
	return (0);
}

int	check_zlib_flags(t_zlib_block *zlib_block)
{
	if ((zlib_block->cmf & 0x0F) != 8)
		return (-1);
	if (((zlib_block->cmf << 8) | zlib_block->flg) % 31 != 0)
		return (-1);
	if (zlib_block->flg & 0x20)
		return (-1);
	return (0);
}

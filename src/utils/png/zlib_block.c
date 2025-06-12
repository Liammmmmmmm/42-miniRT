/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zlib_block.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:56:15 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/12 11:12:07 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

int	get_zlib_block(t_bin data, t_zlib_block *zlib_block)
{
	size_t		i;
	uint32_t	y;

	i = 0;
	if (read_uint8_move(&data, &i, &zlib_block->cmf) == -1)
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	if (read_uint8_move(&data, &i, &zlib_block->extra_flags) == -1)
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	if ((long)data.size - 4 <= 0)
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	zlib_block->data_length = data.size - 4;
	zlib_block->data = malloc((data.size - 4) * sizeof(uint8_t));
	if (!zlib_block->data)
		return (print_err_png(PNG_ERROR_MEM));
	y = (uint32_t)(-1);
	while (++y < zlib_block->data_length)
		if (read_uint8_move(&data, &i, &zlib_block->data[y]) == -1)
			return (print_err_png(PNG_ERROR_GET_ZLIB));
	if (read_uint16_move(&data, &i, &zlib_block->check_value) == -1)
	{
		free(zlib_block->data);
		return (print_err_png(PNG_ERROR_GET_ZLIB));
	}
	return (0);
}

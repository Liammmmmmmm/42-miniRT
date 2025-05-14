/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:49:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/13 12:16:01 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_texture_image(t_tex *tex, char *filename)
{
	printf("Trying to get texture `%s'\n", filename);
	if (is_extension(filename, "bmp"))
		parse_bmp(filename, &tex->img);
	else if (is_extension(filename, "hdr"))
	{
		if (parse_hdr(&tex->hdr, filename))
			printf("HDR file successfully parsed\n");
		tex->type = HDR;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:49:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/14 10:08:52 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_texture_image(t_tex *tex, char *filename)
{
	printf("Trying to get texture `%s'\n", filename);
	if (is_extension(filename, "bmp"))
		parse_bmp(filename, &tex->img);
}
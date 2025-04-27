/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 09:49:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 17:11:29 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_texture_image(t_tex *tex, char *filename)
{
	printf("Trying to get texture `%s'\n", filename);
	if (is_extension(filename, "bmp"))
		parse_bmp(filename, &tex->img);
}

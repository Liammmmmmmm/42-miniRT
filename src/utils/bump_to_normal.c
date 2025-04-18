/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_to_normal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:12:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/18 16:38:46 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	bump_to_normal(t_tex_img *bump)
{
	unsigned int	x;
	unsigned int	y;
	const float		scale = 1.0f;
	t_color			*normal;
	t_vec3			n;

	if (bump->height <= 1 || bump->width <= 1)
		return ;
	normal = ft_calloc(sizeof(t_color), (bump->height - 1) * (bump->width - 1));
	if (!normal)
		return ;
	y = 0;
	while (++y < bump->height - 1)
	{
		x = 0;
		while (++x < bump->width - 1)
		{
			n = vec3_unit((t_vec3){\
	-(bump->pixel_data[y * bump->width + (x + 1)].r \
	- bump->pixel_data[y * bump->width + (x - 1)].r) * scale, \
	-(bump->pixel_data[(y - 1) * bump->width + x].r \
	- bump->pixel_data[(y + 1) * bump->width + x].r) * scale, 1.0f});
			normal[y * (bump->width - 1) + x] = (t_color){
				((n.x * 0.5f + 0.5f) * 255.0f), ((n.y * 0.5f + 0.5f) * 255.0f),
				((n.z * 0.5f + 0.5f) * 255.0f)};
		}
	}
	free(bump->pixel_data);
	bump->pixel_data = normal;
	bump->height--;
	bump->width--;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_to_normal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:12:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 18:39:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_vec3	compute_normal(t_tex_img *bump, unsigned int x, unsigned int y)
{
	const float	scale = 1.0f;
	t_vec3		normal;

	normal.x = -(bump->pixel_data[y * bump->width + (x + 1)].r
			- bump->pixel_data[y * bump->width + (x - 1)].r) * scale;
	normal.y = -(bump->pixel_data[(y - 1) * bump->width + x].r
			- bump->pixel_data[(y + 1) * bump->width + x].r) * scale;
	normal.z = 1.0f;
	return (vec3_unit(normal));
}

char	init_normal(t_color *normal, t_tex_img *bump)
{
	if (bump->height <= 1 || bump->width <= 1)
		return (1);
	normal = ft_calloc(sizeof(t_color), (bump->height - 1) * (bump->width - 1));
	if (!normal)
		return (1);
	return (0);
}

void	bump_to_normal(t_tex_img *bump)
{
	unsigned int	x;
	unsigned int	y;
	t_color			*normal;
	t_vec3			n;

	normal = NULL;
	if (init_normal(normal, bump) == 1)
		return ;
	y = 0;
	while (++y < bump->height - 1)
	{
		x = 0;
		while (++x < bump->width - 1)
		{
			n = compute_normal(bump, x, y);
			normal[y * (bump->width - 1) + x] = (t_color){
				((n.x * 0.5f + 0.5f) * 255.0f),
				((n.y * 0.5f + 0.5f) * 255.0f),
				((n.z * 0.5f + 0.5f) * 255.0f)};
		}
	}
	free(bump->pixel_data);
	bump->pixel_data = normal;
	bump->height--;
	bump->width--;
}

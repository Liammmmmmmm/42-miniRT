/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tex_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:04:42 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/31 11:12:55 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline t_fcolor	get_hdr_value(t_tex *tex, double u, double v,
	t_vec3 *hit_point)
{
	{
		if (!tex->hdr.pixels || !tex->hdr.width || !tex->hdr.height)
			return (get_solid_texture_default(*hit_point, 2));
		else
			return (get_hdr_pixel(&tex->hdr, (u * (tex->img.width - 1)), \
			(v * (tex->img.height - 1))));
	}
}

t_fcolor	get_tex_color(t_tex *tex, double u, double v, t_vec3 hit_point)
{
	int	a;
	int	b;

	if (tex->type == IMAGE)
	{
		if (!tex->img.pixel_data || !tex->img.width || !tex->img.height)
			return (get_solid_texture_default(hit_point, 2));
		else
			return (color_to_fcolor(tex->img.pixel_data[tex->img.width * \
		(int)(v * (tex->img.height - 1)) + (int)(u * (tex->img.width - 1))]));
	}
	else if (tex->type == CHECKER_LOCAL)
	{
		a = floor(u * tex->checker.scale);
		b = floor(v * tex->checker.scale);
		if ((a + b) % 2 == 0)
			return (color_to_fcolor(tex->checker.c1));
		return (color_to_fcolor(tex->checker.c2));
	}
	else if (tex->type == CHECKER_GLOBAL)
		return (get_solid_texture(hit_point, tex->checker.c1, tex->checker.c2, \
			tex->checker.scale));
	else if (tex->type == HDR)
		get_hdr_value(tex, u, v, &hit_point);
	return ((t_fcolor){0.0, 0.0, 0.0});
}

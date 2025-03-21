/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:55:21 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/21 13:19:49 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	basic_image(t_minirt *minirt)
{
	t_uint	tpix;
	t_uint	i;
	t_uint	y;
	t_color	color;

	tpix = minirt->mlx.img.width * minirt->mlx.img.height;
	i = 0;
	y = 0;
	while (i < minirt->mlx.img.height)
	{
		y = 0;
		while (y < minirt->mlx.img.width)
		{
			color.r = i * 255 / minirt->mlx.img.height;
			color.g = y * 255 / minirt->mlx.img.width;
			color.b = (minirt->mlx.img.height - i) * 255 / minirt->mlx.img.height;
			minirt->screen.render[minirt->mlx.img.width * i + y].color = color;
			y++;
		}
		i++;
	}

}

t_viewport	init_viewport(t_minirt *minirt)
{
	t_viewport	viewport;
	t_vec3		w;
	t_vec3		u;
	t_vec3		v;

	viewport.focal_length = 1.0;
	viewport.height = 2.0;
	viewport.width = viewport.height * ((double)minirt->mlx.img.width / minirt->mlx.img.height);
	w = minirt->scene.camera.orientation;
	u = vec3_unit(vec3_cross((t_vec3){0, 1, 0}, vec3_negate(minirt->scene.camera.orientation)));
	v = vec3_cross(w, u);
	viewport.u = vec3_multiply_scalar(u, viewport.width);
	viewport.v = vec3_multiply_scalar(v, viewport.height);
	viewport.pixel_delta_u = vec3_divide_scalar(viewport.u, minirt->mlx.img.width);
	viewport.pixel_delta_v = vec3_divide_scalar(viewport.v, minirt->mlx.img.height);
	viewport.upper_left = vec3_subtract(
		vec3_subtract(
			vec3_add(minirt->scene.camera.position, vec3_multiply_scalar(w, viewport.focal_length)),
			vec3_divide_scalar(viewport.u, 2)
		),
		vec3_divide_scalar(viewport.v, 2)
	);
	viewport.pixel00_loc = vec3_add(viewport.upper_left, vec3_multiply_scalar(vec3_add(viewport.pixel_delta_u, viewport.pixel_delta_v), 0.5));
	return (viewport);
}

void	render(t_minirt *minirt)
{
	t_viewport	viewport;
	
	viewport = init_viewport(minirt);
	printf("u : %f %f %f\n", viewport.u.x, viewport.u.y, viewport.u.z);
	printf("v : %f %f %f\n", viewport.v.x, viewport.v.y, viewport.v.z);

	printf("LC : %f %f %f\n", viewport.upper_left.x, viewport.upper_left.y, viewport.upper_left.z);
	basic_image(minirt);
}

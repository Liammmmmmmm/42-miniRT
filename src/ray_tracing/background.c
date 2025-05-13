/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:37:08 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/13 15:32:51 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_fcolor	get_hdr_pixel(t_hdr *hdr, int x, int y)
{
	double	scale;
	double	gamma_corr;

	if (hdr->exposure == 0)
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128);
	else
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128) * powf(2.0, hdr->exposure);
	if (hdr->gamma == 1.0)
		return ((t_fcolor){
			.r = hdr->pixels[y * hdr->width + x].r * scale / 255.0,
			.g = hdr->pixels[y * hdr->width + x].g * scale / 255.0,
			.b = hdr->pixels[y * hdr->width + x].b * scale / 255.0
		});
	else
	{
		gamma_corr = 1.0 / (hdr->gamma);
		return ((t_fcolor){
			.r = pow(hdr->pixels[y * hdr->width + x].r * scale / 255.0, gamma_corr),
			.g = pow(hdr->pixels[y * hdr->width + x].g * scale / 255.0, gamma_corr),
			.b = pow(hdr->pixels[y * hdr->width + x].b * scale / 255.0, gamma_corr)
		});
	}
}

t_fcolor	get_background_color(t_minirt *minirt, t_ray ray)
{
	int		i;
	double	u;
	double	v;

	if (minirt->scene.amb_light.skybox_t == NULL)
		return (color_to_fcolor(minirt->scene.amb_light.skybox_c));
	else
	{
		ray.dir = vec3_unit(ray.dir);
		// v = 0.5 + ray.dir.y * 0.5;
		// u = 0.5 + atan2(ray.dir.x, ray.dir.z) / (2 * PI_D);
		v = 0.5 + asin(ray.dir.y) / PI_D;
		u = 0.5 - atan2(ray.dir.z, ray.dir.x) / (2.0 * PI_D);
		if (minirt->scene.amb_light.skybox_t->type == IMAGE)
		{
			i = minirt->scene.amb_light.skybox_t->img.width
				* (int)(v * minirt->scene.amb_light.skybox_t->img.height)
				+ (int)(u * minirt->scene.amb_light.skybox_t->img.width);
			return (color_to_fcolor(minirt->scene.amb_light.skybox_t->img.pixel_data[i]));
		}
		else if (minirt->scene.amb_light.skybox_t->type == HDR)
			return (get_hdr_pixel(&minirt->scene.amb_light.skybox_t->hdr, u * minirt->scene.amb_light.skybox_t->hdr.width, v * minirt->scene.amb_light.skybox_t->hdr.height));
		else
		{
			return ((t_fcolor){0.0, 0.0, 0.0});
		}
	}
}

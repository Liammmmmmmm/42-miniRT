/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_hdr_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:13:39 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/10 20:17:47 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "hit_register.h"

t_fcolor	get_hdr_pixel(t_hdr *hdr, int x, int y)
{
	double	scale;
	double	gamma_corr;

	if (hdr->exposure == 0)
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128);
	else
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128)
			* powf(2.0, hdr->exposure);
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
			pow(hdr->pixels[y * hdr->width + x].r * scale / 255.0, gamma_corr),
			pow(hdr->pixels[y * hdr->width + x].g * scale / 255.0, gamma_corr),
			pow(hdr->pixels[y * hdr->width + x].b * scale / 255.0, gamma_corr)
		});
	}
}

t_fcolor	get_hdr_pixel_skybox(t_minirt *minirt, t_hdr *hdr, int x, int y)
{
	double			scale;
	double			gamma_corr;
	const double	exposure
		= hdr->exposure + minirt->scene.amb_light.ratio - 1;

	if (exposure == 0)
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128);
	else
		scale = ldexp(1.0, hdr->pixels[y * hdr->width + x].e - 128)
			* powf(2.0, exposure);
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
			pow(hdr->pixels[y * hdr->width + x].r * scale / 255.0, gamma_corr),
			pow(hdr->pixels[y * hdr->width + x].g * scale / 255.0, gamma_corr),
			pow(hdr->pixels[y * hdr->width + x].b * scale / 255.0, gamma_corr)
		});
	}
}

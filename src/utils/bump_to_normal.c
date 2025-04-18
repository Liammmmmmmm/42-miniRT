/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_to_normal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 10:12:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/18 14:35:01 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void bump_to_normal(t_tex_img *bump)
{
	unsigned int x, y;
	const float	scale = 1.0f;  // Facteur de scale pour ajuster l'intensité des normales
	// const float	epsilon = 1.0f; // Valeur de décalage pour calculer les dérivées
	t_color	*normal;

	if (bump->height <= 1 || bump->width <= 1)
		return ;
	normal = malloc(sizeof(t_color) * (bump->height - 1) * (bump->width - 1));
	if (!normal)
		return ;
	for (y = 1; y < bump->height - 1; y++) {
		for (x = 1; x < bump->width - 1; x++) {
			//unsigned char center = bump->pixel_data[y * bump->width + x].r;
			unsigned char left = bump->pixel_data[y * bump->width + (x - 1)].r;
			unsigned char right = bump->pixel_data[y * bump->width + (x + 1)].r;
			unsigned char top = bump->pixel_data[(y - 1) * bump->width + x].r;
			unsigned char bottom = bump->pixel_data[(y + 1) * bump->width + x].r;

			float dx = (right - left) * scale;
			float dy = (top - bottom) * scale;

			t_vec3 n = vec3_unit((t_vec3){-dx, -dy, 1.0f});

			normal[y * (bump->width - 1) + x].r = (unsigned char)((n.x * 0.5f + 0.5f) * 255.0f);
			normal[y * (bump->width - 1) + x].g = (unsigned char)((n.y * 0.5f + 0.5f) * 255.0f);
			normal[y * (bump->width - 1) + x].b = (unsigned char)((n.z * 0.5f + 0.5f) * 255.0f);
		}
	}
	free(bump->pixel_data);
	bump->pixel_data = normal;
	bump->height--;
	bump->width--;
}

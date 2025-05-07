/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:17:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/07 17:23:52 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_color(t_color color)
{
	printf("(" YELLOW "%d, %d, %d" NC ")", color.r, color.g, color.b);
}

void	print_vec3(t_vec3 vec)
{
	printf("(" CYAN "%.2f, %.2f, %.2f" NC ")", vec.x, vec.y, vec.z);
}

void	print_mat_color(t_mat *mat, t_color color)
{
	if (mat)
		printf("    Material: %s", mat->name);
	else
	{
		printf("    Color: ");
		print_color(color);
	}
}

void	print_tex_color_text(t_tex *tex, t_color color, char *txt)
{
	if (tex)
		printf("    %s Texture: %s\n", txt, tex->name);
	else
	{
		printf("    %s Value: ", txt);
		print_color(color);
		printf("\n");
	}
}

void	print_tex_double_text(t_tex *tex, double value, char *txt)
{
	if (tex)
		printf("    %s Texture: %s\n", txt, tex->name);
	else
		printf("    %s Value: %f\n", txt, value);
}

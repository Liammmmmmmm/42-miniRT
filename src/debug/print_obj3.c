/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_obj3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:16:48 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/28 17:54:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_dlight(t_dlight *light)
{
	printf("  " YELLOW "Directional light:\n" NC);
	printf("    Orientation: ");
	print_vec3(light->orientation);
	printf("\n    Brightness: " BLUE "%.2f" NC "\n", light->brightness);
	printf("    Color: ");
	print_color(light->color);
	printf("\n");
}

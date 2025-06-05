/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:34:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/05 10:56:01 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_ui(t_minirt *minirt)
{
	if (minirt->options.sga && get_font(&minirt->controls.font[0],
		"assets/fonts/standard_galactic_alphabet.ttf") == -1)
		return (0);
	else if (!minirt->options.sga && get_font(&minirt->controls.font[0],
		"assets/fonts/falling_sky_medium.ttf") == -1)
		return (0);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 19:34:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 16:44:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	init_ui(t_minirt *minirt)
{
	if (get_font(&minirt->controls.font[0],
			"assets/fonts/falling_sky_medium.ttf") == -1)
		return (0);
	return (1);
}

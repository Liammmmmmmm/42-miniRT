/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dropdown2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 12:23:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 14:06:58 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"
#include "minirt.h"

int	toggle_dropdown(int key, t_point2 pos, t_dropdown *dropdown)
{
	if (pos.x > dropdown->x && pos.y > dropdown->y && pos.x < dropdown->x
		+ dropdown->width && pos.y < dropdown->y + dropdown->height)
	{
		if (key == LEFT_CLICK)
		{
			dropdown->active = !dropdown->active;
			return (1);
		}
	}
	return (0);
}

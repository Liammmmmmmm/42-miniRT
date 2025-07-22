/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_common.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:35:29 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 23:55:30 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	mousedown_common(int key, t_minirt *minirt)
{
	if (key == LEFT_CLICK)
		minirt->controls.keydown.lmb = 1;
	else if (key == MIDDLE_CLICK)
		minirt->controls.keydown.mmb = 1;
	else if (key == RIGHT_CLICK)
		minirt->controls.keydown.rmb = 1;
}

void	mouseup_common(int key, t_minirt *minirt)
{
	minirt->controls.btn_clicked = 0;
	if (key == LEFT_CLICK)
		minirt->controls.keydown.lmb = 0;
	else if (key == MIDDLE_CLICK)
		minirt->controls.keydown.mmb = 0;
	else if (key == RIGHT_CLICK)
		minirt->controls.keydown.rmb = 0;
}

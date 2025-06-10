/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_buttons.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:29:50 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:37:21 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	clear_buttons(t_minirt *minirt)
{
	int	i;

	if (!minirt->controls.buttons)
		return ;
	i = -1;
	while (++i < minirt->controls.nb_buttons)
	{
		free(minirt->controls.buttons[i].text);
		free(minirt->controls.buttons[i].param);
	}
	free(minirt->controls.buttons);
}

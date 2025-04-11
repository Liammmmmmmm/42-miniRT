/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_controls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:33:35 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/25 16:35:54 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_controls(t_minirt *minirt)
{
	ft_bzero(&minirt->controls.keydown, sizeof(t_keydown));
	minirt->controls.open_controls = KEY_C;
}

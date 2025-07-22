/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:39:37 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/22 11:41:37 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_scene(t_minirt *minirt)
{
	ft_bzero(&minirt->scene, sizeof(t_scene));
	ft_bzero(&minirt->scene.obj_lst, sizeof(t_obj_lst));
	minirt->scene.win_width = -1;
	minirt->scene.win_height = -1;
}

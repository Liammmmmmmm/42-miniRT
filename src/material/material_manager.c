/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:51 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 10:49:08 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "material.h"

t_color	material_manager(t_mat_manager mat_man)
{
	if (mat_man.hit_record.mat)
	{
		return (material_mix_v2(&mat_man));
	}
	return (material_default(&mat_man));
}

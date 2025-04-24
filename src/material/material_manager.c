/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:27:51 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/24 12:12:08 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "material.h"

t_ray_data	material_manager(t_mat_manager mat_man)
{
	if (mat_man.hit_record.mat)
	{
		return (material_mix_v2(&mat_man));
	}
	return (material_default(&mat_man));
}

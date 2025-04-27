/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emissive.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 10:52:06 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 16:05:43 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	emissive_material(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.mat->emission_strength == 1)
		return (mat_man->hit_record.mat->emission_color);
	return (color_scale(mat_man->hit_record.mat->emission_color, \
		mat_man->hit_record.mat->emission_strength));
}

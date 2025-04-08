/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_mix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:28:45 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/08 18:50:22 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "material.h"
#include <math.h>

t_color	material_mix(t_mat_manager *mat_man)
{
	if (mat_man->hit_record.mat->metallic_value > 0.0)
		return (reflection_color(mat_man));
	else if (mat_man->hit_record.mat->ior > 0)
		return (refracted_color(mat_man));
	else
		return (material_default(mat_man));
	return ((t_color){0, 0, 0});
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_heat_map_color.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:32:44 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/17 12:48:40 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "maths.h"

t_fcolor	get_heatmap_fcolor(int current_value, int max_value)
{
    t_fcolor color = {0.0f, 0.0f, 0.0f};
    float   ratio;

    if (max_value == 0)
        return ((t_fcolor){0.0f, 0.0f, 1.0f});
    ratio = fminf((float)current_value / max_value, 1.0f);
    if (ratio < 0.25f)
		  color = (t_fcolor){0.0f, 0.0f, 4.0f * ratio};
    else if (ratio < 0.5f)
		  color = (t_fcolor){0.0f, 1.0f, 1.0f - 4.0f * (ratio - 0.25f)};
    else if (ratio < 0.75f)
		  color = (t_fcolor){4.0f * (ratio - 0.5f), 1.0f, 0.0f};
    else
		  color = (t_fcolor){1.0f, 1.0f - 4.0f * (ratio - 0.75f), 0.0f};
    return (color);
}

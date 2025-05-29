/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_mat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:17:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/28 14:58:04 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	set_selected_mat(t_minirt *minirt, t_mat *mat)
{
	if (!mat)
		return ;
	ft_strlcpy(minirt->controls.text_input[0].text, mat->name, 21);
	minirt->controls.text_input[0].cursor_pos = ft_strlen(mat->name);
	link_color_picker(&minirt->controls.color_picker[1], &mat->color_value);
	link_color_picker(&minirt->controls.color_picker[2], &mat->emission_color);
	minirt->controls.dropdown[3].selected = (void **)&mat->color_tex;
	minirt->controls.dropdown[4].selected = (void **)&mat->metallic_tex;
	minirt->controls.dropdown[5].selected = (void **)&mat->roughness_tex;
	minirt->controls.dropdown[6].selected = (void **)NULL; // TRANSMISSION TEXTURE
	minirt->controls.dropdown[7].selected = (void **)&mat->ao_tex;
	minirt->controls.dropdown[8].selected = (void **)NULL; // Emission strength texture
	minirt->controls.dropdown[9].selected = (void **)NULL; // Emission color texture
	minirt->controls.dropdown[10].selected = (void **)&mat->normal;
	
	link_float_input(&minirt->controls.float_input[11], &mat->metallic_value);
	link_float_input(&minirt->controls.float_input[12], &mat->roughness_value);
	link_float_input(&minirt->controls.float_input[13], &mat->ior);
	link_float_input(&minirt->controls.float_input[14], &mat->transmission);
	link_float_input(&minirt->controls.float_input[15], &mat->ao_value);
	link_float_input(&minirt->controls.float_input[16], &mat->emission_strength);
	link_float_input(&minirt->controls.float_input[17], &mat->scale);
	link_float_input(&minirt->controls.float_input[18], &mat->normal_intensity);
}

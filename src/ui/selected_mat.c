/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_mat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:17:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 15:24:18 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

static inline void	set_selected_mat2(t_minirt *minirt, t_mat *mat)
{
	link_float_input(&minirt->controls.float_input[11], &mat->metallic_value);
	link_float_input(&minirt->controls.float_input[12], &mat->roughness_value);
	link_float_input(&minirt->controls.float_input[13], &mat->ior);
	link_float_input(&minirt->controls.float_input[14], &mat->transmission_value
		);
	link_float_input(&minirt->controls.float_input[15], &mat->ao_value);
	link_float_input(&minirt->controls.float_input[16], &mat->emission_strength)
	;
	link_float_input(&minirt->controls.float_input[17], &mat->scale);
	link_float_input(&minirt->controls.float_input[18], &mat->normal_intensity);
}

void	set_selected_mat(t_minirt *minirt, t_mat *mat)
{
	if (!mat)
		return ;
	minirt->micrort.sample = 0;
	minirt->micrort.sphere.material = mat;
	ft_strlcpy(minirt->controls.text_input[0].text, mat->name, 21);
	minirt->controls.text_input[0].cursor_pos = ft_strlen(mat->name);
	link_color_picker(&minirt->controls.color_picker[1], &mat->color_value);
	link_color_picker(&minirt->controls.color_picker[2],
		&mat->emission_color_tmp);
	minirt->controls.dropdown[3].selected = (void **)&mat->color_tex;
	minirt->controls.dropdown[4].selected = (void **)&mat->metallic_tex;
	minirt->controls.dropdown[5].selected = (void **)&mat->roughness_tex;
	minirt->controls.dropdown[6].selected = (void **)&mat->transmission_tex;
	minirt->controls.dropdown[7].selected = (void **)&mat->ao_tex;
	minirt->controls.dropdown[8].selected
		= (void **)&mat->emission_strength_tex;
	minirt->controls.dropdown[9].selected = (void **)&mat->emission_color_tex;
	minirt->controls.dropdown[10].selected = (void **)&mat->normal;
	set_selected_mat2(minirt, mat);
}

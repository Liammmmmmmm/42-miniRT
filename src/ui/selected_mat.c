/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_mat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:17:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 15:14:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	set_selected_mat(t_minirt *minirt, t_mat *mat)
{
	if (!mat)
		return ;
	ft_strlcpy(minirt->controls.text_input[0].text, mat->name, 21);
	minirt->controls.text_input[0].cursor_pos = ft_strlen(mat->name);
}

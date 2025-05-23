/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_mat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 11:17:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 11:19:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ui.h"

void	set_selected_mat(t_minirt *minirt, t_mat *mat)
{
	if (!mat)
		return ;
	(void)minirt;
	printf("Selected object : %s\n", mat->name);
}

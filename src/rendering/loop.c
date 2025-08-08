/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/08 11:59:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"
#include "camera.h"

void	no_display_enable(t_minirt *minirt)
{
	if (minirt->options.anim.enabled)
		no_display_infos_anim(minirt);
	else
		no_display_infos(minirt);
}

static void	no_display_enable_first(t_minirt *minirt, t_bool *skip)
{
	no_display_enable(minirt);
	if (minirt->screen.sample == 0 && *skip == 0)
	{
		*skip = 1;
		return ;
	}
}

void	render_frame(t_minirt *minirt)
{
	static t_bool	skip = 0;

	move_camera(minirt);
	printf("Cam move\n");
	check_sample_amount(minirt);
	if (exit_if_anim_finished(minirt))
		return ;
	if (minirt->options.no_display)
		no_display_enable_first(minirt, &skip);
	render(minirt);
	render_micrort(minirt);
	if (minirt->options.no_display)
		no_display_enable(minirt);
	if (!minirt->options.no_display)
	{
		render_bvh(minirt);
		render_ui(minirt);
		draw_selected_object(minirt);
	}
	minirt->stats.frame += 1;
	skip = 0;
}

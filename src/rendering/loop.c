/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:31:03 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 13:29:37 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"
#include "bmp_parsing.h"

int	exit_if_anim_finished(t_minirt *minirt)
{
	char	*txt;
	ssize_t	time;

	if (!minirt->options.anim.enabled)
		return (0);
	if (minirt->options.anim.frame_i == minirt->options.anim.frames)
	{
		printf("Every frames have been generated\n");
		time = get_cpu_time() - minirt->screen.first_sample_time;
		txt = get_time_dhmsms(time);
		if (txt)
			printf("%d samples and %u frames generated in %zums (%s)\n",
				minirt->screen.sample_total_anim, minirt->options.anim.frames,
				time, txt);
		free(txt);
		mlx_loop_end(minirt->mlx.mlx);
		return (1);
	}
	return (0);
}

static void	no_display_enable(t_minirt *minirt)
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

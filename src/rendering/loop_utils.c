/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:01:36 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/05 12:01:42 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

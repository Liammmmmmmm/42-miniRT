/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:55:53 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/23 18:40:15 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "bvh.h"

static void	draw_ray(t_minirt *minirt, int points[11][2])
{
	int	i;

	i = 1;
	points[0][0] = minirt->scene.win_width / 2;
	points[0][1] = minirt->scene.win_height / 2;
	while (i < 11)
	{
		if (minirt->controls.traced_ray[i].x == 0.0 && minirt->\
	controls.traced_ray[i].y == 0.0 && minirt->controls.traced_ray[i].z == 0.0)
			break ;
		if (i == 1)
			draw_line(&(t_point){points[i - 1][0], points[i - 1][1], 0, 0},
				&(t_point){points[i][0], points[i][1], 0, 0},
				&minirt->mlx.img, 0xFF0000);
		else
			draw_line(&(t_point){points[i - 1][0], points[i - 1][1], 0, 0},
				&(t_point){points[i][0], points[i][1], 0, 0},
				&minirt->mlx.img, 0xF300F3);
		printf("draw line [%d %d] [%d %d]\n", points[i - 1][0],
			points[i - 1][1], points[i][0], points[i][1]);
		i++;
	}
}

void	debug_ray(t_minirt *minirt)
{
	int	points[11][2];
	int	i;

	i = 1;
	while (i < 11)
	{
		if (minirt->controls.traced_ray[i].x == 0.0 && minirt->\
	controls.traced_ray[i].y == 0.0 && minirt->controls.traced_ray[i].z == 0.0)
			break ;
		project_vertex(minirt, minirt->controls.traced_ray[i], &points[i][0],
			&points[i][1]);
		i++;
	}
	draw_ray(minirt, points);
}

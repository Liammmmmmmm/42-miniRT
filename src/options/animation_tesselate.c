/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_tesselate.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:28:45 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 13:21:47 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "maths.h"

void	tessellate_bezier3(t_vec3 *output, uint32_t output_size, t_bezier3 b)
{
	const float	step_per_iter = 1.0 / output_size;
	uint32_t	i;
	float		t;
	float		t1;

	i = -1;
	while (++i <= output_size)
	{
		t = i * step_per_iter;
		t1 = 1.0f - t;
		output[i].x = t1 * t1 * b.p1.x + 2 * t1 * t * b.pc.x + t * t * b.p2.x;
		output[i].y = t1 * t1 * b.p1.y + 2 * t1 * t * b.pc.y + t * t * b.p2.y;
		output[i].z = t1 * t1 * b.p1.z + 2 * t1 * t * b.pc.z + t * t * b.p2.z;
	}
}

void	tessellate_straight_line(t_vec3 *output, uint32_t output_size,
	t_vec3 p1, t_vec3 p2)
{
	const float	step_per_iter = 1.0 / output_size;
	uint32_t	i;

	i = -1;
	while (++i <= output_size)
		output[i] = vec3_lerp(p1, p2, i * step_per_iter);
}

int	get_frame_index_of_created_point(t_tmp_obj_anim *pts, int index)
{
	int	step;
	int	size;
	int	i;

	i = index;
	while (pts->frames[i] == -1)
		i--;
	size = 1;
	while (pts->frames[i + size] == -1)
		size++;
	step = index - i;
	return (pts->frames[i] + step * ((pts->frames[i + size] - pts->frames[i])
			/ (size - 1)));
}

void	get_points_curve(t_bezier_frame	*bf, t_tmp_obj_anim *pts, int i,
	t_bool *pc)
{
	if (pts->frames[i] != -1)
		*bf = (t_bezier_frame){.b = (t_bezier3){.p1 = bf->b.p1, .p2 = pts->\
	points[i], .pc = bf->b.pc}, .frame1 = bf->frame1, .frame2 = pts->frames[i]};
	else
	{
		*pc = 1;
		bf->b.pc = pts->points[i];
		if (pts->frames[i + 1] != -1)
			*bf = (t_bezier_frame){.b = (t_bezier3){.p1 = bf->b.p1, .p2 = \
pts->points[i + 1], .pc = bf->b.pc}, .frame1 = bf->frame1, \
.frame2 = pts->frames[i + 1]};
		else
			*bf = (t_bezier_frame){.b = (t_bezier3){.p1 = bf->b.p1, .p2 = \
vec3_lerp(pts->points[i + 1], pts->points[i], 0.5), .pc = bf->b.pc}, \
.frame1 = bf->frame1, .frame2 = get_frame_index_of_created_point(pts, i)};
	}
	if (pts->frames[i - 1] != -1)
		*bf = (t_bezier_frame){.b = (t_bezier3){.p1 = pts->points[i - 1], \
.p2 = bf->b.p2, .pc = bf->b.pc}, .frame1 = pts->frames[i - 1], .frame2 = \
bf->frame2};
	else
	{
		bf->b.p1 = vec3_lerp(pts->points[i - 1], pts->points[i], 0.5);
		bf->frame1 = get_frame_index_of_created_point(pts, i - 1);
	}
}

int	tesselate_everything(int tab_size, t_tmp_obj_anim *pts, t_obj_anim *obj)
{
	int				i;
	t_bool			pc;
	t_bezier_frame	bf;

	i = 0;
	while (++i < tab_size)
	{
		pc = 0;
		if (pts->frames[i] != -1 && pts->frames[i - 1] == -1)
			continue ;
		get_points_curve(&bf, pts, i, &pc);
		if (pc)
			tessellate_bezier3(&obj->points[bf.frame1], bf.frame2 - bf.frame1,
				bf.b);
		else
			tessellate_straight_line(&obj->points[bf.frame1],
				bf.frame2 - bf.frame1, bf.b.p1, bf.b.p2);
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_rotations.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:52:01 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 13:01:31 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include "maths.h"
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "parsing.h"

static int	parse_point_orientation(char *str, t_tmp_obj_anim *pts, int i)
{
	char	**co;
	int		tab_size;

	co = ft_split_in_line(str, ",");
	if (!co)
		return (print_error(strerror(errno)));
	tab_size = char_tab_len(co);
	if (tab_size != 4)
		return (anim_print_error_f(co, "Orientation point must have 3 "
				"coordinates and a frame index : <x,y,z,F>"));
	if (!is_valid_double_el_no_bordered(co[0], &pts->points[i].x)
		|| !is_valid_double_el_no_bordered(co[1], &pts->points[i].y)
		|| !is_valid_double_el_no_bordered(co[2], &pts->points[i].z))
		return (anim_print_error_f(co, "Invalid vector format. Expected format:"
				" x,y,z"));
	if (!is_valid_positive_int(co[3], &pts->frames[i]))
		return (anim_print_error_f(co, "Invalid frame index."));
	free(co);
	pts->points[i] = vec3_unit(pts->points[i]);
	return (1);
}

static void	get_every_frame_orientation(int tab_size, t_tmp_obj_anim *pts,
	t_obj_anim *obj)
{
	int		i;
	t_vec3	p1;
	t_vec3	p2;
	int		frame1;
	int		frame2;

	i = 0;
	while (++i < tab_size)
	{
		p1 = pts->points[i - 1];
		frame1 = pts->frames[i - 1];
		p2 = pts->points[i];
		frame2 = pts->frames[i];
		tessellate_straight_line(&obj->orientations[frame1], frame2 - frame1,
			p1, p2);
	}
}

static int	compute_orientation_pts(int tab_size, t_tmp_obj_anim *pts,
	t_obj_anim *obj)
{
	int	i;
	int	last_frame;

	if (pts->frames[0] != 0)
		return (print_error("First point must be the frame 0"));
	if (pts->frames[tab_size - 1] != (int)obj->frames - 1)
		return (print_error("Last movement and orientation points must have the"
				" same frame index"));
	i = -1;
	last_frame = 0;
	while (++i < tab_size)
	{
		if (i != 0 && last_frame >= pts->frames[i])
			return (print_error("Orientations not ordered by frame index"));
		last_frame = pts->frames[i];
	}
	get_every_frame_orientation(tab_size, pts, obj);
	return (1);
}

static int	parse_and_compute_orientation_pts(char **parts, int tab_size,
	t_tmp_obj_anim *pts, t_obj_anim *obj)
{
	int	i;

	i = -1;
	while (++i < tab_size)
		if (!parse_point_orientation(parts[i], pts, i))
			return (0);
	if (!compute_orientation_pts(tab_size, pts, obj))
		return (0);
	return (1);
}

int	parse_orientation_points(char *str, t_obj_anim *obj)
{
	t_tmp_obj_anim	points;
	char			**parts;
	int				tab_size;
	int				res;

	parts = ft_split_in_line(str, " ");
	if (!parts)
		return (print_error("Malloc error"));
	tab_size = char_tab_len(parts);
	if (tab_size < 2)
		return (anim_print_error_f(parts, "Not enough orientation points"));
	points.points = ft_calloc(tab_size, sizeof(t_vec3));
	points.frames = ft_calloc(tab_size, sizeof(int));
	if (!points.points || !points.frames)
	{
		free(points.points);
		free(points.frames);
		return (anim_print_error_f(parts, "Malloc error"));
	}
	res = parse_and_compute_orientation_pts(parts, tab_size, &points, obj);
	free(points.points);
	free(points.frames);
	free(parts);
	return (res);
}

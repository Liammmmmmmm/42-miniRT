/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation_move_points.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 13:32:56 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 12:59:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "options.h"
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "parsing.h"

static int	parse_point_frame(char *str, t_tmp_obj_anim *pts, int i)
{
	char	**co;
	int		tab_size;

	co = ft_split_in_line(str, ",");
	if (!co)
		return (print_error(strerror(errno)));
	tab_size = char_tab_len(co);
	if (tab_size != 3 && tab_size != 4)
		return (anim_print_error_f(co, "Animation point must have 3 coordinates"
				", and can have a frame index : <x,y,z[,F]>"));
	if (!is_valid_double_el_no_bordered(co[0], &pts->points[i].x)
		|| !is_valid_double_el_no_bordered(co[1], &pts->points[i].y)
		|| !is_valid_double_el_no_bordered(co[2], &pts->points[i].z))
		return (anim_print_error_f(co, "Invalid vector format. Expected format:"
				" x,y,z"));
	pts->frames[i] = -1;
	if (tab_size == 4)
	{
		if (!is_valid_positive_int(co[3], &pts->frames[i]))
			return (anim_print_error_f(co, "Invalid frame index."));
	}
	free(co);
	return (1);
}

static int	alloc_and_tesselate(int tab_size, t_tmp_obj_anim *pts,
	t_obj_anim *obj)
{
	obj->points = ft_calloc(obj->frames, sizeof(t_vec3));
	obj->orientations = ft_calloc(obj->frames, sizeof(t_vec3));
	if (!obj->orientations || !obj->points || !tesselate_everything(tab_size,
			pts, obj))
	{
		free(obj->orientations);
		free(obj->points);
		return (0);
	}
	return (1);
}

static int	compute_movement_pts(int tab_size, t_tmp_obj_anim *pts,
	t_obj_anim *obj)
{
	int	i;
	int	last_frame;

	if (pts->frames[0] == -1 || pts->frames[tab_size - 1] == -1)
		return (print_error("First and last points must have a frame index"));
	if (pts->frames[0] != 0)
		return (print_error("First point must be the frame 0"));
	i = -1;
	last_frame = 0;
	while (++i < tab_size)
	{
		if (pts->frames[i] == -1)
			continue ;
		if (i != 0 && last_frame >= pts->frames[i])
			return (print_error("Points not ordered by frame index"));
		last_frame = pts->frames[i];
	}
	obj->frames = pts->frames[tab_size - 1] + 1;
	return (alloc_and_tesselate(tab_size, pts, obj));
}

static int	parse_and_compute_pts(char **parts, int tab_size,
	t_tmp_obj_anim *pts, t_obj_anim *obj)
{
	int	i;

	i = -1;
	while (++i < tab_size)
		if (!parse_point_frame(parts[i], pts, i))
			return (0);
	if (!compute_movement_pts(tab_size, pts, obj))
		return (0);
	return (1);
}

int	parse_movement_points(char *str, t_obj_anim *obj)
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
		return (anim_print_error_f(parts, "Not enough movement points"));
	points.points = ft_calloc(tab_size, sizeof(t_vec3));
	points.frames = ft_calloc(tab_size, sizeof(int));
	if (!points.points || !points.frames)
	{
		free(points.points);
		free(points.frames);
		return (anim_print_error_f(parts, "Malloc error"));
	}
	res = parse_and_compute_pts(parts, tab_size, &points, obj);
	free(points.points);
	free(points.frames);
	free(parts);
	return (res);
}

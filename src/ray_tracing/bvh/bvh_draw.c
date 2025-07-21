/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 19:46:13 by delmath           #+#    #+#             */
/*   Updated: 2025/06/30 15:01:44 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "minirt.h"
#include "bvh.h"

void	draw_aabb(t_minirt *minirt, t_vec3 *verts, int color)
{
	int			points[8][2];
	t_point		p1;
	t_point		p2;
	int			i;
	const int	edges[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, \
								{4, 5}, {5, 6}, {6, 7}, {7, 4}, \
								{0, 4}, {1, 5}, {2, 6}, {3, 7}};

	i = 0;
	while (i < 8)
	{
		project_vertex(minirt, verts[i], &points[i][0], &points[i][1]);
		i++;
	}
	i = 0;
	while (i < 12)
	{
		p1 = (t_point){points[edges[i][0]][0], points[edges[i][0]][1], 0,
			color};
		p2 = (t_point){points[edges[i][1]][0], points[edges[i][1]][1], 0,
			color};
		draw_line(&p1, &p2, &minirt->mlx.img, p1.color);
		i++;
	}
}

void	draw_box(t_minirt *minirt, t_vec3 min, t_vec3 max, int color)
{
	t_vec3	verts[8];

	init_box_vertices(verts, min, max);
	draw_aabb(minirt, verts, color);
}

void	render_bvh_recursive(t_minirt *minirt, int depth, int start, \
	t_bvh_node *node)
{
	if (depth < 1 || node->is_leaf)
		return ;
	if (start != 0)
		start -= 1;
	if (node->left_child != 0)
	{
		if (start == 0)
			draw_box(minirt,
				minirt->scene.bvh.bvh_nodes[node->left_child].node_bounds.min,
				minirt->scene.bvh.bvh_nodes[node->left_child].node_bounds.max,
				depth_to_color_int(depth));
		render_bvh_recursive(minirt, depth - 1, start,
			&minirt->scene.bvh.bvh_nodes[node->left_child]);
	}
	if (node->right_child != 0)
	{
		if (start == 0)
			draw_box(minirt,
				minirt->scene.bvh.bvh_nodes[node->right_child].node_bounds.min,
				minirt->scene.bvh.bvh_nodes[node->right_child].node_bounds.max,
				depth_to_color_int(depth));
		render_bvh_recursive(minirt, depth - 1, start,
			&minirt->scene.bvh.bvh_nodes[node->right_child]);
	}
}

void	render_bvh(t_minirt *minirt)
{
	int	depth;
	int	start;

	if (minirt->scene.bvh.valid == 0 || minirt->controls.values.debug == 0)
		return ;
	depth = 10;
	start = 0;
	depth += start;
	if (depth > 0 && start == 0)
		draw_box(minirt, minirt->scene.bvh.bvh_nodes[0].node_bounds.min,
			minirt->scene.bvh.bvh_nodes[0].node_bounds.max, depth);
	if (depth > 1)
		render_bvh_recursive(minirt, depth, start,
			&minirt->scene.bvh.bvh_nodes[0]);
	mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win,
		minirt->mlx.img.img, 0, 0);
}

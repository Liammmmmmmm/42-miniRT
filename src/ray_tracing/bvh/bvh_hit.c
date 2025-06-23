/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_hit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:17:44 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 19:18:28 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "minirt.h"

char	hit_bvh(t_bvh *bvh, uint32_t node_index, t_hit_register_data *data,
	uint64_t *rand)
{
	char				hit_anything;
	t_hit_register_data	data_tmp;
	char				hit_left;
	char				hit_right;
	t_bvh_node			*node;

	node = &bvh->bvh_nodes[node_index];
	if (!intersect_aabb(data->ray, &node->node_bounds))
		return (0);
	hit_anything = 0;
	if (node->is_leaf)
		hit_anything = hit_register_bvh(bvh, node, data, rand);
	else
	{
		data_tmp = *data;
		hit_left = hit_bvh(bvh, node->left_child, data, rand);
		hit_right = hit_bvh(bvh, node->right_child, &data_tmp, rand);
		if (hit_right)
			if ((hit_right && !hit_left) || \
			(data_tmp.hit_record.t < data->hit_record.t))
				data->hit_record = data_tmp.hit_record;
		hit_anything = hit_left || hit_right;
	}
	return (hit_anything);
}

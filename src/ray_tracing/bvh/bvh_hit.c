/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_hit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 12:17:44 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/26 18:09:56 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "minirt.h"

char	hit_bvh(t_bvh *bvh, uint32_t node_index, t_hit_register_data *data,
	int *depth)
{
	char				hit_anything;
	t_hit_register_data	data_tmp;
	char				hit_left;
	char				hit_right;
	t_bvh_node			*node;

	(*depth)++;
	node = &bvh->bvh_nodes[node_index];
	if (!intersect_aabb(data->ray, &node->node_bounds))
		return (0);
	hit_anything = 0;
	if (node->is_leaf)
		hit_anything = hit_register_bvh(bvh, node, data);
	else
	{
		data_tmp = *data;
		hit_left = hit_bvh(bvh, node->left_child, data, depth);
		hit_right = hit_bvh(bvh, node->right_child, &data_tmp, depth);
		if (hit_right)
			if ((hit_right && !hit_left) || \
			(data_tmp.hit_record.t < data->hit_record.t))
				data->hit_record = data_tmp.hit_record;
		hit_anything = hit_left || hit_right;
	}
	return (hit_anything);
}


// #define BVH_STACK_SIZE 64

// char hit_bvh_iteratif(t_bvh *bvh, uint32_t node_index, t_hit_register_data *data, int *depth)
// {
//     char hit_anything = 0;
//     uint32_t stack[BVH_STACK_SIZE];
//     int stack_ptr = 0;
//     t_bvh_node *node;

//     stack[stack_ptr++] = node_index;

//     while (stack_ptr > 0) {
//         node_index = stack[--stack_ptr];
//         node = &bvh->bvh_nodes[node_index];

//         if (!intersect_aabb(data->ray, &node->node_bounds)) {
//             continue;
//         }

//         if (node->is_leaf) {
//             t_hit_register_data data_tmp = *data;
//             if (hit_register_bvh(bvh, node, &data_tmp)) {
//                 if (!hit_anything || data_tmp.hit_record.t < data->hit_record.t) {
//                     *data = data_tmp;
//                 }
//                 hit_anything = 1;
//             }
//         } else {
//             if (stack_ptr >= BVH_STACK_SIZE) {
//                 printf("Stack overflow in hit_bvh!\n");
//                 return hit_anything;
//             }
//             stack[stack_ptr++] = node->right_child;
//             if (stack_ptr >= BVH_STACK_SIZE) {
//                 printf("Stack overflow in hit_bvh!\n");
//                 return hit_anything; 
//             }
//             stack[stack_ptr++] = node->left_child;
//         }
//     }
//     return hit_anything;
// }

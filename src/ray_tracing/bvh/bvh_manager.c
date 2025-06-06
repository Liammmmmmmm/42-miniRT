/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:26:55 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/02 17:18:27 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "minirt.h"
#include <math.h>

void	init_bvh(t_bvh *bvh, t_object *obj_list, uint32_t obj_c)
{
	const uint32_t	count = count_object(obj_list, obj_c);
	const int		normal_mode = bvh->normal_mode;

	printf("\nBuilding BVH:\n");
	ft_bzero(bvh, sizeof(t_bvh));
	if (count == 0)
		return ;
	if (init_bvh_malloc(bvh, count) == 1)
		return ;
	bvh->valid = 1;
	bvh_make_lst(obj_c, obj_list, bvh);
	bvh->actual = 0;
	bvh->size = count;
	bvh->normal_mode = normal_mode;
	bvh->bvh_nodes_used = 0;
	print_progress_bar(0, count);
	build_bvh(bvh, 0, count);
	free(bvh->task_stack.data);
	ft_printf("\n\n");
}

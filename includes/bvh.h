/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:11:47 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/16 14:20:04 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	BVH_H
# define BVH_H

# include "structs.h"

t_aabb		compute_sphere_bounds(t_sphere *s);
char		intersect_aabb(t_ray ray, t_aabb aabb);
uint32_t	build_bvh(t_bvh *bvh, uint32_t start, uint32_t count);
void		init_bvh(t_bvh *bvh, t_object *obj_list, uint32_t obj_c);
void		qsort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh, int axis);
int			compare_sphere_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, int axis);
char		hit_bvh(t_bvh *bvh, uint32_t node_index, t_ray *ray, t_hit_record *hit_rec);
uint32_t	count_object(t_object *obj_list, uint32_t obj_c);
void		print_bvh(t_bvh *bvh, uint32_t node_index, int depth);

#endif
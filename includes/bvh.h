/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:11:47 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/27 17:33:48 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H

# include "structs.h"

/*═════════════════════════════════════════════════════════════════════════════╗
║                             BVH CONSTRUCTION                                ║
╚═════════════════════════════════════════════════════════════════════════════*/
t_aabb			compute_object_bounds(t_object *obj);
uint32_t		build_bvh(t_bvh *bvh, uint32_t start, uint32_t count);
void			init_bvh(t_bvh *bvh, t_object *obj_list, uint32_t obj_c);
void			qsort_axis(uint32_t *arr, t_interval inter, t_bvh *bvh,
					int axis);
int				compare_obj_axis_with_bvh(uint32_t a, uint32_t b, t_bvh *bvh, \
	int axis);
t_vec3			get_obj_position(uint32_t i, t_bvh *bvh);
float			get_axis_value(t_vec3 pos, int axis);

/*═════════════════════════════════════════════════════════════════════════════╗
║                             BVH UTILISATION                                 ║
╚═════════════════════════════════════════════════════════════════════════════*/
char			intersect_aabb(const t_ray *ray, const t_aabb *aabb);
char			hit_bvh(t_bvh *bvh, uint32_t node_index, t_ray *ray, \
	t_hit_record *hit_rec);
void			print_bvh(t_bvh *bvh, uint32_t node_index, int depth);
void			render_bvh(t_minirt *minirt);

/*═════════════════════════════════════════════════════════════════════════════╗
║                                 OUTILS OBJETS                               ║
╚═════════════════════════════════════════════════════════════════════════════*/
uint32_t		count_object(t_object *obj_list, uint32_t obj_c);
uint32_t		hash32(uint32_t x);
int				depth_to_color_int(int depth);
int				init_bvh_malloc(t_bvh *bvh, int count);
t_aabb			compute_object_bounds(t_object *obj);

/*═════════════════════════════════════════════════════════════════════════════╗
║                         PROJECTION / TRIGONOMÉTRIE                          ║
╚═════════════════════════════════════════════════════════════════════════════*/
t_calc_trigo	get_cam_trigo(t_vec3 orientation);
void			project_vertex(t_minirt *m, t_vec3 v, int *x, int *y);
void			init_box_vertices(t_vec3 *v, t_vec3 min, t_vec3 max);
void			draw_box(t_minirt *minirt, t_vec3 min, t_vec3 max, int color);

#endif

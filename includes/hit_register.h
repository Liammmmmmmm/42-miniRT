/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_register.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:08:50 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/30 16:25:55 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIT_REGISTER_H
# define HIT_REGISTER_H

# include "structs.h"

void		apply_all_map(t_hit_register_data *data);
void		apply_normal_map(t_hit_record *hit);
t_color		get_obj_color(t_object *obj);
t_fcolor	get_hit_register_color(t_mat *mat, t_color color, t_hit_record *hit, t_bvh *bvh);
t_fcolor	get_hdr_pixel(t_hdr *hdr, int x, int y);
t_fcolor	get_tex_color(t_tex *tex, double u, double v, t_vec3 hit_point);

#endif
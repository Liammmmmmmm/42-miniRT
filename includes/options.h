/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:19:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/21 13:35:16 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONS_H
# define OPTIONS_H

# include "structs.h"

typedef struct s_bezier3
{
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	pc;
}	t_bezier3;

typedef struct s_tmp_obj_anim
{
	t_vec3		*points;
	int			*frames;
}	t_tmp_obj_anim;

typedef struct s_bezier_frame
{
	t_bezier3	b;
	int			frame1;
	int			frame2;
}	t_bezier_frame;

int		tesselate_everything(int tab_size, t_tmp_obj_anim *pts, t_obj_anim *obj);
int		get_frame_index_of_created_point(t_tmp_obj_anim *pts, int index);
void	tessellate_straight_line(t_vec3 *output, uint32_t output_size, t_vec3 p1, t_vec3 p2);
void	tessellate_bezier3(t_vec3 *output, uint32_t output_size, t_bezier3 b);

void	debug_print_animation(t_animation *anim);
int		anim_print_error_f(char **parts, char *err);
int		print_error_sy(char *err, int *y);
void	free_anim(t_animation *anim);
void	set_auto_orientation_points(t_obj_anim *obj);
int		parse_orientation_points(char *str, t_obj_anim *obj);
int		parse_movement_points(char *str, t_obj_anim *obj);
int		parse_one_anim_obj(char *str, t_obj_anim *obj);
int		is_animate_option(t_minirt *minirt, char *argvi, int *y);
int		parse_options(t_minirt *minirt, int argc, char **argv);

#endif

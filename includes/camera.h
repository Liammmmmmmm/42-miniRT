/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:23:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/24 16:55:05 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "structs.h"

# define MOVE_SENSI 2 // u/s
# define MOUSE_SENSITIVITY 0.05

t_vec3	get_right_vector(t_minirt *minirt);
t_vec3	get_up_vector(t_minirt *minirt);

void	move_camera(t_minirt *minirt);
t_bool	is_cam_moving(t_minirt *minirt);

#endif

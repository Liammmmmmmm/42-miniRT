/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:23:08 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 18:22:19 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "structs.h"

# define MOVE_SENSI 0.1

t_vec3	get_right_vector(t_minirt *minirt);
t_vec3	get_up_vector(t_minirt *minirt);

void	move_camera(t_minirt *minirt);

#endif

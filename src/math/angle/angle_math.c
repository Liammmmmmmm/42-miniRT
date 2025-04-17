/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:39:30 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/17 20:41:11 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

void	vector_to_angles(double vec[3], double *yaw, double *pitch)
{
	*pitch = asin(vec[1]);
	*yaw = atan2(vec[2], vec[0]);
}

void	angles_to_vector(double yaw, double pitch, double vec[3])
{
	vec[0] = cos(pitch) * cos(yaw);
	vec[1] = sin(pitch);
	vec[2] = cos(pitch) * sin(yaw);
}

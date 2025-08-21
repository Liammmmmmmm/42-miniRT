/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:21:20 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 20:49:30 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASIC_STRUCTS_H
# define BASIC_STRUCTS_H

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef struct s_rgba
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}	t_rgba;

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:39:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/19 10:42:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/**
 * @struct s_color
 * @brief Structure representing a color with RGB components.
 * 
 * @param r Red component of the color.
 * @param g Green component of the color.
 * @param b Blue component of the color.
 */
typedef struct s_color
{
	unsigned char	r;	/* Red component of the color. */
	unsigned char	g;	/* Green component of the color. */
	unsigned char	b;	/* Blue component of the color. */
}	t_color;

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
}	t_vec3;

/**
 * @struct s_point
 * @brief Structure representing a point in 2D space with a color.
 * 
 * @param x X-coordinate of the point on the screen.
 * @param y Y-coordinate of the point on the screen.
 * @param z Depth of the point.
 * @param color Color of the point.
 */
typedef struct s_sc_point
{
	int		x;		/* X-coordinate of the point on the screen. */
	int		y;		/* Y-coordinate of the point on the screen. */
	float	z;		/* Depth of the point. */
	int		color;	/* Color of the point. */
}	t_sc_point;

typedef struct s_amb_light
{
	float	ratio;
	t_color	color;
}	t_amb_light;

typedef struct s_camera
{
	t_vec3			position;
	t_vec3			orientation;
	unsigned char	fov; 
}	t_camera;

typedef struct s_light
{
	t_vec3	position;
	float	brightness;
	t_color	color;
}	t_light;

typedef struct s_sphere
{
	t_vec3	position;
	float	diameter;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	position;
	t_vec3	orientation;
	t_color	color;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	position;
	t_vec3	orientation;
	float	diameter;
	float	height;
	t_color	color;
}	t_cylinder;

typedef enum e_objects
{
	LIGHT,
	SPHERE,
	PLANE,
	CYLINDER
}	t_objects;

typedef struct s_object
{
	void		*object;
	t_objects	type;
}	t_object;

typedef struct s_scene
{
	t_object	*elements;	// Liste des tous les objets de la scene
	int			el_amount;	// Nombre d'elements dans la liste
	t_amb_light	amb_light;
	t_camera	camera;
}	t_scene;

typedef struct s_minirt
{
	t_scene	scene;
	
}	t_minirt;

#endif
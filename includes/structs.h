/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:39:37 by lilefebv          #+#    #+#             */
/*   Updated: 2025/03/25 16:41:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libft.h"

typedef unsigned char t_bool;

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

typedef struct s_lcolor
{
	__uint64_t	r;
	__uint64_t	g;
	__uint64_t	b;
}	t_lcolor;

typedef struct s_lsc_point
{
	t_uint		x;
	t_uint		y;
	t_lcolor	color;
}	t_lsc_point;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef	struct s_interval
{
	double	min;
	double	max;
}	t_interval;

typedef struct s_ray
{
	t_vec3	orig;	/* The origin of the ray. */
	t_vec3	dir;	/* The direction of the ray. */
}	t_ray;	/*	Add more section for future (length_squared for optimisation)	*/

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
	t_uint	x;		/* X-coordinate of the point on the screen. */
	t_uint	y;		/* Y-coordinate of the point on the screen. */
	float	z;		/* Depth of the point. */
	t_color	color;	/* Color of the point. */
}	t_sc_point;

typedef enum e_tex_type
{
	IMAGE,
	COLOR,
}	t_tex_type;

typedef struct s_tex
{
	char		name[21];
	t_tex_type	type;
	int			fd;
}	t_tex;

typedef struct s_mat
{
	char	name[21];
	t_tex	*color_tex;
	t_color	color_value;
	t_tex	*metallic_tex;
	double	metallic_value;
	t_tex	*roughness_tex;
	double	roughness_value;
	double	ior;
	double	transmission;
	double	emission_strength;
	t_color	emission_color;
	t_tex	*normal;
}	t_mat;

typedef	struct s_hit_record
{
	t_vec3	point;
	t_vec3	normal;
	double	t;
	t_mat	*mat;
	char	front_face;
	t_color	color;
}	t_hit_record;

typedef struct s_amb_light
{
	double	ratio;
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
	double	brightness;
	t_mat	*material;
	t_color	color;
}	t_light;

typedef struct s_sphere
{
	t_vec3	position;
	double	diameter;
	t_mat	*material;
	t_color	color;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	position;
	t_vec3	normal;
	t_color	color;
	t_mat	*material;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	position;
	t_vec3	orientation;
	double	diameter;
	double	height;
	t_mat	*material;
	t_color	color;
}	t_cylinder;

typedef enum e_objects
{
	NULL_OBJ,
	TEXTURE,
	MATERIAL,
	AMBIANT_LIGHT,
	CAMERA,
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
	t_tex		*textures;
	int			tex_amount;
	t_mat		*materials;
	int			mat_amount;
	t_object	*elements;	// Liste des tous les objets de la scene
	int			el_amount;	// Nombre d'elements dans la liste
	t_amb_light	amb_light;
	t_camera	camera;
}	t_scene;

/**
 * @struct s_img
 * @brief Structure representing an image with its properties.
 * 
 * @param img Pointer to identify the image.
 * @param img_str String containing all the pixels of the image.
 * @param img_depth Contain the depth of each pixel (from camera)
 * @param bits Number of bits per pixel.
 * @param size_line Size of the img_str.
 * @param endian Indicates the endianness of the image.
 */
typedef struct s_img
{
	void	*img;		/* Pointer to identify the image */
	char	*img_str;	/* String containing all the pixels of the image */
	float	*img_depth;	/* Contain the depth of each pixel (from camera) */
	int		bits;		/* Number of bits per pixel */
	int		size_line;	/* Size of the img_str */
	int		endian;		/* Indicates the endianness of the image */
	t_uint	width;
	t_uint	height;
}	t_img;

typedef struct s_mlx
{
	void	*mlx;
	void	*render_win;
	void	*controls_win;
	t_img	img;
	t_img	img_controls;
}	t_mlx;

typedef struct s_screen
{
	t_lsc_point	*render;	// Liste de chaque pixel pour l'image rendue
	int			sample;
	int			spp; // sample per pixel
	int			start_render;
	// stocker aussi ici les différents boutons, sliders ou autres
}	t_screen;

typedef struct s_stats
{
	int	frame;
}	t_stats;

typedef struct s_viewport
{
	float	focal_length;
	float	height;
	float	width;
	float	gamma;
	t_vec3	u;
	t_vec3	v;
	t_vec3	pixel_delta_u;
	t_vec3	pixel_delta_v;
	t_vec3	upper_left;
	t_vec3	pixel00_loc;
}	t_viewport;

typedef struct s_keydown
{
	t_bool	lmb;
	t_bool	mmb;
	t_bool	rmb;
	t_bool	lshift;
	t_bool	rshift;
	t_bool	lctrl;
	t_bool	rctrl;
}	t_keydown;

typedef struct s_controls
{
	t_keydown	keydown;
	int			open_controls;
	
}	t_controls;

typedef struct s_minirt
{
	t_scene		scene;
	t_mlx		mlx;
	t_screen	screen;
	t_stats		stats;
	t_viewport	viewport;
	t_controls	controls;
}	t_minirt;

#endif
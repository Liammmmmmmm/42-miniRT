/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_base.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:10:05 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/08 15:10:52 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_BASE_H
# define MLX_BASE_H

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
	t_uint	width4;
	t_uint	lpx;
	t_uint	lpy;
	t_uint	height;
}	t_img;

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

/**
 * @struct s_point
 * @brief Structure representing a point in 2D space with a color.
 * 
 * @param x X-coordinate of the point on the screen.
 * @param y Y-coordinate of the point on the screen.
 * @param z Depth of the point.
 * @param color Color of the point.
 */
typedef struct s_point
{
	int32_t	x;		/* X-coordinate of the point on the screen. */
	int32_t	y;		/* Y-coordinate of the point on the screen. */
	double		z;		/* Depth of the point. */
	int			color;	/* Color of the point. */
}	t_point;

#endif
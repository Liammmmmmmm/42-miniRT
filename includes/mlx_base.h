/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_base.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:10:05 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 21:03:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_BASE_H
# define MLX_BASE_H

# include <math.h>
# include "libft.h"

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
	void	*img;
	char	*img_str;
	float	*img_depth;
	int		bits;
	int		size_line;
	int		endian;
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
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_color;

typedef struct s_color_alpha
{
	int		color;
	float	alpha;
}	t_color_alpha;

typedef struct s_hsv_color
{
	uint16_t	hue;
	double		sat;
	double		val;
}	t_hsv_color;

typedef struct s_fcolor
{
	double	r;
	double	g;
	double	b;
}	t_fcolor;

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
	int32_t	x;
	int32_t	y;
	double	z;
	int		color;
}	t_point;

typedef struct s_draw_circle
{
	int	xc;
	int	yc;
	int	x;
	int	y;
}	t_draw_circle;

typedef struct s_circle
{
	int	xc;
	int	yc;
	int	r;
}	t_circle;

void		put_pixel_image_alpha(t_img *img, int x, int y, t_color_alpha color)
			;

int			color_to_int(t_color color);
void		color_to_hex(char *dest, t_color color);
t_color		int_to_color(int color);

void		draw_vertical_line(t_img *img, uint32_t x, uint32_t y1,
				uint32_t y2);
void		draw_horizontal_line(t_img *img, uint32_t y, uint32_t x1,
				uint32_t x2);

t_hsv_color	rgb_to_hsv(int rgb);
int			hsv_to_rgb(int h, float s, float v);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_components.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:34:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/03/22 18:37:53 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_COMPONENTS_H
# define MLX_COMPONENTS_H

# include "libft.h"
# include "structs.h"

typedef struct s_point
{
	int		x;
	int		y;
	double	z;
	int		color;
}	t_point;

typedef struct s_button
{
	int		x;
	int		y;
	int		width;
	int		height;
	int		border_color;
	int		background_color;
	int		border_color_on_click;
	int		background_color_on_click;
	int		is_clicked;
	char	*text;
	void	*param;
	void	(*action)(void *);
}	t_button;

typedef struct s_int_slider
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	point_color;
	int	bar_color;
	int	min;
	int	max;
	int *value;
	int	is_clicked;
}	t_int_slider;

typedef struct s_text_input
{
	int		x;
	int		y;
	int		width;
	int		height;
	int		border_color;
	int		background_color;
	int		border_color_focus;
	int		is_focused;
	int		padding_left;
	int		max_char;
	int		text_color;
	char	text[256];
	char	placeholder[256];
	int		cursor_pos;
}	t_text_input;

void	display_slider_int(t_img *img, const t_int_slider slider);
int		slider_mouse_down(t_int_slider *slider, int mouse_x, int mouse_y);
int		slider_mouse_up(t_int_slider *slider);
int		slider_mouse_move(t_int_slider *slider, int mouse_x);

void	display_button(t_img *img, const t_button button, unsigned char font[96][5]);
int		button_action(t_button *button, int mouse_x, int mouse_y);
int		button_release(t_button *button);

void	display_text_input(t_img *img, t_text_input *text_input, unsigned char font[96][5]);
int		text_input_focus(t_text_input *text_input, int mouse_x, int mouse_y);
int		text_input_type(t_text_input *text_input, int key, int is_shift_down);

#endif
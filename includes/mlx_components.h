/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_components.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:34:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/04/22 15:06:38 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_COMPONENTS_H
# define MLX_COMPONENTS_H

# include "libft.h"
# include "mlx_base.h"
# include "font.h"

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
	int		disabled;
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

typedef struct s_color_picker
{
	t_button		btn;
	t_bool			active;
	t_color			*color;
	t_hsv_color		hsv;
	t_bool			mouse_in_hue;
	t_bool			mouse_in_val_sat;
	t_text_input	text_input;
}	t_color_picker;

void	put_pixel_image(t_img *img, int x, int y, int color);

/**
 * @brief Draw a string in the image.
 *
 * This function draws a string in the image using the specified font and
 * starting position.
 * 
 * The difference between this function and the original one of the mlx is
 * that this one draw on an image and not a window so we can gain some
 * efficiency and be sure that there can't be other error.
 *
 * @param img The image structure.
 * @param font The font data.
 * @param p The starting position and color.
 * @param str The string to draw.
 */
void	string_to_img(t_img *img, unsigned char font[96][5], t_point p, char *str);

int		string_size(char *str);

/**
 * @brief Draw a character in the image.
 *
 * This function draws a character in the image using the specified font and
 * starting position.
 * 
 * This function really do the job of string_to_img. Take a character of the
 * font and set the according pixels to a given color
 *
 * @param img The image structure.
 * @param font The font data.
 * @param p The starting position and color.
 * @param c The character to draw.
 */
void	char_to_img(t_img *img, unsigned char font[96][5], t_point p, char c);

void	draw_circle(int xc, int yc, int x, int y, t_img *img, int color);
void	circle_bres(int xc, int yc, int r, t_img *img, int color);
void	draw_circle_comp(int xc, int yc, int x, int y, t_img *img, int color);
void	circle_bres_comp(int xc, int yc, int r, t_img *img, int color);
void	draw_filled_circle(int xc, int yc, int r, t_img *img, int color);

void	display_slider_int(t_img *img, const t_int_slider slider);
int		slider_mouse_down(t_int_slider *slider, int mouse_x, int mouse_y);
int		slider_mouse_up(t_int_slider *slider);
int		slider_mouse_move(t_int_slider *slider, int mouse_x);

void	display_button(t_img *img, const t_button button, t_ttf *ttf);
int		button_action(t_button *button, int mouse_x, int mouse_y, int *is_clicked);
int		button_release(t_button *button);

void	display_text_input(t_img *img, t_text_input *text_input, unsigned char font[96][5]);
int		text_input_focus(t_text_input *text_input, int mouse_x, int mouse_y);
int		text_input_type(t_text_input *text_input, int key, int is_shift_down);

void	init_color_picker(t_color_picker *cp, t_color *color_var);
void	display_color_picker(t_img *img, t_color_picker *cp, uint8_t font8b[96][5]);
int		color_picker_release(t_color_picker *cp);
int		color_picker_action(t_color_picker *cp, int mouse_x, int mouse_y);
int		color_picker_mouse_move(t_color_picker *cp, int mouse_x, int mouse_y);
int		color_picker_type(t_color_picker *cp, int key);

#endif
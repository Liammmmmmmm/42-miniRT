/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 13:51:54 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/27 15:39:28 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

static int	pow_10(int dec)
{
	int	res;
	int	i;

	i = 0;
	res = 1;
	if (dec > 11)
		dec = 11;
	while (i < dec)
	{
		res *= 10;
		i++;
	}
	return (res);
}

void	set_integer_to_buff(int n, int *i, char *s)
{
	if (n <= 9)
		s[(*i)++] = "0123456789"[n];
	else
	{
		ft_add_i(n / 10, i, s);
		s[(*i)++] = "0123456789"[n % 10];
	}
}

int	ft_get_n_size(int n)
{
	int	nsize;

	if (n == 0)
		return (1);
	nsize = 0;
	while (n > 0)
	{
		nsize++;
		n = n / 10;
	}
	return (nsize);
}

int	set_float_to_buff(double n, char *s)
{
	int				i;
	long long		int_part;
	long long		frac_part;
	const int		decimals = 6;
	const long long	scale = pow_10(decimals);

	i = 0;
	if (n < 0)
	{
		s[i++] = '-';
		n = -n;
	}
	n = n + 0.5 / scale;
	int_part = (long long)n;
	frac_part = (long long)((n - int_part) * scale);
	set_integer_to_buff(int_part, &i, s);
	if (frac_part > 0)
	{
		s[i++] = '.';
		int_part = decimals - ft_get_n_size(frac_part);
		while (int_part-- > 0)
			s[i++] = '0';
		set_integer_to_buff(frac_part, &i, s);
		while (i > 0 && s[i - 1] == '0')
			s[--i] = '\0';
		if (i > 0 && s[i - 1] == '.')
			s[--i] = '\0';
	}
	s[i] = '\0';
	return (i);
}

void	display_float_input(t_img *img, t_float_input *input, t_ttf *ttf)
{
	int			i;
	int			j;
	t_point2	start;
	int			br_color;

	if (input->is_focused)
		br_color = input->border_color_focus;
	else
		br_color = input->border_color;
	i = input->x;
	while (i <= input->x + input->width)
	{
		put_pixel_image(img, i, input->y, br_color);
		put_pixel_image(img, i, input->y + input->height, br_color);
		i++;
	}
	i = input->y + 1;
	while (i < input->y + input->height)
	{
		put_pixel_image(img, input->x, i, br_color);
		put_pixel_image(img, input->x + input->width, i, br_color);
		j = input->x + 1;
		while (j < input->x + input->width)
		{
			put_pixel_image(img, j, i, input->background_color);
			j++;
		}
		i++;
	}
	if (input->number && !input->is_focused && *input->number != input->last_displayed)
	{
		input->last_displayed = *input->number;
		input->cursor_pos = set_float_to_buff(input->last_displayed, input->text);
	}
	if (input->text[0])
	{
		start.x = input->x + input->padding_left;
		start.y = input->y + input->height * 0.5 + ((get_height(ttf)) * 0.5);
		ttf->color = input->text_color;
		draw_string(img, ttf, input->text, start);
	}
}

int	float_input_focus(t_float_input *input, int mouse_x, int mouse_y)
{
	if (input->x <= mouse_x && input->x + input->width >= mouse_x
		&& input->y <= mouse_y && input->y + input->height >= mouse_y)
	{
		input->is_focused = 1;
		return (1);
	}
	if (input->number)
	{
		input->last_displayed = *input->number;
		input->cursor_pos = set_float_to_buff(input->last_displayed, input->text);
	}
	input->is_focused = 0;
	return (0);
}

void	link_float_input(t_float_input *input, double *val)
{
	input->number = val;
	if (!val)
	{
		input->last_displayed = 0.0;
		return ;
	}
	input->cursor_pos = set_float_to_buff(*val, input->text);
	input->last_displayed = *val;
}

int	is_valid_future_float(char *num)
{
	int	i;
	int	pointam;

	pointam = 0;
	i = -1;
	if (num[0] == '-' || num[0] == '+' )
		i++;
	while (num[++i])
	{
		if (ft_isdigit(num[i]))
			continue ;
		else if (num[i] == '.' && pointam == 0)
			pointam = 1;
		else if (num[i] == '.' && pointam != 0)
			return (0);
		else
			return (0);
	}
	return (1);
}

int	float_input_type(t_float_input *input, int key)
{
	if (input->is_focused)
	{
		if (key == ',')
			key = '.';
		if (ft_isdigit(key) || key == '-' || key == '.')
		{
			if (input->cursor_pos >= 16 || input->cursor_pos >= input->max_char)
				return (1);
			input->text[input->cursor_pos] = key;
			input->cursor_pos++;
			input->text[input->cursor_pos] = 0;
			if (input->number && is_valid_future_float(input->text))
				*input->number = ft_atod(input->text);
			else
			{
				input->text[input->cursor_pos] = 0;
				input->cursor_pos--;
			}
		}
		else if (key == 65288 && input->cursor_pos > 0)
		{
			input->cursor_pos--;
			input->text[input->cursor_pos] = 0;
			if (input->number && is_valid_future_float(input->text))
				*input->number = ft_atod(input->text);
		}
		return (1);
	}
	return (0);
}


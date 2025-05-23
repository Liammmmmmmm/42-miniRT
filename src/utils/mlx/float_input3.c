/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 07:44:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 16:25:43 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

static int	is_valid_future_float(char *num)
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

static void	manage_input(t_float_input *input, int key)
{
	if (ft_isdigit(key) || key == '-' || key == '.')
	{
		if (input->cursor_pos >= 16 || input->cursor_pos >= input->max_char)
			return ;
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
}

void	replace_input_float_input(int *key)
{
	if (*key == ',')
		*key = '.';
	else if (*key == 65438)
		*key = '0';
	else if (*key == 65436)
		*key = '1';
	else if (*key == 65433)
		*key = '2';
	else if (*key == 65435)
		*key = '3';
	else if (*key == 65430)
		*key = '4';
	else if (*key == 65437)
		*key = '5';
	else if (*key == 65432)
		*key = '6';
	else if (*key == 65429)
		*key = '7';
	else if (*key == 65431)
		*key = '8';
	else if (*key == 65434)
		*key = '9';
	else if (*key == 65453)
		*key = '-';
	else if (*key == 65439)
		*key = '.';
}

int	float_input_type(t_float_input *input, int key)
{
	if (input->is_focused)
	{
		replace_input_float_input(&key);
		manage_input(input, key);
		return (1);
	}
	return (0);
}

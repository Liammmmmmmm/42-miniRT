/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 07:44:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/28 12:51:27 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include "ui.h"

static int	is_valid_value(char key, char *buffer, int i)
{
	if (ft_isdigit(key))
		return (0);
	if (i == 0)
		return (!(ft_isdigit(key) || key == '-'));
	if (ft_isdigit(buffer[i - 1]))
		return (0);
	if (key == '-' && i > 1 && ft_isdigit(buffer[i - 1]))
		return (0);
	return (1);
}

static void	manage_input(t_float_input *input, int key)
{
	if (ft_isdigit(key) || key == '-' || key == '.' || key == '*' || key == '/' || key == '+')
	{
		if (input->cursor_pos >= 16 || input->cursor_pos >= input->max_char || is_valid_value(key, input->text, input->cursor_pos))
			return ;
		input->text[input->cursor_pos] = key;
		input->cursor_pos++;
		input->text[input->cursor_pos] = '\0';
	}
	else if (key == 65288 && input->cursor_pos > 0)
	{
		input->cursor_pos--;
		input->text[input->cursor_pos] = ' ';
	}
	else if (key == 65293)
	{
		if (input->number)
		{
			*input->number = parse_expression(input->text);
			input->cursor_pos = set_float_to_buff(*input->number, input->text);
			
		}
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
	else if (*key == 65455)
		*key = '/';
	else if (*key == 65450)
		*key = '*';
	else if (*key == 65451)
		*key = '+';
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 07:44:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/02 13:12:33 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"
#include "structs.h"
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
	if (ft_isdigit(key) || key == '-' || key == '.' || key == '*' || key == '/'
		|| key == '+')
	{
		if (input->cursor_pos >= 16 || input->cursor_pos >= input->max_char
			|| is_valid_value(key, input->text, input->cursor_pos))
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
	size_t					i;
	static const t_key_map	map[16] = {{',', '.'},
	{65438, '0'}, {65436, '1'}, {65433, '2'}, {65435, '3'}, {65430, '4'},
	{65437, '5'}, {65432, '6'}, {65429, '7'}, {65431, '8'}, {65434, '9'},
	{65453, '-'}, {65439, '.'}, {65455, '/'}, {65450, '*'}, {65451, '+'}};

	i = 0;
	while (i < sizeof(map) / sizeof(map[0]))
	{
		if (*key == map[i].from)
		{
			*key = map[i].to;
			return ;
		}
		i++;
	}
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

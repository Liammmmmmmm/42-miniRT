/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expr_float.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:30:28 by madelvin          #+#    #+#             */
/*   Updated: 2025/05/28 12:51:14 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static float	parse_number(const char **expr)
{
	double	num;
	double	frac;
	double	divisor;

	num = 0.0f;
	frac = 0.0f;
	divisor = 10.0f;
	while (ft_isdigit(**expr))
	{
		num = num * 10.0f + (**expr - '0');
		(*expr)++;
	}
	if (**expr == '.')
	{
		(*expr)++;
		while (ft_isdigit(**expr))
		{
			frac += (**expr - '0') / divisor;
			divisor *= 10.0f;
			(*expr)++;
		}
	}
	return (num + frac);
}

static double	parse_term(const char **expr)
{
	double	result;
	double	num;
	char	op;

	result = parse_number(expr);
	while (**expr == '*' || **expr == '/')
	{
		op = **expr;
		(*expr)++;
		num = parse_number(expr);
		if (op == '*')
			result *= num;
		else if (op == '/')
		{
			if (num == 0)
				result = 0;
			else
				result /= num;
		}
	}
	return (result);
}

double	parse_expression(const char *expr)
{
	double	result;
	double	term;
	char	op;

	result = parse_term(&expr);
	while (*expr == '+' || *expr == '-')
	{
		op = *expr;
		expr++;
		term = parse_term(&expr);
		if (op == '+')
			result += term;
		else if (op == '-')
			result -= term;
	}
	return (result);
}

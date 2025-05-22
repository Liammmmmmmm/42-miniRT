/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_input2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 07:44:13 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/22 07:47:01 by lilefebv         ###   ########lyon.fr   */
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

static void	set_integer_to_buff(int n, int *i, char *s)
{
	if (n <= 9)
		s[(*i)++] = "0123456789"[n];
	else
	{
		ft_add_i(n / 10, i, s);
		s[(*i)++] = "0123456789"[n % 10];
	}
}

static int	ft_get_n_size(int n)
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

static void	set_float_to_buff_init(int *i, double *n, char *s,
	const long long scale)
{
	*i = 0;
	if (*n < 0)
	{
		s[(*i)++] = '-';
		*n = -*n;
	}
	*n = *n + 0.5 / scale;
}

int	set_float_to_buff(double n, char *s)
{
	int				i;
	long long		int_part;
	long long		frac_part;
	const int		decimals = 6;
	const long long	scale = pow_10(decimals);

	set_float_to_buff_init(&i, &n, s, scale);
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

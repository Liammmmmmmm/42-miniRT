/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   png_filters.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:09:11 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/13 16:33:17 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "png_parser.h"

void	png_filter_sub(uint8_t *data, size_t len, uint8_t bpp)
{
	size_t	i;

	i = bpp - 1;
	while (++i < len)
		data[i] = (uint8_t)(data[i] + data[i - bpp]);
}

void	png_filter_up(uint8_t *data, const uint8_t *prev, size_t len)
{
	size_t	i;

	if (!prev)
		return ;
	i = 0;
	while (i < len)
	{
		data[i] = (uint8_t)(data[i] + prev[i]);
		i++;
	}
}

void	png_filter_average(uint8_t *data, const uint8_t *prev, size_t len,
	uint8_t bpp)
{
	size_t	i;
	uint8_t	left;
	uint8_t	up;

	i = 0;
	while (i < len)
	{
		if (i < bpp)
			left = 0;
		else
			left = data[i - bpp];
		if (prev)
			up = prev[i];
		else
			up = 0;
		data[i] = (uint8_t)(data[i] + ((left + up) >> 1));
		i++;
	}
}

static	uint8_t	paeth_predictor(int a, int b, int c)
{
	const int	p = a + b - c;
	const int	pa = ft_abs(p - a);
	const int	pb = ft_abs(p - b);
	const int	pc = ft_abs(p - c);

	if (pa <= pb && pa <= pc)
		return (a);
	if (pb <= pc)
		return (b);
	return (c);
}

void	png_filter_paeth(uint8_t *data, const uint8_t *prev, size_t len,
	uint8_t bpp)
{
	size_t	i;
	uint8_t	a;
	uint8_t	b;
	uint8_t	c;

	i = 0;
	while (i < len)
	{
		if (i < bpp)
			a = 0;
		else
			a = data[i - bpp];
		if (prev)
			b = prev[i];
		else
			b = 0;
		if (prev && i >= bpp)
			c = prev[i - bpp];
		else
			c = 0;
		data[i] = (uint8_t)(data[i] + paeth_predictor(a, b, c));
		i++;
	}
}

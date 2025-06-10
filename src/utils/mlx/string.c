/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 14:38:27 by lilefebv          #+#    #+#             */
/*   Updated: 2025/06/10 18:54:32 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_components.h"

void	char_to_img(t_img *img, unsigned char font[96][5], t_point p, char c)
{
	int	i;
	int	j;

	if (c < 32)
		return ;
	i = -1;
	c -= 32;
	while (++i < 5)
	{
		j = -1;
		while (++j < 7)
			if (font[(int)c][i] & (1 << j))
				put_pixel_image(img, p.x + i, p.y + j, p.color);
	}
}

static void	set_text_color(char *str, int *i, t_point *p)
{
	if (*(str + 1) == '#' && *(str + 2))
	{
		*i = 0;
		while (ft_strchr("0123456789ABCDEF", str[2 + *i]) && *i < 6)
			(*i)++;
		p->color = ft_atoi_base_limited(str + 2, "0123456789ABCDEF", 6);
		str = str + *i + 1;
	}
}

void	string_to_img(t_img *img, unsigned char font[96][5], t_point p, \
	char *str)
{
	int	xstart;
	int	i;

	xstart = p.x;
	while (*str)
	{
		if (*str == '\033' && *(str + 1))
			set_text_color(str, &i, &p);
		else if (*str == '\n')
			p = (t_point){.x = xstart, .y = p.y = 14};
		else
		{
			char_to_img(img, font, p, *str);
			p.x += 6;
		}
		str++;
	}//fonction inutiliser
}

int	string_size(char *str)
{
	int	i;
	int	size;

	size = 0;
	while (*str)
	{
		if (*str == '\033' && *(str + 1))
		{
			if (*(str + 1) == '#' && *(str + 2))
			{
				i = 0;
				while (ft_strchr("0123456789ABCDEF", str[2 + i]) && i < 6)
					i++;
				str = str + i + 1;
			}
		}
		else if (*str == '\n')
			return (size);
		else
			size += 6;
		str++;
	}
	if (size > 0)
		size--;
	return (size);
}

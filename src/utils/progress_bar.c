/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:52:15 by madelvin          #+#    #+#             */
/*   Updated: 2025/07/21 14:47:59 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	write_no_err(int fd, char *str, size_t size)
{
	(void)!write(fd, str, size);
}

void	print_progress_bar(size_t actual, size_t max)
{
	const size_t	f = (int)((double)actual / (double)max * PROGRESS_BAR_SIZE);
	const size_t	percent = (int)((double)actual / (double)max * 100);
	static size_t	last_filled = (size_t)-1;
	size_t			i;

	if (f == last_filled)
		return ;
	write_no_err(1, "\e[?25l", 7);
	if (f != 0)
		ft_printf("\r");
	write_no_err(1, "[", 1);
	i = (size_t)(-1);
	while (++i < PROGRESS_BAR_SIZE)
	{
		if (i < f)
			write_no_err(1, YELLOW"="NC, 15);
		else
			write_no_err(1, " ", 1);
	}
	ft_printf("] %d%% (%d/%d)", percent, actual, max);
	last_filled = f;
	if (last_filled == 100)
		last_filled = (size_t)(-1);
	write_no_err(1, "\e[?25h", 7);
}

char	get_size_t_size(size_t nb)
{
	char	count;

	count = 1;
	while (nb >= 10)
	{
		nb /= 10;
		count++;
	}
	return (count);
}

void	clear_progress_bar(size_t actual, size_t max)
{
	size_t	total_length;
	size_t	i;

	total_length = PROGRESS_BAR_SIZE + 8;
	total_length += get_size_t_size(actual) + get_size_t_size(max);
	total_length
		+= get_size_t_size((size_t)((double)actual / (double)max * 100));
	write_no_err(1, "\r", 1);
	i = 0;
	while (i < total_length)
	{
		write_no_err(1, " ", 1);
		i++;
	}
	write_no_err(1, "\r", 1);
}

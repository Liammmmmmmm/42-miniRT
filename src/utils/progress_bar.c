/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:52:15 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/05 13:42:57 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static inline void	write_no_err(int fd, char *str, size_t size)
{
	(void)!write(fd, str, size);
}

void	print_progress_bar(size_t actual, size_t max)
{
	const size_t	bar_width = 50;
	const size_t	filled = (int)((double)actual / (double)max * bar_width);
	const size_t	percent = (int)((double)actual / (double)max * 100);
	static size_t	last_filled = (size_t)-1;
	size_t			i;

	if (filled == last_filled)
		return ;
	write_no_err(1, "\e[?25l", 7);
	if (filled != 0)
		ft_printf("\r");
	write_no_err(1, "[", 1);
	i = (size_t)(-1);
	while (++i < bar_width)
	{
		if (i < filled)
			write_no_err(1, YELLOW"="NC, 15);
		else
			write_no_err(1, " ", 1);
	}
	ft_printf("] %d%% (%d/%d)", percent, actual, max);
	last_filled = filled;
	if (last_filled == 100)
		last_filled = (size_t)(-1);
	write_no_err(1, "\e[?25h", 7);
}

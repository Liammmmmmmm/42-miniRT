/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_render_state.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 11:33:34 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/21 22:58:51 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	export_render_state(t_minirt *minirt)
{
	int		fd;

	fd = open("render_state.rts", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error("creating save file\n");
		return ;
	}
	(void)!write(fd, (char *)&minirt->scene.render_width, sizeof(int));
	(void)!write(fd, (char *)&minirt->scene.render_height, sizeof(int));
	(void)!write(fd, (char *)&minirt->screen.sample, sizeof(int));
	(void)!write(fd, (char *)minirt->screen.float_render, sizeof(t_fcolor)
		* minirt->scene.render_width * minirt->scene.render_height);
	printf("Render state successfully saved\n");
	close(fd);
}

void	init_float_render(t_minirt *minirt)
{
	int32_t	rw;
	int32_t	rh;
	int32_t	sample;

	if (!minirt->options.load_render.data
		|| read_int32_little(&minirt->options.load_render, 0, &rw) < 0
		|| read_int32_little(&minirt->options.load_render, 4, &rh) < 0
		|| read_int32_little(&minirt->options.load_render, 8, &sample) < 0
		|| minirt->options.load_render.size - 12 != (size_t)(rw * rh * 3 * 8)
		|| minirt->scene.win_width != rw || minirt->scene.win_height != rh)
		return (ft_bzero(minirt->screen.float_render, sizeof(t_fcolor)
				* minirt->viewport.render_w * minirt->viewport.render_h));
	minirt->screen.sample = sample;
	ft_memcpy(minirt->screen.float_render,
		minirt->options.load_render.data + 12, rw * rh * 3 * 8);
}

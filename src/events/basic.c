/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:23:57 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/22 15:07:39 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	destroy_controls(t_minirt *minirt)
{
	if (!minirt)
		return (0);
	if (minirt->mlx.controls_win)
	{
		mlx_destroy_window(minirt->mlx.mlx, minirt->mlx.controls_win);
		minirt->mlx.controls_win = NULL;
	}
	return (0);
}

int	destroy(t_minirt *minirt)
{
	if (minirt != NULL)
		mlx_loop_end(minirt->mlx.mlx);
	return (0);
}

void	keydown_common(int key, t_minirt *minirt)
{
	if (key == KEY_ESC)
		mlx_loop_end(minirt->mlx.mlx);
	else if (key == KEY_LCTRL)
		minirt->controls.keydown.lctrl = (char)1;
	else if (key == KEY_RCTRL)
		minirt->controls.keydown.rctrl = (char)1;
	else if (key == KEY_LSHIFT)
		minirt->controls.keydown.lshift = (char)1;
	else if (key == KEY_RSHIFT)
		minirt->controls.keydown.rshift = (char)1;
	else if (key == KEY_D && minirt->controls.values.debug == 0)
		minirt->controls.values.debug = 1;
	else if (key == KEY_D && minirt->controls.values.debug == 1)
	{
		minirt->controls.values.debug = 0;
		put_render_to_frame(minirt);
		mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
	}
}

void	keyup_common(int key, t_minirt *minirt)
{
	if (key == KEY_LCTRL)
		minirt->controls.keydown.lctrl = (char)0;
	else if (key == KEY_RCTRL)
		minirt->controls.keydown.rctrl = (char)0;
	else if (key == KEY_LSHIFT)
		minirt->controls.keydown.lshift = (char)0;
	else if (key == KEY_RSHIFT)
		minirt->controls.keydown.rshift = (char)0;
}

void	mousedown_common(int key, t_minirt *minirt)
{
	char	item_null;

	if (key == LEFT_CLICK)
	{
		item_null = (minirt->controls.selected_object == NULL);
		minirt->controls.keydown.lmb = (char)1;
		minirt->controls.selected_object = select_object(minirt, minirt->controls.mlxr, minirt->controls.mlyr);
		if (minirt->controls.selected_object == NULL && item_null == 0)
		{
			put_render_to_frame(minirt);
			mlx_put_image_to_window(minirt->mlx.mlx, minirt->mlx.render_win, minirt->mlx.img.img, 0, 0);
		}
		else if (minirt->controls.selected_object != NULL && item_null == 0)
			put_render_to_frame(minirt);
	}
	else if (key == MIDDLE_CLICK)
		minirt->controls.keydown.mmb = (char)1;
	else if (key == RIGHT_CLICK)
		minirt->controls.keydown.rmb = (char)1;
}

void	mouseup_common(int key, t_minirt *minirt)
{
	minirt->controls.btn_clicked = (char)0;
	if (key == LEFT_CLICK)
		minirt->controls.keydown.lmb = (char)0;
	else if (key == MIDDLE_CLICK)
		minirt->controls.keydown.mmb = (char)0;
	else if (key == RIGHT_CLICK)
		minirt->controls.keydown.rmb = (char)0;
}

int	keydown_render(int key, t_minirt *minirt)
{
	keydown_common(key, minirt);
	if (key == minirt->controls.open_controls)
	{
		if (!minirt->mlx.controls_win)
		{
			minirt->mlx.controls_win = mlx_new_window(minirt->mlx.mlx, CWIN_WIDTH, CWIN_HEIGHT, "Controls");
			events_controls(minirt);
		}
	}
	return (0);
}

int	keydown_controls(int key, t_minirt *minirt)
{
	int	i;

	keydown_common(key, minirt);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		if (color_picker_type(&minirt->controls.color_picker[i], key))
			return (0);
	return (0);
}

int	keyup_controls(int key, t_minirt *minirt)
{
	keyup_common(key, minirt);
	
	return (0);
}

int	keyup_render(int key, t_minirt *minirt)
{
	keyup_common(key, minirt);
	
	return (0);
}

int	mousedown_controls(int key, int x, int y, t_minirt *minirt)
{
	int	i;

	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	mousedown_common(key, minirt);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		if (color_picker_action(&minirt->controls.color_picker[i], x, y))
			return (0);
	i = -1;
	while (++i < minirt->controls.nb_buttons)
		button_action(&minirt->controls.buttons[i], x, y, &minirt->controls.btn_clicked);
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		slider_mouse_down(&minirt->controls.sliders[i], x, y);
	return (0);
}

int	mousedown_render(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mousedown_common(key, minirt);
	return (0);
}

int	mouseup_controls(int key, int x, int y, t_minirt *minirt)
{
	int	i;

	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	mouseup_common(key, minirt);
	i = -1;
	while (++i < minirt->controls.nb_buttons)
		button_release(&minirt->controls.buttons[i]);
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		slider_mouse_up(&minirt->controls.sliders[i]);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		color_picker_release(&minirt->controls.color_picker[i]);
	return (0);
}

int	mouseup_render(int key, int x, int y, t_minirt *minirt)
{
	minirt->controls.mlxr = (t_uint)x;
	minirt->controls.mlyr = (t_uint)y;
	mouseup_common(key, minirt);
	
	return (0);
}

int	mouse_move_controls(int x, int y, t_minirt *minirt)
{
	int	i;

	minirt->controls.mlxc = (t_uint)x;
	minirt->controls.mlyc = (t_uint)y;
	i = -1;
	while (++i < minirt->controls.nb_sliders)
		slider_mouse_move(&minirt->controls.sliders[i], x);
	i = -1;
	while (++i < minirt->controls.nb_color_picker)
		color_picker_mouse_move(&minirt->controls.color_picker[i], x, y);	
	return (0);
}

void	events_controls(t_minirt *minirt)
{
	mlx_hook(minirt->mlx.controls_win, ON_DESTROY, 0, destroy_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_KEYDOWN, 1L << 0, keydown_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_KEYUP, 1L << 0, keyup_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_MOUSEDOWN, 1L << 2, mousedown_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_MOUSEUP, 1L << 3, mouseup_controls, minirt);
	mlx_hook(minirt->mlx.controls_win, ON_MOUSEMOVE, 1L << 6, mouse_move_controls, minirt);
}

void	events(t_minirt *minirt)
{
	// mlx_hook(minirt->mlx.render_win, ON_MOUSEDOWN, 1L << 2, mouse_down, minirt);
	// mlx_hook(minirt->mlx.render_win, ON_MOUSEUP, 1L << 3, mouse_up, minirt);
	// mlx_hook(minirt->mlx.render_win, ON_MOUSEMOVE, 1L << 6, mouse_move_render, minirt);
	mlx_hook(minirt->mlx.render_win, ON_DESTROY, 0, destroy, minirt);
	mlx_hook(minirt->mlx.render_win, ON_KEYDOWN, 1L << 0, keydown_render, minirt);
	mlx_hook(minirt->mlx.render_win, ON_KEYUP, 1L << 0, keyup_render, minirt);
	mlx_hook(minirt->mlx.render_win, ON_MOUSEDOWN, 1L << 2, mousedown_render, minirt);
	mlx_hook(minirt->mlx.render_win, ON_MOUSEUP, 1L << 3, mouseup_render, minirt);
	events_controls(minirt);
}

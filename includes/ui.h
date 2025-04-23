/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:22:47 by lilefebv          #+#    #+#             */
/*   Updated: 2025/04/23 11:57:56 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UI_H
# define UI_H

# include "structs.h"

# define UI_BACK_COLOR 0x0E0E0E
# define UI_SEP_COLOR 0xFFFFFF
# define UI_BUTTON_BACK 0xB4B4B4
# define UI_BUTTON_CLICK 0x919191

/*═════════════════════════════════════════════════════════════════════════════╗
║                                 CONTROLS / UI                                ║
╚═════════════════════════════════════════════════════════════════════════════*/

// typedef struct s_ui_param
// {
// 	int	key;
// 	int	mouse_x;
// 	int	mouse_y;
// }	t_ui_param;

void	draw_box_2d(t_img *img, t_point2 p1, t_point2 p2, int color);

int		init_ui(t_minirt *minirt);

void	init_font(t_minirt *minirt);
void	init_controls(t_minirt *minirt);
int		init_buttons(t_minirt *minirt);
int		init_sliders(t_minirt *minirt);
int		init_cps(t_minirt *minirt);

void	clear_sliders(t_minirt *minirt);
void	click_start_stop(void *vparam);
void	clear_buttons(t_minirt *minirt);

void	change_tab(void *vparam);

void	draw_list_objects(t_minirt *minirt);

void	init_layout(t_img *img, t_minirt *minirt);

void	render_ui(t_minirt *minirt);

#endif
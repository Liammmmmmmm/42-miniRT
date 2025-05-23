/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:53:44 by lilefebv          #+#    #+#             */
/*   Updated: 2025/05/23 11:23:59 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	draw_vertical_line(t_img *img, uint32_t x, uint32_t y1, uint32_t y2)
{
	while (y1 < y2)
	{
		put_pixel_image(img, x, y1, UI_SEP_COLOR);
		y1++;
	}
}

void	draw_horizontal_line(t_img *img, uint32_t y, uint32_t x1, uint32_t x2)
{
	while (x1 < x2)
	{
		put_pixel_image(img, x1, y, UI_SEP_COLOR);
		x1++;
	}
}

void	draw_box_2d(t_img *img, t_point2 p1, t_point2 p2, int color)
{
	int	i;

	while (p1.y < p2.y)
	{
		i = p1.x - 1;
		while (++i < p2.x)
			put_pixel_image(img, i, p1.y, color);
		p1.y++;
	}
}

void	setcolor(t_img *img, uint32_t color)
{
	int i;
	int	total;

	total = img->width * img->height;
	i = 0;
	while (i < total)
	{
		((uint32_t *)img->img_str)[i] = color;
		i++;
	}
}

void	init_layout(t_img *img, t_minirt *minirt)
{
	setcolor(img, UI_BACK_COLOR);
	draw_vertical_line(img, 300, 0, img->height);
	
	minirt->controls.font[0].size = 30;
	minirt->controls.font[0].color = 0xFFFFFF;
	
	if (minirt->controls.ui_infos.tab_selected == 0)
	{
		draw_horizontal_line(img, 300, 300, img->width);
		draw_horizontal_line(img, 450, 300, img->width);
		// draw_string(img, &minirt->controls.font[0], "Liste objets", (t_point2){350, 170});
		draw_string(img, &minirt->controls.font[0], "Ajouter un objet", (t_point2){330, 380});

		draw_list_objects(minirt);
		
		if (minirt->controls.ui_infos.selected_object)
		{
			minirt->controls.font[0].size = 25;
			if (minirt->controls.ui_infos.selected_object->type == SPHERE)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				// Peut etre pas gerer l'orientation de suite pour les spheres
				// draw_string(img, &minirt->controls.font[0], "Orientation", (t_point2){320, 590});
				if (!((t_sphere *)minirt->controls.ui_infos.selected_object->object)->material)
					draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Material", (t_point2){320, 790});
				draw_string(img, &minirt->controls.font[0], "Size", (t_point2){320, 890});

				minirt->controls.font[0].size = 20;
				// Position
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Peut etre pas gerer l'orientation de suite pour les spheres
				// minirt->controls.font[0].size = 20;
				// // Orientation
				// draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 626});
				// draw_box_2d(img, (t_point2){325, 610}, (t_point2){395, 630}, 0xFFFFFF);
				// draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 626});
				// draw_box_2d(img, (t_point2){420, 610}, (t_point2){490, 630}, 0xFFFFFF);
				// draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 626});
				// draw_box_2d(img, (t_point2){520, 610}, (t_point2){590, 630}, 0xFFFFFF);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;

				// Material 
				// Displayed at the end
				
				// Size
				draw_string(img, &minirt->controls.font[0], "Diameter", (t_point2){310, 920});
				display_float_input(img, &minirt->controls.float_input[6], &minirt->controls.font[0]);
				// draw_box_2d(img, (t_point2){400, 905}, (t_point2){590, 925}, 0xFFFFFF);
				
				// Last to render for render priority reasons (with the part that can appear on the others)
				display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
				if (!((t_sphere *)minirt->controls.ui_infos.selected_object->object)->material)
					display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
			}
			else if (minirt->controls.ui_infos.selected_object->type == PLANE)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				draw_string(img, &minirt->controls.font[0], "Orientation", (t_point2){320, 590});
				if (!((t_plane *)minirt->controls.ui_infos.selected_object->object)->material)
					draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Material", (t_point2){320, 790});

				// Position
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Orientation
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 626});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 626});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 626});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[3], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[4], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[5], &minirt->controls.font[0]);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;

				// Material								
				// Last to render for render priority reasons (with the part that can appear on the others)
				display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
				if (!((t_plane *)minirt->controls.ui_infos.selected_object->object)->material)
					display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
			}
			else if (minirt->controls.ui_infos.selected_object->type == CYLINDER)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				draw_string(img, &minirt->controls.font[0], "Orientation", (t_point2){320, 590});
				if (!((t_cylinder *)minirt->controls.ui_infos.selected_object->object)->material)
					draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Material", (t_point2){320, 790});
				draw_string(img, &minirt->controls.font[0], "Size", (t_point2){320, 950});

				// Position
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Orientation
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 626});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 626});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 626});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[3], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[4], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[5], &minirt->controls.font[0]);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;

				// Material
				// Displayed at the end
				
				// Size
				draw_string(img, &minirt->controls.font[0], "Diameter", (t_point2){310, 980});
				display_float_input(img, &minirt->controls.float_input[6], &minirt->controls.font[0]);
				
				draw_string(img, &minirt->controls.font[0], "Height", (t_point2){310, 1010});
				display_float_input(img, &minirt->controls.float_input[7], &minirt->controls.font[0]);
				
				// Last to render for render priority reasons (with the part that can appear on the others)
				display_mat_dropdown(minirt, &minirt->controls.dropdown[2]);
				display_mat_dropdown(minirt, &minirt->controls.dropdown[1]);
				display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
				if (!((t_cylinder *)minirt->controls.ui_infos.selected_object->object)->material)
					display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
			}
			else if (minirt->controls.ui_infos.selected_object->type == LIGHT)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Power", (t_point2){320, 890});
				
				// Position
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Power
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "Power", (t_point2){310, 920});
				display_float_input(img, &minirt->controls.float_input[6], &minirt->controls.font[0]);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;
				display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
			}
			else if (minirt->controls.ui_infos.selected_object->type == CONE)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				draw_string(img, &minirt->controls.font[0], "Orientation", (t_point2){320, 590});
				if (!((t_cylinder *)minirt->controls.ui_infos.selected_object->object)->material)
					draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Material", (t_point2){320, 790});
				draw_string(img, &minirt->controls.font[0], "Size", (t_point2){320, 920});

				// Position
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Orientation
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 626});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 626});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 626});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[3], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[4], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[5], &minirt->controls.font[0]);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;

				// Material
				// Displayed at the end
				
				// Size
				draw_string(img, &minirt->controls.font[0], "Diameter", (t_point2){310, 950});
				display_float_input(img, &minirt->controls.float_input[6], &minirt->controls.font[0]);
				
				draw_string(img, &minirt->controls.font[0], "Height", (t_point2){310, 980});
				display_float_input(img, &minirt->controls.float_input[7], &minirt->controls.font[0]);
				
				// Last to render for render priority reasons (with the part that can appear on the others)
				display_mat_dropdown(minirt, &minirt->controls.dropdown[1]);
				display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
				if (!((t_cone *)minirt->controls.ui_infos.selected_object->object)->material)
					display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
			}
			else if (minirt->controls.ui_infos.selected_object->type == HYPERBOLOID)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				draw_string(img, &minirt->controls.font[0], "Orientation", (t_point2){320, 590});
				if (!((t_hyperboloid *)minirt->controls.ui_infos.selected_object->object)->material)
					draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Material", (t_point2){320, 790});
				draw_string(img, &minirt->controls.font[0], "Properties", (t_point2){320, 890});

				// Position
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Orientation
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 626});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 626});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 626});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[3], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[4], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[5], &minirt->controls.font[0]);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;

				// Material
				// Displayed at the end

				// Size
				draw_string(img, &minirt->controls.font[0], "Shape", (t_point2){310, 920});
				display_float_input(img, &minirt->controls.float_input[6], &minirt->controls.font[0]);

				draw_string(img, &minirt->controls.font[0], "Height", (t_point2){310, 950});
				display_float_input(img, &minirt->controls.float_input[7], &minirt->controls.font[0]);

				draw_string(img, &minirt->controls.font[0], "a", (t_point2){310, 980});
				display_float_input(img, &minirt->controls.float_input[8], &minirt->controls.font[0]);

				draw_string(img, &minirt->controls.font[0], "b", (t_point2){310, 1010});
				display_float_input(img, &minirt->controls.float_input[9], &minirt->controls.font[0]);

				draw_string(img, &minirt->controls.font[0], "c", (t_point2){310, 1040});
				display_float_input(img, &minirt->controls.float_input[10], &minirt->controls.font[0]);


				// Last to render for render priority reasons (with the part that can appear on the others)
				display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
				if (!((t_hyperboloid *)minirt->controls.ui_infos.selected_object->object)->material)
					display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);

			}
			else if (minirt->controls.ui_infos.selected_object->type == CUSTOM)
			{
				draw_string(img, &minirt->controls.font[0], "Position", (t_point2){320, 490});
				draw_string(img, &minirt->controls.font[0], "Orientation", (t_point2){320, 590});
				if (!((t_custom_object *)minirt->controls.ui_infos.selected_object->object)->material)
					draw_string(img, &minirt->controls.font[0], "Color", (t_point2){320, 690});
				draw_string(img, &minirt->controls.font[0], "Material", (t_point2){320, 790});
				draw_string(img, &minirt->controls.font[0], "Properties", (t_point2){320, 890});

				// Position
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 526});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 526});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 526});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[0], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[1], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[2], &minirt->controls.font[0]);

				// Orientation
				minirt->controls.font[0].size = 20;
				draw_string(img, &minirt->controls.font[0], "X", (t_point2){310, 626});
				draw_string(img, &minirt->controls.font[0], "Y", (t_point2){405, 626});
				draw_string(img, &minirt->controls.font[0], "Z", (t_point2){505, 626});
				minirt->controls.font[0].size = 15;
				display_float_input(img, &minirt->controls.float_input[3], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[4], &minirt->controls.font[0]);
				display_float_input(img, &minirt->controls.float_input[5], &minirt->controls.font[0]);

				// Color (without color picker bcs for render priority reasons it should be at the end)
				minirt->controls.font[0].size = 20;

				// Material
				// Displayed at the end

				// Size
				draw_string(img, &minirt->controls.font[0], "Scale x", (t_point2){310, 920});
				display_float_input(img, &minirt->controls.float_input[6], &minirt->controls.font[0]);

				draw_string(img, &minirt->controls.font[0], "Scale y", (t_point2){310, 950});
				display_float_input(img, &minirt->controls.float_input[7], &minirt->controls.font[0]);

				draw_string(img, &minirt->controls.font[0], "Scale z", (t_point2){310, 980});
				display_float_input(img, &minirt->controls.float_input[8], &minirt->controls.font[0]);


				// A afficher : name, triangle count

				// Last to render for render priority reasons (with the part that can appear on the others)
				display_mat_dropdown(minirt, &minirt->controls.dropdown[0]);
				if (!((t_custom_object *)minirt->controls.ui_infos.selected_object->object)->material)
					display_color_picker(img, &minirt->controls.color_picker[0], &minirt->controls.font[0]);
			}
			// draw_string(img, &minirt->controls.font[0], "Props de l'objet", (t_point2){330, 750});
			// draw_string(img, &minirt->controls.font[0], get_object_name(minirt->controls.ui_infos.selected_object->type), (t_point2){330, 800});
			// 
		}
	}
	else if (minirt->controls.ui_infos.tab_selected == 1)
	{
		draw_horizontal_line(img, 300, 300, img->width);
		draw_horizontal_line(img, 780, 300, img->width);

		draw_list_materials(minirt);

		draw_string(img, &minirt->controls.font[0], "Preview mat\n(microRT)", (t_point2){370, 930});
		
		if (minirt->controls.ui_infos.selected_material)
		{
			draw_string(img, &minirt->controls.font[0], "Prop du materiau", (t_point2){320, 540});
		}
	}
	draw_box_2d(img, (t_point2){301, 0}, (t_point2){600, 30}, UI_BACK_COLOR);
	draw_horizontal_line(img, 30, 300, img->width);
}

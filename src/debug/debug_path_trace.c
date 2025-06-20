/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_path_trace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:43:13 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/20 18:54:23 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	debug_path_trace(t_minirt *minirt, t_ray ray, int max_depth)
{
	t_fcolor			accumulated_color;
	t_fcolor			power;
	t_hit_register_data	data;
	int					bounce;

	bounce = 0;
	accumulated_color = (t_fcolor){0.0, 0.0, 0.0};
	power = (t_fcolor){1.0, 1.0, 1.0};
	while (bounce < max_depth && bounce < 10)
	{
		minirt->controls.traced_ray[bounce] = ray.orig;
		printf("hit : ");
		print_vec3(minirt->controls.traced_ray[bounce]);
		printf(" - dir: ");
		print_vec3(ray.dir);
		printf("\n");
		if (hit_register_all(minirt, &data) == 1)
		{
			ray.orig = data.hit_record.point;
			material_manager_v3(minirt, &ray, &data.hit_record,
				(t_ray_data){&power, &accumulated_color});
		}
		else
		{
			minirt->controls.traced_ray[bounce + 1]
				= vec3_add(ray.orig, vec3_multiply_scalar(ray.dir, 4));
			printf("skybox : ");
			print_vec3(minirt->controls.traced_ray[bounce + 1]);
			printf(" - dir: ");
			print_vec3(ray.dir);
			printf("\n");
			break ;
		}
		bounce++;
	}
}

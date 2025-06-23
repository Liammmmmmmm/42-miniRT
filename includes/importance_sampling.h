/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   importance_sampling.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:59:32 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/23 18:31:09 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPORTANCE_SAMPLING_H
# define IMPORTANCE_SAMPLING_H

# include "structs.h"

char	init_all_malloc_importance_sampling(t_scene	*scene, int width,
	int height);
void	simulate_importance_sampling_debug(t_scene *scene, int width,
	int height);
void	free_unused_malloc_importance_sampling(t_scene *scene);
t_vec2	calc_inverse_transform_sampling_uv(t_scene *scene, uint32_t *rand);
t_vec3	calc_inverse_transform_sampling_dir(const t_vec2 *uv);
void	free_importance_sampling_malloc(t_scene *scene);

void	calc_gray_scale(t_scene *scene, int width, int height);
void	calc_raw_average(t_scene *scene, int width, int height);
void	calc_pdfs(t_scene *scene, int width, int height);
void	calc_cdf_marginal_inv(t_scene *scene, int height);
void	calc_cdf_conditional_inv(t_scene *scene, int width, int height);

void	make_importance_sampling_map(t_scene *scene);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caustic.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:27:47 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/30 14:32:07 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAUSTIC_H
# define CAUSTIC_H

#include "structs.h"

void	print_recursive(t_kd_node *node, int depth);
void	kd_tree_print(t_kd_tree* tree);
void	kd_tree_destroy(t_kd_tree *tree);
void	swap_photons(t_photon *a, t_photon *b);
float	get_pos_on_axis(t_photon *p, int axis);
char    kd_tree_build(t_kd_tree *tree, t_vector *photons_vector);
int		caustic_manager(t_minirt *minirt, int photons_to_shoot);
char	trace_single_photon_path(t_minirt *minirt, t_vec3 light_position,
	t_fcolor initial_power, t_photon *photon_out);
	
#endif
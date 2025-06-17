/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_ref.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:05:39 by madelvin          #+#    #+#             */
/*   Updated: 2025/06/11 18:28:40 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	OBJ_PARSER_REF_H
# define OBJ_PARSER_REF_H

# include "structs.h"

t_aabb	compute_custom_obj(t_custom_object *obj, char init);
t_aabb	update_custom_obj_aabb(t_custom_object *obj);
t_aabb	compute_custom_obj(t_custom_object *obj, char init);


#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelvin <madelvin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:35:11 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/22 11:41:32 by madelvin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "structs.h"

/*═════════════════════════════════════════════════════════════════════════════╗
║                                   PARSING                                    ║
╚═════════════════════════════════════════════════════════════════════════════*/

int		not_enough_el_error(char *el);
int		to_many_el_error(char *el);
int		print_line_error(int nb, char *line);
int		invalid_struct_error(t_objects type, char **splited);
int		invalid_float_error(char **splited, int i);
int		invalid_size_error(char **splited);
int		texture_error(int error, char **parts);
int		material_error(int error, char **parts);
int		material_item_error(int error, char *part);
int		texture_item_error(int error, char *part);

/**
 * @return 1 if the line start by `type` folowed by a space or tab, 0 otherwise
 */
int		cmp_type(const char *type, const char *line);

int		is_empty_line(char *line);

/**
 * @return -1 for an invalid line, 0 for an empty line, 1 for a valid line
 */
int		is_valid_line(char *line);

/**
 * @return 1 valid, -1 incorrect characters, -2 to long
 */
int		is_valid_variable_name(char *str);

/**
 * @return 1 valid, 0 already used, -1 incorrect characters, -2 to long
 */
int		is_valid_variable_name_tex(char *str, t_scene *scene);

/**
 * @return 1 valid, 0 already used, -1 incorrect characters, -2 to long
 */
int		is_valid_variable_name_mat(char *str, t_scene *scene);

int		is_valid_element(char *line);
int		contains_var_chars(char *str);

int		count_valid_lines(char *filename);
char	*remove_useless_spaces(char *line);
char	*remove_comments(char *line);
int		valid_elements_amount(char **lines);

int		free_scene(t_scene *scene, char **lines);
int		parse_elements(t_scene *scene, char **lines, int ln_amount);

t_mat	*get_material(char *str, t_scene *scene);
int		is_valid_double_el(char *str, double *co);
int		is_valid_double_el_no_bordered(char *str, double *co);
int		is_valid_size(char *str, double *co);
int		is_valid_fov(char *str, unsigned char *fov);
int		parse_color(char *color, t_color *store);
int		parse_vector(char *str, t_vec3 *vec);
int		parse_vector_normalized(char *str, t_vec3 *vec);
int		parse_color_or_mat(char *str, t_color *store, t_mat **mat, t_scene *scene);
int		parse_only_mat(char *str, t_mat **mat, t_scene *scene);
int		parse_color_or_tex(char *str, t_color *store, t_tex **tex, t_scene *scene);
int		parse_double_b_or_tex(char *str, double *d, t_tex **tex, t_scene *scene);
int		parse_double_or_tex(char *str, double *d, t_tex **tex, t_scene *scene);
t_tex	*get_texture(char *str, t_scene *scene);
void	get_texture_image(t_tex *tex, char *filename);
int		is_extension(char *filename, char *extension);
int		is_valid_positive_int(char *str, int *res);


int		parse_ambiant_light(t_scene *scene, char *line);
int		parse_camera(t_scene *scene, char *line);
int		parse_light(t_scene *scene, char *line);
int		parse_dlight(t_scene *scene, char *line);
int		parse_sphere(t_scene *scene, char *line);
int		parse_plane(t_scene *scene, char *line);
int		parse_cylinder(t_scene *scene, char *line);
int		parse_texture(t_scene *scene, char *line);
int		parse_material(t_scene *scene, char *line);
int		parse_cone(t_scene *scene, char *line);
int		parse_hyperboloid(t_scene *scene, char *line);
int		parse_obj_custom(t_scene *scene, char *line);
int		parse_win_size(t_scene *scene, char *line);

void	bump_to_normal(t_tex_img *bump);

int		parse_scene(t_minirt *minirt, char *filename);
int		parse_ambient_light_ratio_and_color(t_scene *scene, char **parts);

void	init_scene(t_minirt *minirt);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gpu.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:19:55 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/30 09:25:13 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPU_H
# define GPU_H

# include <stdlib.h>
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include <GL/gl.h>
# include "libft.h"
# include "utils.h"
# include "gpu_struct.h"
# include "structs.h"


GLuint	compile_shader_from_files(const char **paths, int count,
	GLenum shader_type);
int		check_program_link(GLuint program);
int		check_shader_compile(GLuint shader);

void	init_ssbo(t_shader_data *shader_data, size_t render_width,
	size_t render_height);
int		init_shader(t_shader_data *shader_data, size_t render_width,
	size_t render_height, t_scene *scene);
void	compute_frame_gpu(t_minirt *minirt);

void	delete_ssbo(GLuint *ssbo);
void	clean_scene(t_gpu_structs *gpu_structs);
void	clean_shaders(t_shader_data *shader_data);
void	clean_importance_sampling(t_gpu_textures *gpu_tex);

void	fcolor_to_float3(t_fcolor *vec, float *tab);
void	color_to_float3(t_color *vec, float *tab);
void	vert_to_gpu_vert(t_vertex *ver, t_gpu_vertex *gpu_ver);
void	vec3_to_float3(t_vec3 *vec, float *tab);
void	create_ssbo(GLuint *ssbo, size_t size, void *data, long bind);

void	count_obj(t_scene *scene, t_gpu_structs *gpu_structs);
void	count_tex(t_scene *scene, t_gpu_textures *gpu_textures);

void	convert_viewport(t_gpu_viewport *dst, t_viewport *src, double ior);
void	convert_camera(t_scene *cpu_scene, t_gpu_camera *cam);
void	convert_ambiant(t_scene *cpu_scene, t_gpu_amb_light *amb, t_scene *scene);
void	convert_plane(t_scene *cpu_scene, t_gpu_plane *gpu);
void	convert_materials(t_scene *cpu_scene, t_gpu_material *gpu);
void	convert_textures(t_scene *scene, t_gpu_textures *gpu_textures);
void	convert_lights(t_scene *cpu_scene, t_gpu_structs *gpu_scene);
void	convert_bvh_node(t_scene *cpu_scene, t_gpu_bvh_node *bvh_node);

void	send_importance_sampling(t_minirt *minirt, t_scene *scene);
void	send_uniforms(t_minirt *minirt);
int		convert_scene(t_minirt *minirt, t_scene *scene, t_viewport *viewport, t_gpu_structs *gpu_structs);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: delmath <delmath@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:02:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/01 17:00:09 by delmath          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

#define SOURCES_AMOUNT 24
#define SHD "src/shaders/"
#define SHDPT "src/shaders/path_trace/"

// SHD"structs.comp",
// SHD"bind.comp",
// SHD"utils/random.comp",
// SHD"utils/defocus.comp",
// SHDPT"importance_sampling.comp",
// SHDPT"textures/sample_texture.comp",
// SHDPT"textures/get_tex_color.comp",
// SHDPT"textures/apply_maps.comp",
// SHDPT"hit_register/ray.comp",
// SHDPT"hit_register/normal.comp",
// SHDPT"hit_register/quadratic.comp",
// SHDPT"hit_register/sphere.comp",
// SHDPT"hit_register/cylinder.comp",
// SHDPT"hit_register/cone.comp",
// SHDPT"hit_register/hyperboloid.comp",
// SHDPT"hit_register/triangle.comp",
// SHDPT"hit_register/plane.comp",
// SHDPT"hit_register/hit_bvh.comp",
// SHDPT"hit_register/hit_register.comp",
// SHDPT"skybox.comp",
// SHDPT"material/utils.comp",
// SHDPT"light/p_light.comp",
// SHDPT"light/d_light.comp",
// SHDPT"light/light_manager.comp",
// SHDPT"material/default.comp",
// SHDPT"material/metallic.comp",
// SHDPT"material/refraction.comp",
// SHDPT"material/dielectric.comp",
// SHDPT"material/manager.comp",
// SHDPT"path_trace.comp",
// SHD"shader.comp"

const char	**get_shaders_sources(void)
{
	static const char	*sources[SOURCES_AMOUNT] = {
		SHD"structs.comp",
		SHD"bind.comp",
		SHD"utils/random.comp",
		SHD"utils/defocus.comp",
		SHDPT"importance_sampling.comp",
		SHDPT"textures/sample_texture.comp",
		SHDPT"textures/get_tex_color.comp",
		SHDPT"textures/apply_maps.comp",
		SHDPT"hit_register/ray.comp",
		SHDPT"hit_register/normal.comp",
		SHDPT"hit_register/quadratic.comp",
		SHDPT"hit_register/sphere.comp",
		SHDPT"hit_register/cylinder.comp",
		SHDPT"hit_register/cone.comp",
		SHDPT"hit_register/hyperboloid.comp",
		SHDPT"hit_register/triangle.comp",
		SHDPT"hit_register/plane.comp",
		SHDPT"hit_register/hit_bvh.comp",
		SHDPT"hit_register/hit_register.comp",
		SHDPT"skybox.comp",
		SHDPT"material/utils.comp",
		SHDPT"material/manager_opti.comp",
		SHDPT"path_trace.comp",
		SHD"shader.comp"
	};
	return (sources);
}

int	create_program(t_shader_data *shader_data)
{
	GLuint			cs;
	const char		**sources = get_shaders_sources();
	const ssize_t	time_start = get_cpu_time();

	printf("\nStart compiling shaders (it can take a lot of time)\n");
	cs = compile_shader_from_files(sources, SOURCES_AMOUNT, GL_COMPUTE_SHADER);
	if (check_shader_compile(cs) == -1)
		return (-1);
	shader_data->program = glCreateProgram();
	glAttachShader(shader_data->program, cs);
	glLinkProgram(shader_data->program);
	glDeleteShader(cs);
	if (check_program_link(shader_data->program) == -1)
		return (-1);
	printf("Shaders compilation successfull in %.2fs\n",
		(get_cpu_time() - time_start) / 1000.0);
	return (0);
}

void	init_ssbo(t_shader_data *shader_data, size_t render_width,
	size_t render_height)
{
	glGenBuffers(1, &shader_data->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader_data->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		render_height * render_width * sizeof(float) * 4, NULL,
		GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shader_data->ssbo);
}

static int	init_shader_p2(t_shader_data *shader_data, size_t render_width,
	size_t render_height, t_scene *scene)
{
	if (glewInit() != GLEW_OK)
	{
		glfwDestroyWindow(shader_data->window);
		glfwTerminate();
		glDeleteProgram(shader_data->program);
		return (print_errorm1("Failed to initialize GLEW"));
	}
	if (create_program(shader_data) == -1)
	{
		glfwDestroyWindow(shader_data->window);
		glfwTerminate();
		return (-1);
	}
	init_ssbo(shader_data, render_width, render_height);
	convert_textures_init(scene, &shader_data->tex);
	return (0);
}

int	init_shader(t_shader_data *shader_data, size_t render_width,
	size_t render_height, t_scene *scene)
{
	if (!glfwInit())
		return (print_errorm1("Failed to init GLFW"));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	shader_data->window = glfwCreateWindow(1, 1, "Compute Shader", NULL, NULL);
	if (!shader_data->window)
	{
		glfwTerminate();
		return (print_errorm1("Failed to create GLFW window"));
	}
	glfwMakeContextCurrent(shader_data->window);
	if (!glfwGetCurrentContext())
	{
		glfwDestroyWindow(shader_data->window);
		glfwTerminate();
		return (print_errorm1("Error: No valid OpenGL context"));
	}
	if (init_shader_p2(shader_data, render_width, render_height, scene) == -1)
		return (-1);
	return (0);
}

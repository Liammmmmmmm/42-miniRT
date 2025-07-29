/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 15:02:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 09:27:25 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"

int	check_shader_compile(GLuint shader)
{
	GLint	success;
	char	log[512];

	success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success)
		return (0);
	glGetShaderInfoLog(shader, 512, NULL, log);
	ft_dprintf(2, RED"[Error]"NC" Shader compile error: %s\n", log);
	glDeleteShader(shader);
	return (-1);
}

int	check_program_link(GLuint program)
{
	GLint	success;
	char	log[512];

	success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success)
		return (0);
	glGetProgramInfoLog(program, 512, NULL, log);
	ft_dprintf(2, RED"[Error]"NC" Program link error: %s\n", log);
	glDeleteProgram(program);
	return (-1);
}

GLuint	compile_step(char **sources, int count, GLenum shader_type)
{
	const char	**const_sources = (const char **)sources;
	int			i;
	GLuint		shader;

	shader = glCreateShader(shader_type);
	glShaderSource(shader, count, const_sources, NULL);
	glCompileShader(shader);
	i = -1;
	while (++i < count)
		free(sources[i]);
	free(sources);
	return (shader);
}

int	print_shader_sources(char **sources, int count, const char **paths)
{
	int		line_num;
	int		k;
	char	*p;

	line_num = 1;
	printf("\n--- Combined Shader Source ---\n");
	k = -1;
	while (++k < count)
	{
		printf("--- Source from: %s ---\n", paths[k]);
		p = sources[k];
		if (!p)
			continue ;
		while (*p)
		{
			printf("%4d: ", line_num++);
			while (*p && *p != '\n')
			{
				printf("%c", *p);
				p++;
			}
			printf("\n");
			if (*p)
				p++;
		}
	}
	printf("--- End of Shader Source ---\n\n");
	return (0);
}

GLuint	compile_shader_from_files(const char **paths, int count,
	GLenum shader_type)
{
	char	**sources;
	t_bin	tmp;
	int		i;
	int		j;
	GLuint	shader;

	sources = malloc(sizeof(char *) * count);
	if (!sources)
		return (0);
	i = -1;
	while (++i < count) {
		if (read_bin_file(&tmp, paths[i]) == 0)
		{
			j = -1;
			while (++j < i)
				free(sources[j]);
			free(sources);
			return (print_errorm1("Error reading the shaders files."));
		}
		sources[i] = (char *)tmp.data;
	}
	// print_shader_sources(sources, count, paths);
	shader = compile_step(sources, count, shader_type);
	if (check_shader_compile(shader) == -1)
		return (0);
	return (shader);
}

#define SOURCES_AMOUNT 31

int	create_program(t_shader_data *shader_data)
{
	GLuint	cs;
	const char	*sources[SOURCES_AMOUNT] = {
		"src/shaders/structs.comp",
		"src/shaders/bind.comp",
		"src/shaders/utils/random.comp",
		"src/shaders/utils/defocus.comp",
		"src/shaders/path_trace/importance_sampling.comp",
		"src/shaders/path_trace/textures/sample_texture.comp",
		"src/shaders/path_trace/textures/get_tex_color.comp",
		"src/shaders/path_trace/textures/apply_maps.comp",
		"src/shaders/path_trace/hit_register/ray.comp",
		"src/shaders/path_trace/hit_register/normal.comp",
		"src/shaders/path_trace/hit_register/quadratic.comp",
		"src/shaders/path_trace/hit_register/sphere.comp",
		"src/shaders/path_trace/hit_register/cylinder.comp",
		"src/shaders/path_trace/hit_register/cone.comp",
		"src/shaders/path_trace/hit_register/hyperboloid.comp",
		"src/shaders/path_trace/hit_register/triangle.comp",
		"src/shaders/path_trace/hit_register/plane.comp",
		"src/shaders/path_trace/hit_register/hit_bvh.comp",
		"src/shaders/path_trace/hit_register/hit_register.comp",
		"src/shaders/path_trace/skybox.comp",
		"src/shaders/path_trace/material/utils.comp",
		"src/shaders/path_trace/light/p_light.comp",
		"src/shaders/path_trace/light/d_light.comp",
		"src/shaders/path_trace/light/light_manager.comp",
		"src/shaders/path_trace/material/default.comp",
		"src/shaders/path_trace/material/metallic.comp",
		"src/shaders/path_trace/material/refraction.comp",
		"src/shaders/path_trace/material/dielectric.comp",
		"src/shaders/path_trace/material/manager.comp",
		"src/shaders/path_trace/path_trace.comp",
		"src/shaders/shader.comp"
	};

	ssize_t	time_start = get_cpu_time();
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
	printf("Shaders compilation successfull in %.2fs\n", (get_cpu_time() - time_start) / 1000.0);
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
	if (!glfwGetCurrentContext()) {
		glfwDestroyWindow(shader_data->window);
		glfwTerminate();
		return (print_errorm1("Error: No valid OpenGL context"));
	}
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

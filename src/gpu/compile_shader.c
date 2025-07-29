/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compile_shader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:30:30 by lilefebv          #+#    #+#             */
/*   Updated: 2025/07/29 16:36:09 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "libft.h"

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

static GLuint	compile_step(char **sources, int count, GLenum shader_type)
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

// int	print_shader_sources(char **sources, int count, const char **paths)
// {
// 	int		line_num;
// 	int		k;
// 	char	*p;

// 	line_num = 1;
// 	printf("\n--- Combined Shader Source ---\n");
// 	k = -1;
// 	while (++k < count)
// 	{
// 		printf("--- Source from: %s ---\n", paths[k]);
// 		p = sources[k];
// 		if (!p)
// 			continue ;
// 		while (*p)
// 		{
// 			printf("%4d: ", line_num++);
// 			while (*p && *p != '\n')
// 			{
// 				printf("%c", *p);
// 				p++;
// 			}
// 			printf("\n");
// 			if (*p)
// 				p++;
// 		}
// 	}
// 	printf("--- End of Shader Source ---\n\n");
// 	return (0);
// }

GLuint	compile_shader_from_files(const char **paths, int count,
	GLenum shader_type)
{
	char	**sources;
	t_bin	tmp;
	int		i;
	GLuint	shader;

	sources = malloc(sizeof(char *) * count);
	if (!sources)
		return (0);
	i = -1;
	while (++i < count)
	{
		if (read_bin_file(&tmp, paths[i]) == 0)
		{
			ft_free_tab(sources, i);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 13:10:02 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/07 16:24:34 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "gpu.h"
#include "network.h"
#include "utils.h"
#include "minirt.h"
#include <signal.h>

static int	verify_server_connection(int *sockfd, t_minirt *minirt)
{
	if (!is_connection_alive(*sockfd))
	{
		printf("Server disconnected\n");
		if (passive_mode(sockfd, minirt) < 0)
			return (print_errorm1("failed to enable passive mode"));
		else
			minirt->screen.start_render = 0;
	}
	return (0);
}

int	read_stdin(fd_set *readfds)
{
	char	*line;

	if (!FD_ISSET(STDIN_FILENO, readfds))
		return (0);
	line = get_next_line(STDIN_FILENO);
	if (!line || ft_strcmp("exit\n", line) == 0)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}

int	recv_error_client(int *sockfd, t_minirt *minirt)
{
	*sockfd = -1;
	if (passive_mode(sockfd, minirt) < 0)
		return (print_errorm1("failed to enable passive mode"));
	else
		return (1);
}

static int	read_socket(int *sockfd, fd_set *readfds, t_minirt *minirt)
{
	char	header[10];
	ssize_t	received;
	char	*data;

	if (!FD_ISSET(*sockfd, readfds))
		return (0);
	received = recv(*sockfd, header, 10, 0);
	if (received <= 0)
		recv_error_client(sockfd, minirt);
	if (received != 10)
		return (0);
	
	uint64_t	bytes_to_read;
	uint16_t	type;
	
	bytes_to_read = *(uint64_t *)header;
	type = *(uint16_t *)(header + 8);

	printf("Received %u (%lu bytes)\n", type, bytes_to_read);
	
	glUseProgram(minirt->shaders_data.program);

	if (bytes_to_read == 0)
	{
		if (type == SRV_COMPUTE_START)
		{
			delete_ssbo(&minirt->shaders_data.ssbo);
			init_ssbo(&minirt->shaders_data, minirt->scene.render_width, minirt->scene.render_height);
			free(minirt->screen.client_accumulation);
			minirt->screen.client_accumulation = ft_calloc(sizeof(float), 3 * minirt->scene.render_width * minirt->scene.render_height);
			if (minirt->screen.client_accumulation)
				minirt->screen.start_render = 1;
			minirt->screen.client_samples = 0;
			minirt->screen.client_last_sample_send = get_cpu_time();
		}
		else if (type == SRV_COMPUTE_STOP)
			minirt->screen.start_render = 0;
		return (0);
	}

	data = recv_large_data(*sockfd, bytes_to_read);
	if (!data)
		return (0);

	if (type == SRV_CHECKERS)
	{
		delete_ssbo(&minirt->shaders_data.tex.checkers_ssbo);
		create_ssbo(&minirt->shaders_data.tex.checkers_ssbo, bytes_to_read,
			data, SSBO_BIND_CHECKERS);
	}
	else if (type == SRV_IMAGES)
	{
		delete_ssbo(&minirt->shaders_data.tex.images_ssbo);
		create_ssbo(&minirt->shaders_data.tex.images_ssbo, bytes_to_read,
			data, SSBO_BIND_IMAGES);
	}
	else if (type == SRV_IMAGE_STREAM)
	{
		delete_ssbo(&minirt->shaders_data.tex.images_stream_ssbo);
		create_ssbo(&minirt->shaders_data.tex.images_stream_ssbo, bytes_to_read,
			data, SSBO_BIND_IMAGES_STREAM);
	}
	else if (type == SRV_TEX_TYPES_INDICES)
	{
		delete_ssbo(&minirt->shaders_data.tex.textures_types_indices_ssbo);
		create_ssbo(&minirt->shaders_data.tex.textures_types_indices_ssbo, bytes_to_read,
			data, SSBO_BIND_TEX_TYPE_INDICE);
	}
	else if (type == SRV_SKYBOX_WH && bytes_to_read == sizeof(t_point2))
	{
		minirt->scene.amb_light.srv_skybox_wh = *(t_point2 *)data;
	}
	else if (type == SRV_CDF_CONDITIONAL_INVERSE)
	{
		if (minirt->scene.amb_light.srv_skybox_wh.x && minirt->scene.amb_light.srv_skybox_wh.y)
		{
			glDeleteTextures(1, &minirt->shaders_data.tex.cdf_ci_texture2d);
			minirt->shaders_data.tex.cdf_ci_texture2d = create_texture2d_from_data(
				(float *)data, minirt->scene.amb_light.srv_skybox_wh.x,
				minirt->scene.amb_light.srv_skybox_wh.y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, minirt->shaders_data.tex.cdf_ci_texture2d);
			glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "cdf_conditional_inverse"), 0);
		}
	}
	else if (type == SRV_CDF_MARGINAL_INVERSE)
	{
		if (minirt->scene.amb_light.srv_skybox_wh.x && minirt->scene.amb_light.srv_skybox_wh.y)
		{
			glDeleteTextures(1, &minirt->shaders_data.tex.cdf_mi_texture2d);
			minirt->shaders_data.tex.cdf_mi_texture2d = create_texture1d_from_data(
				(float *)data, minirt->scene.amb_light.srv_skybox_wh.y);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_1D, minirt->shaders_data.tex.cdf_mi_texture2d);
			glUniform1i(glGetUniformLocation(minirt->shaders_data.program,
					"cdf_marginal_inverse"), 2);
		}
	}
	else if (type == SRV_PDF_JOINT)
	{
		if (minirt->scene.amb_light.srv_skybox_wh.x && minirt->scene.amb_light.srv_skybox_wh.y)
		{
			glDeleteTextures(1, &minirt->shaders_data.tex.pdf_joint_texture2d);
			minirt->shaders_data.tex.pdf_joint_texture2d = create_texture2d_from_data(
				(float *)data, minirt->scene.amb_light.srv_skybox_wh.x,
				minirt->scene.amb_light.srv_skybox_wh.y);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, minirt->shaders_data.tex.pdf_joint_texture2d);
			glUniform1i(glGetUniformLocation(minirt->shaders_data.program, "pdf_joint"), 1);
		}
	}
	else if (type == SRV_MATERIALS)
	{
		delete_ssbo(&minirt->shaders_data.scene.mat_ssbo);
		create_ssbo(&minirt->shaders_data.scene.mat_ssbo, bytes_to_read,
			data, SSBO_BIND_MATERIALS);
	}
	else if (type == SRV_HYPER)
	{
		delete_ssbo(&minirt->shaders_data.scene.hypers_ssbo);
		create_ssbo(&minirt->shaders_data.scene.hypers_ssbo, bytes_to_read,
			data, SSBO_BIND_HYPERS);
	}
	else if (type == SRV_TRIANGLE)
	{
		delete_ssbo(&minirt->shaders_data.scene.triangles_ssbo);
		create_ssbo(&minirt->shaders_data.scene.triangles_ssbo, bytes_to_read,
			data, SSBO_BIND_TRIANGLES);
	}
	else if (type == SRV_LIGHTS)
	{
		delete_ssbo(&minirt->shaders_data.scene.lights_ssbo);
		create_ssbo(&minirt->shaders_data.scene.lights_ssbo, bytes_to_read,
			data, SSBO_BIND_LIGHTS);
	}
	else if (type == SRV_PRIM_TYPES_INDICES)
	{
		delete_ssbo(&minirt->shaders_data.scene.prim_types_indices_ssbo);
		create_ssbo(&minirt->shaders_data.scene.prim_types_indices_ssbo, bytes_to_read,
			data, SSBO_BIND_PRIM_TYPE_INDICE);
	}
	else if (type == SRV_PRIMITIVES)
	{
		delete_ssbo(&minirt->shaders_data.scene.primitive_ssbo);
		create_ssbo(&minirt->shaders_data.scene.primitive_ssbo, bytes_to_read,
			data, SSBO_BIND_PRIMITIVE);
	}
	else if (type == SRV_BVH_NODE)
	{
		delete_ssbo(&minirt->shaders_data.scene.bvh_node_ssbo);
		create_ssbo(&minirt->shaders_data.scene.bvh_node_ssbo, bytes_to_read,
			data, SSBO_BIND_BVH);
	}
	else if (type == SRV_PLANES)
	{
		delete_ssbo(&minirt->shaders_data.scene.planes_ssbo);
		create_ssbo(&minirt->shaders_data.scene.planes_ssbo, bytes_to_read,
			data, SSBO_BIND_PLANE);
	}
	else if (type == SRV_PHOTONS)
	{
		delete_ssbo(&minirt->shaders_data.scene.photons_ssbo);
		create_ssbo(&minirt->shaders_data.scene.photons_ssbo, bytes_to_read,
			data, SSBO_BIND_PHOTON);
	}
	else if (type == SRV_CELLS)
	{
		delete_ssbo(&minirt->shaders_data.scene.cells_ssbo);
		create_ssbo(&minirt->shaders_data.scene.cells_ssbo, bytes_to_read,
			data, SSBO_BIND_CELL);
	}
	else if (type == SRV_PHOTON_INDICES)
	{
		delete_ssbo(&minirt->shaders_data.scene.photon_indices_ssbo);
		create_ssbo(&minirt->shaders_data.scene.photon_indices_ssbo, bytes_to_read,
			data, SSBO_BIND_PHOTON_INDICE);
	}
	else if (type == SRV_CAMERA && bytes_to_read == sizeof(t_gpu_camera))
	{
		minirt->shaders_data.scene.camera = *(t_gpu_camera *)data;
		camera_uniforms(minirt->shaders_data.program, &minirt->shaders_data.scene.camera);
	}
	else if (type == SRV_AMBIANT && bytes_to_read == sizeof(t_gpu_amb_light))
	{
		minirt->shaders_data.scene.ambiant = *(t_gpu_amb_light *)data;
		ambiant_uniforms(minirt->shaders_data.program, &minirt->shaders_data.scene.ambiant);
	}
	else if (type == SRV_VIEWPORT && bytes_to_read == sizeof(t_gpu_viewport))
	{
		minirt->shaders_data.scene.viewport = *(t_gpu_viewport *)data;
		viewport_uniforms(minirt->shaders_data.program, &minirt->shaders_data.scene.viewport);
	}
	else if (type == SRV_ANIM_FRAME && bytes_to_read == sizeof(t_uint))
	{
		minirt->options.anim.frame_i = *(t_uint *)data;
	}
	else if (type == SRV_WIDTH_RENDER && bytes_to_read == sizeof(int))
	{
		minirt->scene.render_width = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "width_render"),
			minirt->scene.render_width);
	}
	else if (type == SRV_HEIGHT_RENDER && bytes_to_read == sizeof(int))
	{
		minirt->scene.render_height = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "height_render"),
			minirt->scene.render_height);
	}
	else if (type == SRV_MAX_BOUNCES && bytes_to_read == sizeof(double))
	{
		minirt->controls.max_bounces = *(double *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "max_bounces"),
			minirt->controls.max_bounces);
	}
	else if (type == SRV_RENDER_MODE && bytes_to_read == sizeof(char))
	{
		minirt->render_mode = *(char *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "render_mode"),
			minirt->render_mode);
	}
	else if (type == SRV_PLANES_AM && bytes_to_read == sizeof(int))
	{
		minirt->shaders_data.scene.planes_am = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "planes_am"),
			minirt->shaders_data.scene.planes_am);
	}
	else if (type == SRV_LIGHTS_AM && bytes_to_read == sizeof(int))
	{
		minirt->shaders_data.scene.lights_am = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "lights_am"),
			minirt->shaders_data.scene.lights_am);
	}
	else if (type == SRV_GRID_WORLD_MIN && bytes_to_read == sizeof(float) * 3)
	{
		minirt->shaders_data.scene.grid_world_min[0] = *(float *)data;
		minirt->shaders_data.scene.grid_world_min[1] = *((float *)data + 1);
		minirt->shaders_data.scene.grid_world_min[2] = *((float *)data + 2);
		glUniform3fv(glGetUniformLocation(minirt->shaders_data.program, "grid_world_min"),
			1, minirt->shaders_data.scene.grid_world_min);
	}
	else if (type == SRV_CELL_SIZE && bytes_to_read == sizeof(float))
	{
		minirt->shaders_data.scene.cell_size = *(float *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "cell_size"),
			minirt->shaders_data.scene.cell_size);
	}
	else if (type == SRV_TABLE_SIZE && bytes_to_read == sizeof(int))
	{
		minirt->shaders_data.scene.table_size = *(int *)data;
		glUniform1ui(glGetUniformLocation(minirt->shaders_data.program, "table_size"),
			minirt->shaders_data.scene.table_size);
	}

	free(data);
	
	return (0);
}

static int	listen_stdin_and_socket(int *sockfd, t_minirt *minirt)
{
	fd_set			readfds;
	struct timeval	select_timeout;
	int				ret;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);
	FD_SET(*sockfd, &readfds);
	select_timeout = (struct timeval){.tv_sec = 1, .tv_usec = 0};
	if (select(*sockfd + 1, &readfds, NULL, NULL, &select_timeout) < 0)
		return (print_errorm1("select error"));
	ret = read_stdin(&readfds);
	if (ret == -1)
		return (-1);
	else if (ret == 1)
		return (1);
	ret = read_socket(sockfd, &readfds, minirt);
	if (ret == -1)
		return (-1);
	else if (ret == 1 && *sockfd == -1)
		return (-1);
	return (0);
}

void	calc_sample_for_server(t_minirt *minirt, int *sockfd)
{
	if (!minirt->screen.start_render)
		return ;
	printf("Called to calculate a frame\n");
	minirt->options.client.sockfd = sockfd;
	minirt->screen.sample = 1;
	render(minirt);
}

void	do_nothing(int sig)
{
	(void)sig;
	print_error("Server disconnected while sending a frame");
}

void	connect_client(char *ip, int port, char *password, t_minirt *minirt)
{
	int	sockfd;
	int	keepalive;

	minirt->screen.start_render = 0;
	minirt->screen.client_samples = 0;
	signal(SIGPIPE, do_nothing);
	if (active_mode(&sockfd, ip, port, password) < 0)
		if (passive_mode(&sockfd, minirt) < 0)
			return ((void)print_error("Failed to enable passive mode"));
	keepalive = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
	while (1)
	{
		if (verify_server_connection(&sockfd, minirt))
			break ;
		if (listen_stdin_and_socket(&sockfd, minirt) < 0)
			break ;
		calc_sample_for_server(minirt, &sockfd);
	}
	close(sockfd);
}

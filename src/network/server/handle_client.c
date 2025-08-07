/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_client.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilefebv <lilefebv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:28:59 by lilefebv          #+#    #+#             */
/*   Updated: 2025/08/07 17:39:29 by lilefebv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "network.h"
#include "utils.h"

static void	*close_connection(t_client_data data)
{
	close(data.client_fd);
	return (NULL);
}

int	receive_client_info(t_minirt *minirt, t_client_data *data, ssize_t *bytes_received, char *c_ip)
{
	char		header_buff[14];
	uint64_t	bytes_to_read;
	uint16_t	samples;
	uint32_t	anime_i;
	char		*data_buff;
	float		*data_float;

	int	tpx = minirt->scene.render_height * minirt->scene.render_width;
	
	*bytes_received = recv(data->client_fd, header_buff, 14, 0);
	if (*bytes_received <= 0)
		return (-1);
	if (*bytes_received != 14)
		return (0);
	bytes_to_read = *(uint64_t *)header_buff;
	samples = *(uint16_t *)(header_buff + 8);
	anime_i = *(uint32_t *)(header_buff + 10);

	if (bytes_to_read > (uint64_t)(tpx * 4 * 3))
		return (0); // Secure on empeche un boug de faire malloc 17go au serveur
		
	data_buff = recv_large_data(data->client_fd, bytes_to_read);
	if (!data_buff)
		return (0);
	
	*bytes_received = bytes_to_read; // decompression step

	printf("Received frame %u instead of %u\n", anime_i, minirt->options.anim.frame_i);
		
	if (*bytes_received != tpx * 4 * 3 || anime_i != minirt->options.anim.frame_i)
	{
		free(data_buff);
		return (0);
	}

	data_float = (float *)data_buff;

	pthread_mutex_lock(&minirt->screen.sample_mutex);

	int	i = -1;
	while (++i < tpx)
	{
		minirt->screen.float_render[i].r += data_float[i * 3];
		minirt->screen.float_render[i].g += data_float[i * 3 + 1];
		minirt->screen.float_render[i].b += data_float[i * 3 + 2];
	}

	minirt->screen.sample += samples;
	minirt->screen.sample_total_anim++;
	minirt->screen.last_sample_am = minirt->screen.sample;

	pthread_mutex_unlock(&minirt->screen.sample_mutex);

	free(data_buff);
	
	printf("Successfully recieved %u samples from %s (%lu bytes)\n", samples, c_ip, bytes_to_read);
	
	// send(data->client_fd, "Ok bro j'ai recu\0", 18, 0);
	return (0);
}

void	send_changing_map_part(t_minirt *minirt, int *fd)
{
	t_gpu_structs *gsc = &minirt->shaders_data.scene;

	send_scene_data(fd, NULL, 0, SRV_COMPUTE_STOP);
	send_scene_data(fd, (char *)&gsc->camera, sizeof(t_gpu_camera), SRV_CAMERA);
	send_scene_data(fd, (char *)&gsc->ambiant, sizeof(t_gpu_amb_light), SRV_AMBIANT);
	send_scene_data(fd, (char *)&gsc->viewport, sizeof(t_gpu_viewport), SRV_VIEWPORT);

	send_scene_data(fd, (char *)&minirt->options.anim.frame_i, sizeof(t_uint), SRV_ANIM_FRAME);

	send_scene_data(fd, (char *)gsc->hypers, sizeof(t_gpu_hyper) * gsc->hypers_am, SRV_HYPER);
	send_scene_data(fd, (char *)gsc->triangles, sizeof(t_gpu_triangle) * gsc->triangles_am, SRV_TRIANGLE);
	send_scene_data(fd, (char *)gsc->lights, sizeof(t_gpu_light) * gsc->lights_am, SRV_LIGHTS);
	send_scene_data(fd, (char *)gsc->prim_types_indices, sizeof(t_type_indice) * gsc->prim_indice_am, SRV_PRIM_TYPES_INDICES);
	send_scene_data(fd, (char *)gsc->primitives, sizeof(t_gpu_primitive) * gsc->primitive_am, SRV_PRIMITIVES);
	send_scene_data(fd, (char *)gsc->bvh_node, sizeof(t_gpu_bvh_node) * gsc->bvh_node_am, SRV_BVH_NODE);
	send_scene_data(fd, (char *)gsc->planes, sizeof(t_gpu_plane) * gsc->planes_am, SRV_PLANES);
	send_scene_data(fd, (char *)gsc->photons, sizeof(t_gpu_photon) * gsc->photon_am, SRV_PHOTONS);
	send_scene_data(fd, (char *)gsc->cells, sizeof(t_gpu_cell) * gsc->cells_am, SRV_CELLS);
	send_scene_data(fd, (char *)gsc->photon_indices, sizeof(uint32_t) * gsc->photon_am, SRV_PHOTON_INDICES);

	send_scene_data(fd, (char *)&minirt->controls.max_bounces, sizeof(double), SRV_MAX_BOUNCES);
	send_scene_data(fd, (char *)&minirt->render_mode, sizeof(char), SRV_RENDER_MODE);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.planes_am, sizeof(int), SRV_PLANES_AM);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.lights_am, sizeof(int), SRV_LIGHTS_AM);
	send_scene_data(fd, (char *)minirt->shaders_data.scene.grid_world_min, sizeof(float) * 3, SRV_GRID_WORLD_MIN);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.cell_size, sizeof(float), SRV_CELL_SIZE);
	send_scene_data(fd, (char *)&minirt->shaders_data.scene.table_size, sizeof(int), SRV_TABLE_SIZE);

	send_scene_data(fd, NULL, 0, SRV_COMPUTE_START);
}

void	*handle_client(void *arg)
{
	t_client_data	data;
	char			c_ip[INET_ADDRSTRLEN];
	ssize_t			bytes_received;
	pthread_t		monitoring;
	
	data = *(t_client_data *)arg;
	data.monitor = 1;
	free(arg);
	inet_ntop(AF_INET, &(data.client_addr.sin_addr), c_ip, INET_ADDRSTRLEN);
	printf("Thread: New client %s\n", c_ip);
	if (password_connexion(&data, data.client_fd) < 0)
	{
		print_error("Client password connection failed");
		return (close_connection(data));
	}

	printf("SEND THE MAP\n");

	// ---- SEND MAP ---- //
	t_minirt *minirt = data.minirt;
	t_gpu_textures *gtx = &minirt->shaders_data.tex;
	t_gpu_structs *gsc = &minirt->shaders_data.scene;
	

	send_scene_data(&data.client_fd, (char *)gtx->checkers, sizeof(t_gpu_checker) * gtx->checkers_am, SRV_CHECKERS);
	send_scene_data(&data.client_fd, (char *)gtx->images, sizeof(t_gpu_tex_data) * gtx->images_am, SRV_IMAGES);
	send_scene_data(&data.client_fd, (char *)gtx->images_stream, sizeof(float) * gtx->total_pixel_images * 4, SRV_IMAGE_STREAM);
	send_scene_data(&data.client_fd, (char *)gtx->textures_types_indices, sizeof(t_type_indice) * minirt->scene.tex_amount, SRV_TEX_TYPES_INDICES);

	if (minirt->scene.amb_light.skybox_t && minirt->scene.amb_light.skybox_t->hdr.pixels)
	{
		t_point2	p;
		
		p.x = minirt->scene.amb_light.skybox_t->hdr.width;
		p.y = minirt->scene.amb_light.skybox_t->hdr.height;
		send_scene_data(&data.client_fd, (char *)&p, sizeof(t_point2), SRV_SKYBOX_WH);
		send_scene_data(&data.client_fd, (char *)minirt->scene.amb_light.cdf_conditional_inverse, sizeof(float) * p.x * p.y, SRV_CDF_CONDITIONAL_INVERSE);
		send_scene_data(&data.client_fd, (char *)minirt->scene.amb_light.cdf_marginal_inverse, sizeof(float) * p.y, SRV_CDF_MARGINAL_INVERSE);
		send_scene_data(&data.client_fd, (char *)minirt->scene.amb_light.pdf_joint, sizeof(float) * p.x * p.y, SRV_PDF_JOINT);
	}

	send_scene_data(&data.client_fd, (char *)&minirt->scene.render_width, sizeof(int), SRV_WIDTH_RENDER);
	send_scene_data(&data.client_fd, (char *)&minirt->scene.render_height, sizeof(int), SRV_HEIGHT_RENDER);

	send_scene_data(&data.client_fd, (char *)gsc->mat, sizeof(t_gpu_material) * gsc->mat_am, SRV_MATERIALS);

	send_changing_map_part(minirt, &data.client_fd);

	// ---- SEND MAP ---- //

	if (pthread_create(&monitoring, NULL, client_monitoring, (void*)&data) != 0)
	{
		close(data.client_fd);
		print_error("pthread_create failed");
		return (NULL);
	}

	while (g_server_fd != -1)
	{
		if (receive_client_info(data.minirt, &data, &bytes_received, c_ip) < 0)
			break ;
	}
	data.monitor = 0;
	pthread_join(monitoring, NULL);
	if (bytes_received == 0)
		printf("Client %s disconnected\n", c_ip);
	else
		print_error("recv error");
	return (close_connection(data));
}

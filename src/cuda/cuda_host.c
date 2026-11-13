/*
** cuda_host.c - CUDA replacements of src/gpu/init_shader.c and of the
** dispatch part of src/gpu/use_shader.c (make CUDA=1).
*/

#include "gpu.h"
#include "minirt.h"

void	init_ssbo(t_shader_data *shader_data, size_t render_width,
	size_t render_height)
{
	shader_data->ssbo = rt_cuda_buffer_set(RT_SLOT_OUTPUT,
			render_height * render_width * sizeof(float) * 4, NULL);
}

int	init_shader(t_shader_data *shader_data, size_t render_width,
	size_t render_height, t_scene *scene)
{
	if (rt_cuda_init() != 0)
		return (print_errorm1("Failed to init the CUDA backend"));
	init_ssbo(shader_data, render_width, render_height);
	convert_textures_init(scene, &shader_data->tex);
	return (0);
}

/* Gather what the compute shader received as uniforms and run one sample */
int	cuda_compute_frame(t_minirt *m)
{
	t_cuda_frame	frame;

	ft_bzero(&frame, sizeof(t_cuda_frame));
	frame.viewport = m->shaders_data.scene.viewport;
	frame.camera = m->shaders_data.scene.camera;
	frame.ambiant = m->shaders_data.scene.ambiant;
	frame.width = m->scene.render_width;
	frame.height = m->scene.render_height;
	frame.max_bounces = (uint32_t)m->controls.max_bounces;
	frame.render_mode = (uint32_t)m->render_mode;
	frame.planes_am = (uint32_t)m->shaders_data.scene.planes_am;
	frame.lights_am = (uint32_t)m->shaders_data.scene.lights_am;
	frame.grid_world_min[0] = m->shaders_data.scene.grid_world_min[0];
	frame.grid_world_min[1] = m->shaders_data.scene.grid_world_min[1];
	frame.grid_world_min[2] = m->shaders_data.scene.grid_world_min[2];
	frame.cell_size = m->shaders_data.scene.cell_size;
	frame.table_size = m->shaders_data.scene.table_size;
	if (m->options.client.enabled)
		frame.sample_count = rand();
	else
		frame.sample_count = m->screen.sample;
	if (rt_cuda_render(&frame) != 0)
		return (print_error1("CUDA render failed"));
	return (0);
}

/*
** rt_cuda.h - C interface of the CUDA backend (make CUDA=1).
**
** The backend replaces the OpenGL compute shader: the host code keeps building
** the same t_gpu_* arrays, they are uploaded into numbered slots (the old SSBO
** binding points) and the kernel in src/cuda/ reads them.
** The gl* macros at the bottom let the files written for OpenGL (uniforms,
** network client, cleanup) compile unchanged: uniforms are no-ops because the
** kernel gets its parameters from t_cuda_frame at each frame.
*/

#ifndef RT_CUDA_H
# define RT_CUDA_H

# include <stdint.h>
# include <stddef.h>
# include "gpu_scene.h"

# define RT_SLOT_OUTPUT 1

/* Everything that was a uniform in the compute shader */
typedef struct s_cuda_frame
{
	t_gpu_viewport	viewport;
	t_gpu_camera	camera;
	t_gpu_amb_light	ambiant;
	uint32_t		width;
	uint32_t		height;
	uint32_t		max_bounces;
	uint32_t		render_mode;
	uint32_t		planes_am;
	uint32_t		lights_am;
	uint32_t		sample_count;
	float			grid_world_min[3];
	float			cell_size;
	int				table_size;
}	t_cuda_frame;

# ifdef __cplusplus

extern "C" {
# endif

int				rt_cuda_init(void);
void			rt_cuda_shutdown(void);
/* (Re)create the buffer of a slot. data may be NULL. Returns a handle != 0 */
unsigned int	rt_cuda_buffer_set(long slot, size_t size, const void *data);
void			rt_cuda_buffer_free(unsigned int handle);
/* Float textures of the HDR importance sampling (unit 0/1/2 as in OpenGL) */
unsigned int	rt_cuda_tex_create(const float *data, int width, int height);
void			rt_cuda_tex_delete(unsigned int id);
void			rt_cuda_tex_unit(int unit);
void			rt_cuda_tex_bind(unsigned int id);
/* Render one sample per pixel. Returns 0 on success */
int				rt_cuda_render(const t_cuda_frame *frame);
/* width * height * 4 floats (rgb + padding) of the last rendered sample */
const float		*rt_cuda_result(void);

# ifdef __cplusplus

}
# endif

# ifndef RT_CUDA_KERNEL

#  define GL_SHADER_STORAGE_BUFFER 0
#  define GL_SHADER_STORAGE_BARRIER_BIT 0
#  define GL_MAP_READ_BIT 0
#  define GL_TEXTURE_1D 0
#  define GL_TEXTURE_2D 0
#  define GL_TEXTURE0 0
#  define GL_TEXTURE1 1
#  define GL_TEXTURE2 2

#  define glUseProgram(p) ((void)(p))
#  define glDeleteProgram(p) ((void)(p))
#  define glGetUniformLocation(p, name) ((void)(p), 0)
#  define glUniform1ui(loc, v) ((void)(loc), (void)(v))
#  define glUniform1i(loc, v) ((void)(loc), (void)(v))
#  define glUniform1f(loc, v) ((void)(loc), (void)(v))
#  define glUniform3fv(loc, n, v) ((void)(loc), (void)(v))
#  define glBindBuffer(target, buffer) ((void)(buffer))
#  define glMemoryBarrier(bits) ((void)0)
#  define glMapBufferRange(target, off, len, access) rt_cuda_result()
#  define glUnmapBuffer(target) ((void)0)
#  define glDeleteBuffers(n, p) rt_cuda_buffer_free(*(p))
#  define glDeleteTextures(n, p) rt_cuda_tex_delete(*(p))
#  define glActiveTexture(unit) rt_cuda_tex_unit(unit)
#  define glBindTexture(target, id) rt_cuda_tex_bind(id)
#  define glfwDestroyWindow(w) ((void)(w))
#  define glfwTerminate() rt_cuda_shutdown()

# endif

#endif

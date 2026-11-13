/*
** web_run.c - headless render loop + HTTP routes of the web mode.
**
** Threads: the main thread renders and is the only one touching the scene
** and the GPU. The server thread only queues commands, and takes `lock` to
** read the accumulated image or the status between two samples.
*/

#include "minirt.h"
#include "web.h"
#include <signal.h>
#include <sys/socket.h>
#include <time.h>

typedef struct s_web_state
{
	t_minirt				*minirt;
	int						listen_fd;
	pthread_t				thread;
	pthread_mutex_t			lock;
	pthread_mutex_t			cmd_lock;
	char					cmds[WEB_CMD_MAX][WEB_CMD_LEN];
	int						cmd_count;
	int						waiters;
	int						exported;
	double					last_ms;
	double					total_ms;
}	t_web_state;

static t_web_state				g_web;
static volatile sig_atomic_t	g_web_stop;

static void	on_stop_signal(int sig)
{
	(void)sig;
	g_web_stop = 1;
}

static double	now_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6);
}

static const char	*backend_name(t_minirt *m)
{
	if (m->options.cpu)
		return ("CPU");
#ifdef RT_CUDA_EMU
	return ("CUDA kernel (CPU emulation)");
#elif defined(USE_CUDA)
	return ("CUDA");
#else
	return ("OpenGL compute");
#endif
}

/* ---------------------------------------------------------------- routes */

static void	lock_for_read(void)
{
	__sync_fetch_and_add(&g_web.waiters, 1);
	pthread_mutex_lock(&g_web.lock);
}

static void	unlock_for_read(void)
{
	pthread_mutex_unlock(&g_web.lock);
	__sync_fetch_and_sub(&g_web.waiters, 1);
}

static void	route_status(int fd)
{
	t_minirt	*m;
	char		name[64];
	char		json[1024];
	int			i;

	m = g_web.minirt;
	lock_for_read();
	i = 0;
	while (m->scene.name && m->scene.name[i] && i < 63)
	{
		name[i] = m->scene.name[i];
		if (name[i] == '"' || name[i] == '\\' || (t_uchar)name[i] < 32)
			name[i] = '_';
		i++;
	}
	name[i] = '\0';
	snprintf(json, sizeof(json), "{\"scene\":\"%s\",\"backend\":\"%s\","
		"\"sample\":%d,\"spp\":%d,\"paused\":%d,\"running\":%d,"
		"\"width\":%d,\"height\":%d,\"bounces\":%d,\"mode\":%d,"
		"\"gamma\":%.3f,\"ms\":%.2f,\"elapsed\":%.0f,"
		"\"pos\":[%.4f,%.4f,%.4f],\"dir\":[%.4f,%.4f,%.4f],"
		"\"fov\":%d,\"focus\":%.4f,\"defocus\":%.4f}",
		name, backend_name(m), m->screen.sample, m->screen.spp,
		m->screen.pause_render,
		m->screen.start_render && m->screen.sample < m->screen.spp,
		m->scene.render_width, m->scene.render_height,
		(int)m->controls.max_bounces, (int)m->render_mode, m->viewport.gamma,
		g_web.last_ms, g_web.total_ms,
		m->scene.camera.position.x, m->scene.camera.position.y,
		m->scene.camera.position.z, m->scene.camera.orientation.x,
		m->scene.camera.orientation.y, m->scene.camera.orientation.z,
		m->controls.values.fov, m->scene.camera.focus_dist,
		m->scene.camera.defocus_angle);
	unlock_for_read();
	web_send(fd, "200 OK", "application/json", json, strlen(json));
}

static void	route_frame(int fd, const char *query)
{
	char	value[16];
	t_uchar	*bmp;
	size_t	size;
	int		scale;

	scale = 1;
	if (web_query_get(query, "scale", value, sizeof(value)))
		scale = ft_atoi(value);
	if (scale > 16)
		scale = 16;
	lock_for_read();
	bmp = web_encode_bmp(g_web.minirt, scale, &size);
	unlock_for_read();
	if (!bmp)
		web_send_text(fd, "503 Service Unavailable", "no frame\n");
	else
		web_send(fd, "200 OK", "image/bmp", bmp, size);
	free(bmp);
}

static void	route_index(int fd)
{
	t_bin	file;

	if (read_bin_file(&file, WEB_INDEX_PATH) == 0)
	{
		web_send_text(fd, "500 Internal Server Error", "Cannot read "
			WEB_INDEX_PATH " (run miniRT from the repository root)\n");
		return ;
	}
	web_send(fd, "200 OK", "text/html; charset=utf-8", file.data, file.size);
	free(file.data);
}

/* The custom header keeps other web sites from driving a local instance */
static void	route_cmd(int fd, int allowed, const char *query)
{
	if (!allowed)
	{
		web_send_text(fd, "403 Forbidden", "POST with X-MiniRT header\n");
		return ;
	}
	pthread_mutex_lock(&g_web.cmd_lock);
	if (g_web.cmd_count < WEB_CMD_MAX && ft_strlen(query) < WEB_CMD_LEN)
		ft_strlcpy(g_web.cmds[g_web.cmd_count++], query, WEB_CMD_LEN);
	pthread_mutex_unlock(&g_web.cmd_lock);
	web_send(fd, "200 OK", "application/json", "{\"ok\":true}", 11);
}

static void	handle_client(int fd)
{
	char	request[WEB_REQ_MAX];
	char	*path;
	char	*query;
	char	*end;
	int		cmd_allowed;

	if (web_read_request(fd, request, sizeof(request)) < 0)
		return ;
	cmd_allowed = (ft_strncmp(request, "POST ", 5) == 0
			&& web_has_header(request, "X-MiniRT"));
	path = strchr(request, ' ');
	if (!path)
		return ;
	end = strpbrk(++path, " \r\n");
	if (end)
		*end = '\0';
	query = strchr(path, '?');
	if (query)
		*query++ = '\0';
	else
		query = "";
	if (ft_strcmp(path, "/") == 0 || ft_strcmp(path, "/index.html") == 0)
		route_index(fd);
	else if (ft_strcmp(path, "/status") == 0)
		route_status(fd);
	else if (ft_strcmp(path, "/frame.bmp") == 0)
		route_frame(fd, query);
	else if (ft_strcmp(path, "/cmd") == 0)
		route_cmd(fd, cmd_allowed, query);
	else
		web_send_text(fd, "404 Not Found", "not found\n");
}

static void	*server_thread(void *arg)
{
	int	fd;

	(void)arg;
	while (!g_web_stop)
	{
		fd = accept(g_web.listen_fd, NULL, NULL);
		if (fd < 0)
		{
			if (g_web_stop)
				break ;
			usleep(10000);
			continue ;
		}
		handle_client(fd);
		close(fd);
	}
	return (NULL);
}

/* ----------------------------------------------------------- render loop */

static void	apply_commands(t_minirt *m)
{
	char	cmds[WEB_CMD_MAX][WEB_CMD_LEN];
	int		count;
	int		restart;
	int		i;

	pthread_mutex_lock(&g_web.cmd_lock);
	count = g_web.cmd_count;
	ft_memcpy(cmds, g_web.cmds, sizeof(cmds[0]) * count);
	g_web.cmd_count = 0;
	pthread_mutex_unlock(&g_web.cmd_lock);
	restart = 0;
	i = -1;
	while (++i < count)
		restart |= web_apply_command(m, cmds[i]);
	if (!restart)
		return ;
	i = m->screen.pause_render;
	restart_minirt(m);
	m->screen.pause_render = i;
	g_web.exported = 0;
	g_web.total_ms = 0;
}

/* Render one sample if there is something to do. Returns 0 when idle */
static int	render_step(t_minirt *m)
{
	const int	sample = m->screen.sample;
	double		start;

	if (!m->screen.start_render || m->screen.pause_render)
		return (0);
	if (m->screen.sample >= m->screen.spp)
	{
		if (!g_web.exported && m->options.auto_export)
			auto_export(m);
		if (!g_web.exported)
			printf("Render finished: %d samples\n", m->screen.sample);
		g_web.exported = 1;
		return (0);
	}
	start = now_ms();
	if (m->options.cpu)
		render_cpu(m);
	else
		render_gpu(m);
	g_web.last_ms = now_ms() - start;
	g_web.total_ms += g_web.last_ms;
	g_web.exported = 0;
	return (m->screen.sample != sample);
}

static void	render_loop(t_minirt *m)
{
	int	busy;
	int	spins;

	while (!g_web_stop)
	{
		pthread_mutex_lock(&g_web.lock);
		apply_commands(m);
		busy = render_step(m);
		pthread_mutex_unlock(&g_web.lock);
		if (!busy)
			usleep(20000);
		spins = 0;
		while (g_web.waiters > 0 && spins++ < 5000 && !g_web_stop)
			usleep(200);
	}
}

int	web_run(t_minirt *minirt)
{
	ft_bzero(&g_web, sizeof(g_web));
	g_web.minirt = minirt;
	g_web.listen_fd = web_listen(minirt->options.web.addr,
			minirt->options.web.port);
	if (g_web.listen_fd < 0)
		return (print_error1("web: cannot listen on the requested address"));
	pthread_mutex_init(&g_web.lock, NULL);
	pthread_mutex_init(&g_web.cmd_lock, NULL);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, on_stop_signal);
	signal(SIGTERM, on_stop_signal);
	if (pthread_create(&g_web.thread, NULL, server_thread, NULL) != 0)
	{
		close(g_web.listen_fd);
		return (print_error1("web: cannot start the server thread"));
	}
	printf("Web interface: http://%s:%d/  (backend: %s, Ctrl+C to stop)\n",
		minirt->options.web.addr, minirt->options.web.port,
		backend_name(minirt));
	if (ft_strcmp(minirt->options.web.addr, "127.0.0.1") != 0)
		print_warn("web: listening on a non local address, there is no "
			"authentication");
	render_loop(minirt);
	shutdown(g_web.listen_fd, SHUT_RDWR);
	close(g_web.listen_fd);
	pthread_join(g_web.thread, NULL);
	pthread_mutex_destroy(&g_web.lock);
	pthread_mutex_destroy(&g_web.cmd_lock);
	printf("\nWeb mode stopped\n");
	return (0);
}

/* --web=PORT or --web=ADDR:PORT (default address 127.0.0.1) */
int	parse_web_option(t_minirt *minirt, char *arg)
{
	char	*value;
	char	*colon;
	int		i;

	if (ft_strncmp(arg, "--web=", 6) != 0)
		return (print_error("Usage: --web=PORT or --web=ADDR:PORT"));
	value = arg + 6;
	ft_strlcpy(minirt->options.web.addr, "127.0.0.1",
		sizeof(minirt->options.web.addr));
	colon = strrchr(value, ':');
	if (colon && (size_t)(colon - value) < sizeof(minirt->options.web.addr)
		&& colon != value)
	{
		ft_strlcpy(minirt->options.web.addr, value, colon - value + 1);
		value = colon + 1;
	}
	i = -1;
	while (value[++i])
		if (!ft_isdigit(value[i]) || i > 5)
			return (print_error("Usage: --web=PORT or --web=ADDR:PORT"));
	minirt->options.web.port = ft_atoi(value);
	if (i == 0 || minirt->options.web.port < 1
		|| minirt->options.web.port > 65535)
		return (print_error("Usage: --web=PORT or --web=ADDR:PORT"));
	minirt->options.web.enabled = 1;
	minirt->options.no_display = 1;
	return (1);
}

/*
** web_commands.c - commands sent by the web page. They are applied by the
** render thread, between two samples.
**
**   action=pause|resume|restart|save
**   spp=N  bounces=N  mode=0|1|2  gamma=F  width=N&height=N
**   fov=N  focus=F  defocus=F
**   move=forward,right,up      (scene units, in the camera frame)
**   rot=yaw,pitch              (degrees)
**   pos=x,y,z   dir=x,y,z      (absolute)
*/

#include "minirt.h"
#include "web.h"

static int	get_numbers(const char *query, const char *key, double *out, int n)
{
	char	value[128];
	char	*cursor;
	char	*end;
	int		i;

	if (!web_query_get(query, key, value, sizeof(value)))
		return (0);
	cursor = value;
	i = -1;
	while (++i < n)
	{
		out[i] = strtod(cursor, &end);
		if (end == cursor || isnan(out[i]) || isinf(out[i]))
			return (0);
		cursor = end;
		if (*cursor == ',')
			cursor++;
	}
	return (1);
}

static t_vec3	unit(t_vec3 v)
{
	const double	len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	if (len < 1e-12)
		return ((t_vec3){0, 0, -1});
	return ((t_vec3){v.x / len, v.y / len, v.z / len});
}

static t_vec3	cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x});
}

static t_vec3	cam_right(t_vec3 dir)
{
	if (fabs(dir.y) > 0.999)
		return (unit(cross(dir, (t_vec3){0, 0, 1})));
	return (unit(cross(dir, (t_vec3){0, 1, 0})));
}

static void	cam_move(t_camera *cam, const double *d)
{
	const t_vec3	dir = unit(cam->orientation);
	const t_vec3	right = cam_right(dir);
	const t_vec3	up = cross(right, dir);

	cam->position.x += dir.x * d[0] + right.x * d[1] + up.x * d[2];
	cam->position.y += dir.y * d[0] + right.y * d[1] + up.y * d[2];
	cam->position.z += dir.z * d[0] + right.z * d[1] + up.z * d[2];
}

/* yaw around the world Y axis, then pitch around the camera right vector */
static void	cam_rotate(t_camera *cam, const double *deg)
{
	const double	yaw = -deg[0] * PI_D / 180.0;
	const double	pitch = deg[1] * PI_D / 180.0;
	t_vec3			dir;
	t_vec3			right;
	t_vec3			up;
	t_vec3			res;

	dir = unit(cam->orientation);
	res.x = dir.x * cos(yaw) + dir.z * sin(yaw);
	res.y = dir.y;
	res.z = -dir.x * sin(yaw) + dir.z * cos(yaw);
	dir = unit(res);
	right = cam_right(dir);
	up = cross(right, dir);
	res.x = dir.x * cos(pitch) + up.x * sin(pitch);
	res.y = dir.y * cos(pitch) + up.y * sin(pitch);
	res.z = dir.z * cos(pitch) + up.z * sin(pitch);
	res = unit(res);
	if (fabs(res.y) < 0.995)
		dir = res;
	cam->orientation = dir;
}

static int	camera_commands(t_minirt *m, const char *query)
{
	double	v[3];
	int		restart;

	restart = 0;
	if (get_numbers(query, "pos", v, 3) && ++restart)
		m->scene.camera.position = (t_vec3){v[0], v[1], v[2]};
	if (get_numbers(query, "dir", v, 3) && ++restart)
		m->scene.camera.orientation = unit((t_vec3){v[0], v[1], v[2]});
	if (get_numbers(query, "move", v, 3) && ++restart)
		cam_move(&m->scene.camera, v);
	if (get_numbers(query, "rot", v, 2) && ++restart)
		cam_rotate(&m->scene.camera, v);
	if (get_numbers(query, "fov", v, 1) && v[0] >= 1 && v[0] <= 179
		&& ++restart)
		m->controls.values.fov = (int)v[0];
	if (get_numbers(query, "focus", v, 1) && v[0] > 0 && ++restart)
		m->scene.camera.focus_dist = v[0];
	if (get_numbers(query, "defocus", v, 1) && v[0] >= 0 && v[0] < 180
		&& ++restart)
		m->scene.camera.defocus_angle = v[0];
	return (restart != 0);
}

static int	render_commands(t_minirt *m, const char *query)
{
	double	v[2];
	double	h;
	int		restart;

	restart = 0;
	if (get_numbers(query, "spp", v, 1) && v[0] >= 1 && v[0] < 1e9)
		m->screen.spp = (int)v[0];
	if (get_numbers(query, "gamma", v, 1) && v[0] >= 0.1 && v[0] <= 10)
		m->viewport.gamma = v[0];
	if (get_numbers(query, "bounces", v, 1) && v[0] >= 1 && v[0] <= 1000
		&& ++restart)
		m->controls.max_bounces = (int)v[0];
	if (get_numbers(query, "mode", v, 1) && v[0] >= 0 && v[0] <= 2
		&& ++restart)
		m->render_mode = (char)v[0];
	if (get_numbers(query, "width", v, 1) && get_numbers(query, "height", &h, 1)
		&& v[0] >= 2 && h >= 2 && v[0] <= WEB_MAX_RES && h <= WEB_MAX_RES
		&& ++restart)
	{
		m->controls.res_render_x = (int)v[0];
		m->controls.res_render_y = (int)h;
		free(m->viewport.depth_buffer);
		m->viewport.depth_buffer = NULL;
	}
	return (restart != 0);
}

int	web_apply_command(t_minirt *m, const char *query)
{
	char	action[16];
	int		restart;

	restart = camera_commands(m, query);
	restart |= render_commands(m, query);
	if (!web_query_get(query, "action", action, sizeof(action)))
		return (restart);
	if (ft_strcmp(action, "pause") == 0)
		m->screen.pause_render = 1;
	else if (ft_strcmp(action, "resume") == 0)
		m->screen.pause_render = 0;
	else if (ft_strcmp(action, "restart") == 0)
		restart = 1;
	else if (ft_strcmp(action, "save") == 0)
		auto_export(m);
	return (restart);
}

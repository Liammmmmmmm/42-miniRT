/*
** web.h - headless mode driven from a browser (--web=[ADDR:]PORT).
** No X window is opened: the render loop runs in the main thread and a small
** HTTP server (one thread) serves the current image and receives commands.
*/

#ifndef WEB_H
# define WEB_H

# include <stddef.h>
# include "structs.h"

# define WEB_INDEX_PATH "assets/web/index.html"
# define WEB_REQ_MAX 4096
# define WEB_CMD_MAX 64
# define WEB_CMD_LEN 512
# define WEB_MAX_RES 8192

/* option */
int		parse_web_option(t_minirt *minirt, char *arg);

/* render loop + server, returns when the process is asked to stop */
int		web_run(t_minirt *minirt);

/* web_http.c */
int		web_listen(const char *addr, int port);
int		web_read_request(int fd, char *buf, size_t size);
int		web_send(int fd, const char *status, const char *type,
			const void *body, size_t len);
int		web_send_text(int fd, const char *status, const char *text);
int		web_query_get(const char *query, const char *key, char *out,
			size_t out_size);
int		web_has_header(const char *request, const char *name);

/* web_frame.c */
t_uchar	*web_encode_bmp(t_minirt *minirt, int scale, size_t *size);

/* web_commands.c: returns 1 when the render must restart */
int		web_apply_command(t_minirt *minirt, const char *query);

#endif

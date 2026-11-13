/*
** web_http.c - the few HTTP/1.0 primitives needed by the web mode.
** One request per connection, no keep-alive.
*/

#include "minirt.h"
#include "web.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <strings.h>

int	web_listen(const char *addr, int port)
{
	struct sockaddr_in	sa;
	int					fd;
	int					one;

	ft_bzero(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	if (inet_pton(AF_INET, addr, &sa.sin_addr) != 1)
		return (-1);
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
		return (-1);
	one = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0 || listen(fd, 16) < 0)
	{
		close(fd);
		return (-1);
	}
	return (fd);
}

/* Read the request head (up to the empty line). Returns its length or -1 */
int	web_read_request(int fd, char *buf, size_t size)
{
	struct timeval	tv;
	size_t			len;
	ssize_t			got;

	tv.tv_sec = 5;
	tv.tv_usec = 0;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	len = 0;
	while (len + 1 < size)
	{
		got = recv(fd, buf + len, size - 1 - len, 0);
		if (got <= 0)
			return (-1);
		len += got;
		buf[len] = '\0';
		if (strstr(buf, "\r\n\r\n") || strstr(buf, "\n\n"))
			return ((int)len);
	}
	return (-1);
}

static int	send_all(int fd, const char *data, size_t len)
{
	ssize_t	sent;

	while (len > 0)
	{
		sent = send(fd, data, len, MSG_NOSIGNAL);
		if (sent <= 0)
			return (-1);
		data += sent;
		len -= sent;
	}
	return (0);
}

int	web_send(int fd, const char *status, const char *type, const void *body,
	size_t len)
{
	char	head[256];
	int		head_len;

	head_len = snprintf(head, sizeof(head), "HTTP/1.0 %s\r\n"
			"Content-Type: %s\r\nContent-Length: %zu\r\n"
			"Cache-Control: no-store\r\nConnection: close\r\n\r\n",
			status, type, len);
	if (send_all(fd, head, head_len) < 0)
		return (-1);
	return (send_all(fd, body, len));
}

int	web_send_text(int fd, const char *status, const char *text)
{
	return (web_send(fd, status, "text/plain; charset=utf-8", text,
			strlen(text)));
}

static int	hex_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}

/* Copy the url-decoded value of `key` from "a=1&b=2". Returns 1 if found */
int	web_query_get(const char *query, const char *key, char *out,
	size_t out_size)
{
	const size_t	key_len = strlen(key);
	size_t			i;

	while (query && *query)
	{
		if (strncmp(query, key, key_len) == 0 && query[key_len] == '=')
		{
			query += key_len + 1;
			i = 0;
			while (*query && *query != '&' && i + 1 < out_size)
			{
				if (*query == '%' && hex_value(query[1]) >= 0
					&& hex_value(query[2]) >= 0)
				{
					out[i++] = hex_value(query[1]) * 16 + hex_value(query[2]);
					query += 3;
				}
				else if (*query == '+' && query++)
					out[i++] = ' ';
				else
					out[i++] = *query++;
			}
			out[i] = '\0';
			return (1);
		}
		query = strchr(query, '&');
		if (query)
			query++;
	}
	return (0);
}

/* Is there a header line starting with `name` (case insensitive) */
int	web_has_header(const char *request, const char *name)
{
	const size_t	len = strlen(name);

	request = strchr(request, '\n');
	while (request)
	{
		request++;
		if (strncasecmp(request, name, len) == 0 && request[len] == ':')
			return (1);
		request = strchr(request, '\n');
	}
	return (0);
}

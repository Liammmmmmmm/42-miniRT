#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(8080),
		.sin_addr.s_addr = INADDR_ANY
	};

	bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
	listen(sockfd, 5);

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
		
		char buffer[1024];
		recv(client_fd, buffer, sizeof(buffer), 0);
		send(client_fd, "HTTP/1.1 200 OK\r\n\r\nHello", 20, 0);
		close(client_fd);
	}
}
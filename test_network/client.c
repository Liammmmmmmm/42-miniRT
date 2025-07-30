#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(8080),
		.sin_addr.s_addr = inet_addr("127.0.0.1")
	};
	
	connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	send(sockfd, "GET / HTTP/1.1\r\n\r\n", 18, 0);
	
	char response[1024];
	recv(sockfd, response, sizeof(response), 0);
	printf("%s\n", response);
	
	close(sockfd);
}
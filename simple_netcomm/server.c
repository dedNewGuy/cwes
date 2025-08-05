#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define CONNECTION_QUEUE_LIMIT 10
#define SERV_PORT "6969"

char recv_buffer[4096];

int main(void)
{
	memset(recv_buffer, 0, sizeof(recv_buffer));

	struct sockaddr client_addr = {0};
	socklen_t client_addr_len;
	struct addrinfo hints = {0}, *serv_info = NULL;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int ai_status = getaddrinfo(NULL,
                       SERV_PORT,
                       &hints,
                       &serv_info);
	if (ai_status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ai_status));
		return 1;
	}

	int sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
	if (sockfd < 0) {
		perror("server socket error: ");
		return 2;
	}

	int bind_status = bind(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
	if (bind_status < 0) {
		perror("server bind error: ");
		close(sockfd);
		return 3;
	}

	int listen_status = listen(sockfd, CONNECTION_QUEUE_LIMIT);
	if (listen_status < 0) {
		perror("server listen error: ");
		close(sockfd);
		return 4;
	}

	client_addr_len = sizeof(client_addr);
	int client_sock = accept(sockfd, &client_addr, &client_addr_len);
	if (client_sock < 0) {
		perror("server accept error: ");
		close(sockfd);
		return 5;
	}

	ssize_t buf_size = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);
	if (buf_size == 0) {
		fprintf(stderr, "Client has closed the connection\n");
		close(sockfd);
	} else if (buf_size < 0) {
		perror("server received error: ");
		close(sockfd);
		return 6;
	}

	printf("Message from client: %s\n", recv_buffer);

	close(sockfd);
	return 0;
}

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define CONNECTION_QUEUE_LIMIT 10
#define SERV_PORT "6969"

char recv_buffer[4096];
int err_status = 0;

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
		err_status = 1;
	}

	int sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
	if (sockfd < 0) {
		perror("server socket error: ");
		err_status = 2;
		goto cleanup;
	}

	int bind_status = bind(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
	if (bind_status < 0) {
		perror("server bind error: ");
		err_status = 3;
		goto cleanup;
	}

	int listen_status = listen(sockfd, CONNECTION_QUEUE_LIMIT);
	if (listen_status < 0) {
		perror("server listen error: ");
		err_status = 4;
		goto cleanup;
	}

	client_addr_len = sizeof(client_addr);
	int client_sock = accept(sockfd, &client_addr, &client_addr_len);
	if (client_sock < 0) {
		perror("server accept error: ");
		err_status = 5;
		goto cleanup;
	}

	ssize_t buf_size = recv(client_sock, recv_buffer, sizeof(recv_buffer), 0);
	if (buf_size == 0) {
		fprintf(stderr, "Client has closed the connection\n");
		err_status = 6;
		goto cleanup;
	} else if (buf_size < 0) {
		perror("server received error: ");
		err_status = 7;
		goto cleanup;
	}

	printf("Message from client: %s\n", recv_buffer);

	freeaddrinfo(serv_info);
	close(sockfd);
	return 0;

cleanup:
	freeaddrinfo(serv_info);
	if (sockfd != -1) {
		close(sockfd);
	}
	return err_status;
}

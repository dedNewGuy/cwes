#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "httplib.h"

#define PORT "8080"
#define BACKLOG 10

char req_buffer[4096];

int main(int argc, char **argv)
{
	memset(req_buffer, 0, sizeof req_buffer);
	if (argc != 2) {
		fprintf(stderr, "Usage: http_server <directory>\n");
		return 2;
	}

	char *dir_name = argv[1];
	printf("Hosting from directory: %s\n", dir_name);

	struct addrinfo hints = {0}, *serv_info = NULL;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int addrinfo_status = getaddrinfo(NULL,
								   PORT,
								   &hints,
								   &serv_info);
	if (addrinfo_status < 0) {
		fprintf(stderr, "%s\n", gai_strerror(addrinfo_status));
		return 1;
	}

	int sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
	if (sockfd < 0) {
		perror("Socket:");
		goto defer;
	}

	int yes = 1;

	int sockopt_status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
	if (sockopt_status < 0) {
		perror("Socket option error");
		goto defer;
	}

	int bind_status = bind(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
	if (bind_status < 0) {
		perror("Bind error");
		goto defer;
	}

	int listen_status = listen(sockfd, BACKLOG);
	if (listen_status < 0) {
		perror("Listening error");
		goto defer;
	}

	int client_sockfd = accept(sockfd, NULL, NULL);
	if (client_sockfd < 0) {
		perror("Accept error");
		goto defer;
	}

	ssize_t n_buffer = recv(client_sockfd, req_buffer, sizeof req_buffer, 0);
	if (n_buffer < 0) {
		perror("Receiving request error");
		goto defer;
	}

	struct http_request_t req = parse_http_request(req_buffer, client_sockfd);

	send_http_response(dir_name, req);

	freeaddrinfo(serv_info);
	close(sockfd);
	close(client_sockfd);
	return 0;

defer:
	freeaddrinfo(serv_info);
	if (sockfd != -1) {
		close(sockfd);
	}
	if (client_sockfd != -1) {
		close(client_sockfd);
	}
	return 1;
}

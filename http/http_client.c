#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

char buffer[256];
char recv_buffer[6000];

#define TARGET_WEBSITE "www.example.com"

int main(void)
{
	memset(recv_buffer, 0, sizeof(recv_buffer));
	strcpy(buffer, "GET / HTTP/1.1\n\
Host: www.example.com\n\r\n");

	struct addrinfo hints = {0}, *serv_info = NULL;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int ai_status = getaddrinfo(TARGET_WEBSITE,
                       "80",
                       &hints,
                       &serv_info);
	if (ai_status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ai_status));
		return 1;
	}

	int sockfd = socket(serv_info->ai_family, serv_info->ai_socktype, serv_info->ai_protocol);
	if (sockfd < 0) {
		perror("client socket error: ");
		return 2;
	}

	printf("CONNECTING...\n");
	int conn_status = connect(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
	if (conn_status < 0) {
		perror("client connnect error: ");
		return 3;
	}

	printf("WRITING MESSAGE\n");
	ssize_t nbuf = write(sockfd, buffer, sizeof(buffer));
	if (nbuf < 0) {
		perror("client write error: ");
		return 4;
	}

	printf("RECEIVING...\n");
	ssize_t nrecv_buffer = recv(sockfd, recv_buffer, sizeof(recv_buffer), 0);
	if (nrecv_buffer < 0) {
		perror("client receive error: ");
		return 5;
	}

	printf("%s\n", recv_buffer);

	printf("Success!\n");
	close(sockfd);
	freeaddrinfo(serv_info);
	return 0;
}

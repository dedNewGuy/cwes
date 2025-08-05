#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

char buffer[20];

int main(void)
{
	memset(buffer, 'b', sizeof(buffer));
	struct addrinfo hints = {0}, *serv_info = NULL;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int ai_status = getaddrinfo("127.0.0.1",
                       "6969",
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

	int conn_status = connect(sockfd, serv_info->ai_addr, serv_info->ai_addrlen);
	if (conn_status < 0) {
		perror("client connnect error: ");
		return 3;
	}
	write(sockfd, buffer, sizeof(buffer));
	close(sockfd);
	return 0;
}

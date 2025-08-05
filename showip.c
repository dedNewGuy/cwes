/**
* showip.c
*
* simple program to show the ip address of the website
*/

#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>

int main(int argc, char **argv)
{
	int status = 0;
	char ipstr[INET6_ADDRSTRLEN];
	struct addrinfo hints = {0}, *res = NULL, *p = NULL;

	if (argc != 2) {
		fprintf(stderr, "Usage: ipshow <hostname>\n");
		return 1;
	}

	hints.ai_family = AF_UNSPEC;	
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(argv[1],
				 NULL,
				 &hints,
				 &res);
	if (status != 0) {
		const char *err_msg = gai_strerror(status);
		fprintf(stderr, "getaddrinfo: %s\n", err_msg);
		return 2;
	}

	printf("IP Address for %s:\n\n", argv[1]);

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;
		struct sockaddr_in *ipv4;
		struct sockaddr_in6 *ipv6;
		
		if (p->ai_family == AF_INET) {
			ipv4 = (struct sockaddr_in *) p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else {
			ipv6 = (struct sockaddr_in6 *) p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		printf("    %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res);
	return 0;
}

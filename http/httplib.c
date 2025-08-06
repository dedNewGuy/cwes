#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

#include "httplib.h"

enum http_request_method_t get_method_from_str(char *method)
{
	if (strcmp(method, "GET") == 0) {
		return HTTP_GET;
	}

	return HTTP_UNKNOWN_METHOD;
}

struct http_request_t parse_http_request(char *req_msg, int id)
{
	struct http_request_t req = {0};

	char *method_str = strtok(req_msg, " ");
	enum http_request_method_t method = get_method_from_str(method_str);

	char *path = strtok(NULL, " ");

	req.id = id;
	req.method = method;
	req.path = path;

	return req;
}

int write_body(int sockfd, char *path)
{
	char *index_html_filepath = strcat(path, "index.html");
	FILE *fptr = fopen(index_html_filepath, "r");

	printf("reading %s\n", path);

	char buffer[2048] = "HTTP/1.1 200 OK\r\n\
Content-Type: text/html\r\n\
Content-Length: 150\r\n\
Connection: close\r\n\
\r\n";

	char lines[256] = {0};
	while (1) {
		char *fgets_status = fgets(lines, sizeof lines, fptr);
		if (fgets_status == NULL) break;
		printf("%s", lines);
		strcat(buffer, lines);
	}

	fclose(fptr);
	ssize_t nbuf = send(sockfd, buffer, sizeof buffer, MSG_NOSIGNAL);
	if (nbuf < 0) {
		perror("Sending response error");
		return -1;
	}

	return 1;
}

int send_http_response(char *directory, struct http_request_t req)
{
	enum http_request_method_t method =  req.method;
	char *path = req.path;

	switch (method) {
		case HTTP_GET:
			if (strcmp(path, "/") == 0)
				return write_body(req.id, directory);
			else
				return write_body(req.id, path);
		break;
		default:
		break;
	}

	return 1;
}


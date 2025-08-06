#ifndef HTTP_LIB_H
#define HTTP_LIB_H

enum http_request_method_t {
	HTTP_GET,
	HTTP_UNKNOWN_METHOD,
};

struct http_request_t {
	char *path;
	int id;
	enum http_request_method_t method;
};

struct http_request_t parse_http_request(char *req_msg, int id);
int send_http_response(char *dir, struct http_request_t req);

#endif // HTTP_LIB_H

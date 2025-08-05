CC = gcc
CFLAGS = -ggdb -Wall -Wextra

all: simp_client simp_server showip http_client

simp_client: ./simple_netcomm/client.c
	$(CC) $(CFLAGS) -o simp_client ./simple_netcomm/client.c


simp_server: ./simple_netcomm/server.c
	$(CC) $(CFLAGS) -o simp_server ./simple_netcomm/server.c


showip: ./showip_src/showip.c
	$(CC) $(CFLAGS) -o showip ./showip_src/showip.c

http_client: ./http/http_client.c
	$(CC) $(CFLAGS) -o http_client ./http/http_client.c

.PHONY: all

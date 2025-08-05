CC = gcc
CFLAGS = -ggdb -Wall -Wextra

all: simp_client simp_server showip

simp_client: ./simple_netcomm/client.c
	$(CC) $(CFLAGS) -o simp_client ./simple_netcomm/client.c


simp_server: ./simple_netcomm/server.c
	$(CC) $(CFLAGS) -o simp_server ./simple_netcomm/server.c


showip: ./showip_src/showip.c
	$(CC) $(CFLAGS) -o showip ./showip_src/showip.c

.PHONY: all

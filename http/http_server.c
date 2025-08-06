#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: http_server <directory>\n");
		return 2;
	}

	char *dir_name = argv[1];

	return 0;
}

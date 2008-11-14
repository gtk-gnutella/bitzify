/* THIS FILE IS PUBLIC DOMAIN. NO WARRANTY OF ANY KIND. NO RIGHTS RESERVED. */
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, const char *argv[])
{
	static char buf[20];
	size_t i, n;
	FILE *f;

	if (argc > 1) {
		f = fopen(argv[1], "rb");
		if (NULL == f) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
	} else {
		f = stdin;
	}
	n = fread(buf, 1, sizeof buf, f);
	if (0 == n && ferror(f)) {
		perror("fread");
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < n; i++) {
		printf("%02X", (unsigned char) (buf[i] & 0xFF));
	}
	printf("\n");
	return 0;
}

/* THIS FILE IS PUBLIC DOMAIN. NO WARRANTY OF ANY KIND. NO RIGHTS RESERVED. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int
is_safe_char(int c)
{
	return	(c >= 'a' && c <= 'z')
	|| 	(c >= 'A' && c <= 'Z')
	|| 	(c >= '0' && c <= '9')
	|| 	NULL != strchr(".:-_/", c);
}

int
main(int argc, const char *argv[])
{
	const char *p;

	if (argc != 2) {
		exit(EXIT_FAILURE);
	}
	for (p = argv[1]; '\0' != *p; p++) {
		unsigned char c = *p;

		if (is_safe_char(c))
			printf("%c", c);
		else
			printf("%%%02X", c);
	}
	return 0;
}

/* THIS FILE IS PUBLIC DOMAIN. NO WARRANTY OF ANY KIND. NO RIGHTS RESERVED. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utf8.h"

static int
is_safe_char(UTF32_INT_TYPE c)
{
	return	(c <= 127 && '&' != c);
}

static bool
is_utf8_string(const char *p)
{
	size_t size = strlen(p);

	while ('\0' != *p) {
		UTF32_INT_TYPE c;
		size_t clen;

		c = utf8_decode(p, size);
		if ((UTF32_INT_TYPE)-1 == c)
			return false;
		clen = utf8_first_byte_length_hint(*p);
		size -= clen;
		p += clen;
	}
	return true;
}

int
main(int argc, const char *argv[])
{
	const char *p;
	size_t size;

	if (argc != 2) {
		exit(EXIT_FAILURE);
	}
	p = argv[1];
	if (!is_utf8_string(p)) {
		exit(EXIT_FAILURE);
	}
	size = strlen(p);
	while ('\0' != *p) {
		UTF32_INT_TYPE c;
		size_t clen;

		c = utf8_decode(p, size);
		if (is_safe_char(c)) {
			printf("%c", (unsigned char) c);
		} else {
			printf("&#%lu;", (unsigned long) c);
		}
		clen = utf8_first_byte_length_hint(*p);
		size -= clen;
		p += clen;
	}
	return 0;
}

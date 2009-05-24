# THIS FILE IS PUBLIC DOMAIN. NO WARRANTY OF ANY KIND. NO RIGHTS RESERVED.

CC ?= cc
CFLAGS := -W -Wall -Wformat=2 -Wshadow -O2 -g $(CFLAGS)
PREFIX ?= /usr/local

all: audiotags first20 urlencode htmlencode bitzify

audiotags: audiotags.c
	$(CC) $(CFLAGS) `pkg-config taglib --cflags --libs` -ltag_c -o $@ $@.c

first20: first20.c

urlencode: urlencode.c

htmlencode: htmlencode.c

bitzify:

install-anyway: all
	cp -i -- audiotags first20 urlencode htmlencode bitzify "$(PREFIX)/bin/"

install: check install-anyway

distclean: clobber

clobber:
	rm -f -- audiotags first20 urlencode htmlencode

check:
	/bin/sh run_checks.sh

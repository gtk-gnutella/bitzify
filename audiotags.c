/* THIS FILE IS PUBLIC DOMAIN. NO WARRANTY OF ANY KIND. NO RIGHTS RESERVED. */
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tag_c.h>

static char *progname = "";

enum {
	tag_item_artist,
	tag_item_album,
	tag_item_title,
	tag_item_tracknumber,
	tag_item_year,
	tag_item_genre,
	tag_item_comment,
	tag_item_bitrate,
	tag_item_samplerate,
	tag_item_channels,
	tag_item_duration,
	
	tag_item_NUM
};

static int show_items[tag_item_NUM];
static int show_all = 1;

static void
usage(void)
{
	fprintf(stderr,
		"Usage: %s [OPTIONS] FILE\n", progname);
	fprintf(stderr,
		"Options:\n"
		"  --help             Display this message.\n"
		"  --quiet            Don't show any error output.\n"
		"  --show-album       Display the album.\n"
		"  --show-artist      Display the artist.\n"
		"  --show-bitrate     Display the bitrate.\n"
		"  --show-channels    Display the number of channels.\n"
		"  --show-duration    Display the duration.\n"
		"  --show-genre       Display the genre.\n"
		"  --show-samplerate  Display the samplerate.\n"
		"  --show-title       Display the title.\n"
		"  --show-tracknumber Display the tracknumber.\n"
		"  --show-year        Display the year.\n"
	);
	exit(EXIT_FAILURE);
}

#define SELECTED(item) (show_items[tag_item_ ## item])

static int
print_tags(const char *filename)
{
  TagLib_File *file;
  TagLib_Tag *tag;
  const TagLib_AudioProperties *properties;

  if (show_all) {
  	int i;
	for (i = 0; i < tag_item_NUM; i++) {
		show_items[i] = 1;
	}
  }

  file = taglib_file_new(filename);
  if (NULL == file) {
	return -1;
  }

  taglib_set_strings_unicode(1);

  tag = taglib_file_tag(file);
  properties = taglib_file_audioproperties(file);

  if (SELECTED(artist)) {
	printf("artist=%s\n", taglib_tag_artist(tag));
  }
  if (SELECTED(album)) {
	printf("album=%s\n", taglib_tag_album(tag));
  }
  if (SELECTED(title)) {
  	printf("title=%s\n", taglib_tag_title(tag));
  }
  if (SELECTED(tracknumber)) {
	  unsigned value = taglib_tag_track(tag);
	  if (value > 0) {
		  printf("tracknumber=%u\n", value);
	  } else {
		  printf("tracknumber=\n");
	  }
  }
  if (SELECTED(year)) {
	  unsigned value = taglib_tag_year(tag);
	  if (value > 0) {
		  printf("year=%d\n", value);
	  } else {
		  printf("year=\n");
	  }
  }
  if (SELECTED(genre)) {
	printf("genre=%s\n", taglib_tag_genre(tag));
  }
  if (SELECTED(comment)) {
	printf("comment=%s\n", taglib_tag_comment(tag));
  }
  if (SELECTED(bitrate)) {
  	int value = taglib_audioproperties_bitrate(properties);
	if (value > 0) {
		printf("bitrate=%d\n", value);
	} else {
		printf("bitrate=\n");
	}
  }
  if (SELECTED(samplerate)) {
	int value = taglib_audioproperties_samplerate(properties);
	if (value > 0) {
		printf("samplerate=%d\n", value);
	} else {
		printf("samplerate=\n");
	}
  }
  if (SELECTED(channels)) {
	int value = taglib_audioproperties_channels(properties);
  	if (value > 0) {
		printf("channels=%d\n", value);
	} else {
		printf("channels=\n");
	}
  }
  if (SELECTED(duration)) {
	int value = taglib_audioproperties_length(properties);
	if (value >= 0) {
		printf("duration=%u:%02u\n", value / 60, value % 60);
	} else {
		printf("duration=\n");
	}
  }

  taglib_tag_free_strings();
  taglib_file_free(file);
  return 0;
}

static int
is_regular_file(const char *filename)
{
	struct stat sb;

	errno = 0;
	if (stat(filename, &sb))
		return 0;
	else
		return 0 != S_ISREG(sb.st_mode);
}

#define SHIFT do { if (argc > 0) { argc--; argv++; } } while (0)

#define SHOW(item) \
	else if (0 == strcmp(s, "--show-" #item )) { \
		show_all = 0; \
		show_items[( tag_item_ ## item )] = 1;

int
main(int argc, char *argv[])
{
  int help = 0;
  int quiet = 0;
  const char *filename;

  progname = strrchr(*argv, '/');
  progname = progname ? &progname[1] : *argv;
  SHIFT;

  while (argc > 0) {
	const char *s = *argv;

	if (0 == strcmp(s, "--help")) {
		help = 1;
		break;
	} else if (0 == strcmp(s, "--quiet")) {
		quiet = 1;
	} SHOW(artist)
	} SHOW(album)
	} SHOW(title)
	} SHOW(tracknumber)
	} SHOW(year)
	} SHOW(genre)
	} SHOW(comment)
	} SHOW(bitrate)
	} SHOW(samplerate)
	} SHOW(channels)
	} SHOW(duration)
	} else if (0 == strcmp(s, "--")) {
		SHIFT;
		break;
	} else if ('-' == s[0]) {
		fprintf(stderr, "ERROR: Unsupported argument \"%s\"\n", s);
		usage();
	} else {
		break;
	}
	SHIFT;
  }

  if (1 != argc) {
	fprintf(stderr, "ERROR: %s\n",
		argc < 1 ? "Missing argument." : "Too many arguments.");
	usage();
  }
  if (help) {
	usage();
  }

  filename = *argv;
  if (!is_regular_file(filename)) {
	fprintf(stderr, "ERROR: %s: \"%s\"\n",
		errno ? strerror(errno) : "Not a regular file",
		filename);
	exit(EXIT_FAILURE);
  }
  if (quiet) {
	fflush(stderr);
	if (NULL == freopen("/dev/null", "w", stderr)) {
		exit(EXIT_FAILURE);
	}
  }
  if (print_tags(filename)) {
	fprintf(stderr, "ERROR: Cannot handle file: \"%s\"\n", filename);
	exit(EXIT_FAILURE);
  }
  return 0;
}

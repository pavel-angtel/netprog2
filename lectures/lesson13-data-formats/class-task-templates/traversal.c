
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <jansson.h>

static void print_strings(json_t *j)
{
	json_t *jchild;
	const char *key;
	int index;

	printf("string value: %s\n", json_string_value(j));

	json_object_foreach(j, FIXME)

	json_array_foreach(j, FIXME)
}

int main(int argc, char *argv[])
{
	json_t *jconf = NULL;
	json_error_t jerr;

	jconf = json_load_file("configuration.json", 0, &jerr);
	if (!jconf) {
		fprintf(stderr, "Failed to load configuration: %s (line %d)\n",
				jerr.text, jerr.line);
		goto on_error;
	}

	print_strings(jconf);

	json_decref(jconf);
	return 0;
on_error:
	json_decref(jconf);
	return 1;
}


#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <jansson.h>

#define TRAFFIC_UNICAST 0x1
#define TRAFFIC_MULTICAST 0x2
#define TRAFFIC_BROADCAST 0x4

static int parse_json_config(json_t *jconf)
{
	json_error_t jerr;
	int enable;
	const char *address;
	int prefix_length;
	int mtu;
	const char *types[2];
	unsigned int traffic_types_mask = 0;
	int i;
	int ret;

	ret = json_unpack_ex(jconf, &jerr, 0,
			     FIXME);
	if (ret < 0) {
		fprintf(stderr, "Invalid config: %s\n", jerr.text);
		return -1;
	}
	for (i = 0; i < 2; i++) {
		if (!strcmp(types[i], "unicast"))
			traffic_types_mask |= TRAFFIC_UNICAST;
		else if (!strcmp(types[i], "multicast"))
			traffic_types_mask |= TRAFFIC_MULTICAST;
	}


	printf("enable %d, address %s, prefix-length %d, mtu %d, traffic mask %x\n",
	       enable, address, prefix_length, mtu, traffic_types_mask);
	return 0;
}

int main(int argc, char *argv[])
{
	json_t *jconf = NULL;
	json_error_t jerr;
	int ret;


	jconf = json_load_file("configuration.json", 0, &jerr);
	if (!jconf) {
		fprintf(stderr, "Failed to load configuration: %s (line %d)\n",
				jerr.text, jerr.line);
		goto on_error;
	}

	ret = parse_json_config(jconf);
	if (ret < 0) {
		fprintf(stderr, "Failed to parse JSON config\n");
		goto on_error;
	}

	json_decref(jconf);
	return 0;
on_error:
	json_decref(jconf);
	return 1;
}

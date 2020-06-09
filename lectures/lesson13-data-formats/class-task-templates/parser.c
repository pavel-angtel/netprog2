
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <jansson.h>

#define FIXME 0

#define TRAFFIC_UNICAST 0x1
#define TRAFFIC_MULTICAST 0x2
#define TRAFFIC_BROADCAST 0x4

static int parse_json_config(json_t *jconf)
{
	json_t *j, *jparent;
	int enable;
	const char *address;
	int prefix_length;
	int mtu;
	unsigned int traffic_types_mask = 0;
	int i;

	j = FIXME("network");
	assert(j);

	j = FIXME("interfaces");
	assert(j);

	jparent = FIXME("eth1");

	j = FIXME("enable");
	enable = FIXME;

	j = FIXME("address");
	address = FIXME;

	j = FIXME("prefix-length");
	prefix_length = FIXME;

	j = FIXME("mtu");
	mtu = FIXME

	jparent = FIXME("traffic-types");

	for (i = 0; i < json_array_size(jparent); i++) {
		const char *traffic_type;

		j = FIXME(i);
		traffic_type = FIXME;

		FIXME;
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

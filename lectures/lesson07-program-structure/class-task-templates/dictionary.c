
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

#include <libubox/utils.h>
#include <libubox/avl.h>
#include <libubox/avl-cmp.h>

#define FIXME 0

struct device {
	struct avl_node anode;
	char *name;
};

int main(int argc, char *argv[])
{
	struct avl_tree _devices, *devices = &_devices;
	const char *device_names[] = {"123", "456", "acbc", "xyz"};
	struct device *dev, *next_dev;
	int i;
	int ret;

	avl_init(devices, avl_strcmp, false, NULL);

	for (i = 0; i < ARRAY_SIZE(device_names); i++) {
		struct device *new;

		new = calloc(1, sizeof(*new));
		if (!new)
			goto on_error;

		new->name = strdup(device_names[i]);
		if (!new->name) {
			free(new);
			goto on_error;
		}
		new->anode.key = FIXME;

		ret = avl_insert(devices, FIXME);
		if (ret < 0) {
			fprintf(stderr, "Key collision\n");
			free(new->name);
			free(new);
			goto on_error;
		}
	}

	/* Find element with key "456". */
	dev = avl_find_element(devices, FIXME);
	printf("Deleting device '%s' with address: %p\n", dev->name, dev);
	avl_delete(devices, FIXME);

	avl_for_each_element(devices, FIXME)
		printf("Device '%s' address: %p\n", dev->name, dev);

	avl_for_each_element_safe(devices, FIXME) {
		free(dev->name);
		free(dev);
	}

	return 0;
on_error:
	avl_for_each_element_safe(devices, FIXME) {
		free(dev->name);
		free(dev);
	}
	return 1;
}

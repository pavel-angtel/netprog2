
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

#include <libevdev-1.0/libevdev/libevdev.h>

#define FIXME 0

struct key {
	char key_char;
	struct timeval tv;
};

int main(int argc, char *argv[])
{
	int fd = -1;
	struct libevdev *evdev = NULL;
	struct input_event ev;
	struct key key;
	int ret;

	fd = open("/dev/input/event3", O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Failed to open input device file: %s\n", strerror(errno));
		goto on_error;
	}

	ret = libevdev_new_from_fd(fd, &evdev);
	if (ret < 0) {
		fprintf(stderr, "Failed to init evdev: %s\n", strerror(errno));
		goto on_error;
	}

	if (!libevdev_has_event_type(evdev, FIXME) ||
	    !libevdev_has_event_code(evdev, FIXME, FIXME)) {
		fprintf(stderr, "Wrong device\n");
		goto on_error;
	}

	while (1) {
		ret = libevdev_next_event(evdev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		if (ret < 0) {
			fprintf(stderr, "Failed to read event from power button\n");
			goto on_error;
		}
		/* Find a pressing of digit. */
		if (ev.type == FIXME &&
		    ev.code >= FIXME && ev.code <= FIXME &&
		    ev.value == FIXME) {
			key.key_char = FIXME;
			gettimeofday(FIXME, NULL);
			break;
		}
	}

	printf("Entered key '%c' at %ld seconds since Epoch\n", key.key_char, key.tv.tv_sec);
	libevdev_free(evdev);
	close(fd);
	return 0;
on_error:
	if (evdev)
		libevdev_free(evdev);
	if (fd >= 0)
		close(fd);
	return 1;
}

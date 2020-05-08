
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/time.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

#define FIXME 0

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

struct repeater {
	unsigned int index;
	int listen_sock;
	int data_sock;
};

static int init_repeater(struct repeater *r, unsigned int index)
{
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(9000 + r->index),
		.sin_addr = {INADDR_ANY},
	};
	int option = 1;
	int ret;

	r->listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!r->listen_sock) {
		fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
		return -1;
	}

	ret = setsockopt(r->listen_sock, SOL_SOCKET, SO_REUSEADDR,
			 &option, sizeof(option));
	if (ret < 0) {
		fprintf(stderr, "Failed to set socket option: %s\n", strerror(errno));
		goto on_error;
	}

	ret = bind(r->listen_sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		fprintf(stderr, "Failed to bind to server: %s\n", strerror(errno));
		goto on_error;
	}

	ret = listen(r->listen_sock, 1);
	if (ret < 0) {
		fprintf(stderr, "Failed to put socket into listening state: %s\n",
				strerror(errno));
		goto on_error;
	}

	return 0;
on_error:
	close(r->listen_sock);
	r->listen_sock = -1;
	return -1;
}

int main(int argc, char *argv[])
{
	struct repeater repeaters[10];
	int ret;
	int i;

	memset(repeaters, 0, sizeof(repeaters));
	for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
		repeaters[i].index = i;
		repeaters[i].listen_sock = -1;
		repeaters[i].data_sock = -1;
	}
	for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
		ret = init_repeater(&repeaters[i], i);
		if (ret < 0)
			goto on_error;
	}

	while (1) {
		fd_set fds;
		int max_fd = 0;
		struct timeval timeout;

		FD_ZERO(&fds);

		for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
			struct repeater *r = &repeaters[i];
			int sock = r->data_sock < 0 ? FIXME : FIXME;

			FD_SET(FIXME);
			max_fd = FIXME;
		}

		memset(&timeout, 0, sizeof(timeout));
		FIXME = 10;

		ret = select(FIXME);
		if (ret < 0) {
			fprintf(stderr, "select() failed: %s\n", strerror(errno));
			goto on_error;
		}

		if (FIXME) {
			printf("Timeout.\n");
			for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
				struct repeater *r = &repeaters[i];

				if (r->data_sock >= 0) {
					close(r->data_sock);
					r->data_sock = -1;
				}
			}
			continue;
		}

		for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
			struct repeater *r = &repeaters[i];

			if (FIXME) {
				r->data_sock = accept(r->listen_sock, NULL, NULL);
				if (r->data_sock < 0) {
					fprintf(stderr, "Failed to accept data "
							"connection: %s\n",
							strerror(errno));
					goto on_error;
				}
			} else if (FIXME && FIXME) {
				char  ch;

				ret = recv(r->data_sock, &ch, 1, MSG_DONTWAIT);
				if (ret < 0)
					goto on_error;
				if (ret == 0) {
					FIXME;
				}
				send(r->data_sock, &ch, 1, MSG_DONTWAIT);
			}
		}
	}

	for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
		if (repeaters[i].listen_sock >= 0)
			close(repeaters[i].listen_sock);
		if (repeaters[i].data_sock >= 0)
			close(repeaters[i].data_sock);
	}
	return 0;
on_error:
	for (i = 0; i < ARRAY_SIZE(repeaters); i++) {
		if (repeaters[i].listen_sock >= 0)
			close(repeaters[i].listen_sock);
		if (repeaters[i].data_sock >= 0)
			close(repeaters[i].data_sock);
	}
	return 1;
}

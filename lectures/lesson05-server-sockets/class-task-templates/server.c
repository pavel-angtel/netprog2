
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
#define MESSAGE_SIZE 1024

int main(int argc, char *argv[])
{
	int listen_sock = -1, data_sock = -1;
	short port = 10000;
	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = {INADDR_ANY},
	};
	int ret;

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0) {
		fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
		return -1;
	}

	{
		int option = 1;

		ret = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR,
				 &option, sizeof(option));
		if (ret < 0) {
			fprintf(stderr, "Failed to set socket option: %s\n",
					strerror(errno));
			goto on_error;
		}
	}

	ret = bind(FIXME);
	if (ret < 0) {
		fprintf(stderr, "Failed to bind to server: %s\n", strerror(errno));
		goto on_error;
	}

	ret = listen(FIXME);
	if (ret < 0) {
		fprintf(stderr, "Failed to put socket into listening state: %s\n",
				strerror(errno));
		goto on_error;
	}

	while (1) {
		int msg_len;

		data_sock = accept(FIXME);
		if (data_sock < 0) {
			fprintf(stderr, "Failed to accept data connection: %s\n",
					strerror(errno));
			goto on_error;
		}

		while (1) {
			char request[MESSAGE_SIZE];

			msg_len = recv(data_sock, request, sizeof(request) - 1, 0);
			if (FIXME) {
				fprintf(stderr, "Failed to read HTTP request: %s\n",
						strerror(errno));
				goto on_error;
			} else if (FIXME) {
				printf("Client disconnected\n");
				close(data_sock);
				break;
			}

			FIXME;
			printf("Received message: %s", request);
		}
	}

	return 0;
on_error:
	if (listen_sock >= 0)
		close(listen_sock);
	if (data_sock >= 0)
		close(data_sock);
	return 1;
}


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

#include <picohttpparser.h>

#define FIXME 0

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define MESSAGE_SIZE 1024
#define METHOD_SIZE 16
#define PATH_SIZE 32
#define BODY_SIZE 64

int main(int argc, char *argv[])
{
	char buf[MESSAGE_SIZE];
	size_t len;
	char method[METHOD_SIZE];
	char path[PATH_SIZE];
	char body[BODY_SIZE];
	FILE *fp;
	const char *method_ptr, *path_ptr;
	struct phr_header headers[100];
	size_t method_len, path_len, num_headers;
	int minor_version;
	int headers_len;

	fp = fopen("http-request", "r");
	if (!fp) {
		fprintf(stderr, "Failed to open HTTP request: %s\n", strerror(errno));
		return 1;
	}
	len = fread(buf, 1,  MESSAGE_SIZE, fp);
	if (ferror(fp)) {
		fprintf(stderr, "Failed to read HTTP request\n");
		fclose(fp);
		return 1;
	}
	fclose(fp);

	num_headers = ARRAY_SIZE(headers);
	headers_len = phr_parse_request(FIXME);

	if (headers_len <= 0) {
		fprintf(stderr, "Failed to parse HTTP request\n");
		return -1;
	}

	FIXME;

	printf("Received HTTP request: method %s, path %s, body %s\n",
	       method, path, body);

	return 0;
}

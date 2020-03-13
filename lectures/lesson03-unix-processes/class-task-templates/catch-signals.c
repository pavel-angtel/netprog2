
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/signal.h>
#include <errno.h>
#include <assert.h>

static bool sigterm;
static bool sigint;
static bool sigchld;
static bool sigsegv;
static bool sigkill;

static void signal_handler(int sig)
{
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	int ret;

	/* Регистрация обработчика для SIGTERM, SIGINT, SIGKILL, SIGCHLD. */

	/* ... */

	while (1) {
		char buf[100];

		ret = read(STDIN_FILENO, buf, sizeof(buf));

		/* ... */
	}

	return 0;
}

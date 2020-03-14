
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/signal.h>
#include <errno.h>
#include <assert.h>

#define FIXME 0

static bool sigterm;
static bool sigint;
static bool sigchld;
static bool sigkill;

static void signal_handler(int sig)
{
	/* ... */
}

int main(int argc, char *argv[])
{
	struct sigaction sa;
	int ret;

	/* Регистрация обработчика для SIGTERM, SIGINT, SIGKILL, SIGCHLD. */

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = FIXME;
	sigaction(FIXME, &sa, NULL);

	/* ... */

	while (1) {
		char buf[100];

		ret = read(STDIN_FILENO, buf, sizeof(buf));
		if (FIXME) {
			printf("Interrupted by signal: "
			       "term %d, int %d, chld %d, kill %d\n",
			       sigterm, sigint, sigchld, sigkill);
		}
	}

	return 0;
}

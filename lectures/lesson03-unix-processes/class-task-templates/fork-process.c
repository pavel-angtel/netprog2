
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>

#define FIXME 0

int main(int argc, char *argv[])
{
	pid_t pid;

	printf("Parent PID: %d\n", getpid());

	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Failed to fork: %s\n", strerror(errno));
		return -1;
	}
	
	if (FIXME) {
		/* Дочерний процесс */

		exit(FIXME);
	} else if (FIXME) {
		/* Родительский процесс */
		int child_status;
		pit_t terminated_pid;

		terminated_pid = waitpid(FIXME, &child_status, FIXME);

		/* Для работы с переменной child_status используются макросы
		 * WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG.
		 */
		if (FIXME)
			printf("Child %d exited with status %d\n",
			       FIXME, FIXME);
		else if (FIXME)
			printf("Child %d was terminated by signal %d\n",
			       FIXME, FIXME);
	}
	return 0;
}

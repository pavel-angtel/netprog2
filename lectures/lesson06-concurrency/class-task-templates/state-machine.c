
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/time.h>
#include <errno.h>
#include <assert.h>

#define FIXME 0

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

enum state {
	STOP,
	ECHO,
	PAUSE,
};

struct repeater {
	enum state state;

	/* Данные для состояния PAUSE. */
	struct timeval pause_expiration;
};

static const char *state2str(enum state state)
{
	const char *strings[] = {
		[STOP] = "stop",
		[ECHO] = "echo",
		[PAUSE] = "pause",
	};
	return strings[state];
}

static const char *event2str(int event)
{
	static char str[2];

	if (!isalnum(event))
		return NULL;
	sprintf(str, "%c", event);
	return str;
}

static void run_state_machine(struct repeater *r, int event)
{
	enum state old_state = r->state;
	const char *event_str = event2str(event);
	struct timeval now;
	int ret;

	if (!event_str)
		return;

	printf("Received event '%s' in state %s\n", 
	       event_str, state2str(r->state));

	gettimeofday(&now, NULL);

	switch (r->state) {
	case STOP:
		if (event == 'e')
			r->state = FIXME;
		break;
	case ECHO:
		if (isdigit(event)) {
			ret = printf("Echoing digit '%c'\n", event);
			if (ret < 0)
				goto on_error;
		} else if (event == 's') {
			r->state = FIXME;
		} else if (event == 'p') {
			struct timeval pause = {.tv_sec = 3};

			timeradd(FIXME);
			r->state = FIXME;
		}
		break;
	case PAUSE:
		if (isdigit(event)) {
			struct timeval left;

			if (timercmp(FIXME)) {
				r->state = FIXME;
			} else {
				timersub(FIXME);

				ret = printf("%ld seconds left\n", left.tv_sec);
				if (ret < 0)
					goto on_error;
			}
		}
		break;
	}
	if (r->state != old_state)
		printf("Transitioning to state %s\n", state2str(r->state));
	return;
on_error:
	return;
}

int main(int argc, char *argv[])
{
	struct repeater _repeater, *r = &_repeater;
	int ret;

	memset(r, 0, sizeof(*r));
	r->state = STOP;

	while (1) {
		char ch;

		ret = fread(&ch, 1, 1, stdin);
		if (ferror(stdin))
			goto on_error;
		if (ret != 1)
			break;
		if (!isalnum(ch))
			continue;
		run_state_machine(r, ch);
	}

	return 0;
on_error:
	return 1;
}

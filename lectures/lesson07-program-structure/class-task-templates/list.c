
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
#include <libubox/list.h>

#define FIXME 0

struct event {
	struct list_head lnode;
	char *name;
};

int main(int argc, char *argv[])
{
	struct list_head _events, *events = &_events;
	const char *event_names[] = {"123", "456", "acbc", "xyz"};
	struct event *ev, *next_ev;
	int i;

	INIT_LIST_HEAD(events);

	for (i = 0; i < ARRAY_SIZE(event_names); i++) {
		struct event *new;

		new = calloc(1, sizeof(*new));
		if (!new)
			goto on_error;

		new->name = strdup(event_names[i]);
		if (!new->name) {
			free(new);
			goto on_error;
		}

		list_add_tail(FIXME, events);
	}

	ev = list_first_entry(events, FIXME);
	printf("Deleting first event with name: %s\n", ev->name);
	list_del(&ev->lnode);

	list_for_each_entry(FIXME, events, FIXME)
		printf("Event '%s'\n", ev->name);

	list_for_each_entry_safe(FIXME, events, FIXME) {
		free(ev->name);
		free(ev);
	}

	return 0;
on_error:
	list_for_each_entry_safe(FIXME, events, FIXME) {
		free(ev->name);
		free(ev);
	}
	return 1;
}

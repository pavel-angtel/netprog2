
CC ?= $(CROSS_COMPILE)gcc
CFLAGS += -O2 -Wall -g
LIBS = -levdev -lubox

APPS = keyboard dictionary list

all: $(APPS)

$(APPS): %: %.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(APPS) $(TEMPLATES)

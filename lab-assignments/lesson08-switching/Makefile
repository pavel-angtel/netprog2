
CC ?= $(CROSS_COMPILE)gcc
CFLAGS += -O2 -Wall -g
LIBS = -lubox

APPS = lab_08

all: $(APPS)

$(APPS): lab_08.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(APPS)

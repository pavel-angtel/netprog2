
CC ?= $(CROSS_COMPILE)gcc
CFLAGS += -O2 -Wall -g
LIBS = -lpicohttpparser

APPS = lab_05

all: $(APPS)

$(APPS): lab_05.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(APPS)

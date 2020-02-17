
CC ?= $(CROSS_COMPILE)gcc
CFLAGS += -O2 -Wall -g
LIBS = -lubox -levdev

APPS = lab_07

all: $(APPS)

$(APPS): lab_07.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(APPS)


CC ?= $(CROSS_COMPILE)gcc
CFLAGS += -O2 -Wall -g
LIBS = -lubox

APPS = mac-table af-packet

all: $(APPS)

$(APPS): %: %.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(APPS) $(TEMPLATES)

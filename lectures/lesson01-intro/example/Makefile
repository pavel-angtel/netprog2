CC = mipsel-oe-linux-musl-gcc -mel -mabi=32 -msoft-float -march=mips32 -mmusl \
     --sysroot=/usr/angtel-sdk/sysroots/mips32el-nf-oe-linux-musl
CFLAGS = -O2 -pipe -g -feliminate-unused-debug-types
LDFLAGS = -Wl,-O1 -Wl,--as-needed
LIBS =

SOURCES = hello_world.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = hello_world

BOARD_ADDRESS = 192.168.0.8

.PHONY: all install clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LIBS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

install: all
	curl --user root: --upload-file $(EXECUTABLE) ftp://$(BOARD_ADDRESS)

clean:
	rm -rf *.o $(EXECUTABLE)

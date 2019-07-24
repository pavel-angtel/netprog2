BOARD_ADDRESS ?= 192.168.0.8

BUILD_DIR ?= $(PWD)
DEPS_DIR ?= $(PWD)
INCLUDE_DIR ?= $(PWD)
SRC_DIR ?= $(PWD)

# Logical root directory for headers and libraries.
SDKTARGETSYSROOT ::= /usr/angtel-sdk/sysroots/mips32el-nf-oe-linux-musl

# Program for compiling C programs.
CC ::= mipsel-oe-linux-musl-gcc -mel -mabi=32 -msoft-float \
       -march=mips32 -mmusl --sysroot=$(SDKTARGETSYSROOT)
# Extra flags to give to the C preprocessor.
CPPFLAGS = -MT $@ -MMD -MP -MF $(DEPS_DIR)/$*.d -I$(INCLUDE_DIR)
# Extra flags to give to the C compiler.
CFLAGS ::= -O2 -pipe -g -feliminate-unused-debug-types
# Extra flags to give to the linker.
LDFLAGS ::= -Wl,-O1 -Wl,--as-needed
# Library flags or names given to the linker.
LDLIBS ::= -lubox -ljansson

$(shell mkdir -p $(BUILD_DIR) $(DEPS_DIR))

SRC ::= $(wildcard $(SRC_DIR)/*.c)
OBJ ::= $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
APP ::= $(BUILD_DIR)/$(APP_NAME)

.PHONY: all clean install uninstall help

all: $(APP)

$(APP): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS_DIR)/%.d
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(DEPS_DIR)/%.d: ;

-include $(SRC:$(SRC_DIR)/%.c=$(DEPS_DIR)/%.d)

clean:
	$(RM) $(APP) $(BUILD_DIR)/*.o $(DEPS_DIR)/*.d
	rmdir --ignore-fail-on-non-empty $(BUILD_DIR) $(DEPS_DIR)

install: all
	curl --user root: --upload-file $(APP) ftp://$(BOARD_ADDRESS)

uninstall:
	curl --user root: --quote "DELE $(notdir $(APP))" ftp://$(BOARD_ADDRESS)

help:
	@echo available targets: all install uninstall clean

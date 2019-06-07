APP_NAME ::= app-hello-world-example

MAIN_MAKEFILE ::= ../common.mk

ifneq ($(KERNELRELEASE),)
MAIN_MAKEFILE ::= $(M)/$(MAIN_MAKEFILE)
endif

include $(MAIN_MAKEFILE)

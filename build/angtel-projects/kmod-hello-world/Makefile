DRV_NAME ::= hello_world

MAIN_MAKEFILE ::= ../kmod-common.mk

ifneq ($(KERNELRELEASE),)
MAIN_MAKEFILE ::= $(M)/$(MAIN_MAKEFILE)
endif

include $(MAIN_MAKEFILE)

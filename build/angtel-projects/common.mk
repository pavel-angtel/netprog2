APP_MAKEFILE ::= ../app-common-mips.mk
MOD_MAKEFILE ::= ../mod-common-mips.mk

ifeq ($(or $(APP_NAME),$(MOD_NAME)),)
$(error One of APP_NAME or MOD_NAME must be defined.)
endif

ifneq ($(and $(APP_NAME),$(MOD_NAME)),)
$(error Only one of APP_NAME or MOD_NAME must be defined.)
endif

ifdef APP_NAME
include $(APP_MAKEFILE)
endif

ifdef MOD_NAME

ifneq ($(KERNELRELEASE),)
MOD_MAKEFILE ::= $(M)/$(MOD_MAKEFILE)
endif

include $(MOD_MAKEFILE)

endif

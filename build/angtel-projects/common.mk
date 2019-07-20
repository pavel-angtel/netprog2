APP_MAKEFILE_NAME ::= app-common-mips.mk
MOD_MAKEFILE_NAME ::= mod-common-mips.mk

APP_MAKEFILE ::= $(RELATIVE_PATH_PREFIX)/$(APP_MAKEFILE_NAME)
MOD_MAKEFILE ::= $(RELATIVE_PATH_PREFIX)/$(MOD_MAKEFILE_NAME)

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

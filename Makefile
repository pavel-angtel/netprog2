
SUBDIRS := $(sort $(wildcard ./lectures/lesson*))
export SRCDIR := $(shell pwd)

.PHONY : all clean
all clean : $(SUBDIRS)
	$(foreach d, $(SUBDIRS), make -C $(d) $@;)

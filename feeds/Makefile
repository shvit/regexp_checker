SUBDIRS := $(wildcard */.)

all clean check: $(SUBDIRS) FORCE

$(SUBDIRS): FORCE
	$(MAKE) -C $@ $(MAKECMDGOALS)

FORCE:

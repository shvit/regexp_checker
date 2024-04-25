SUBDIRS=feeds

GLOBAL_LOG=$(shell date +"%Y-%m-%d_%Hh%Mm%Ss").log
export GLOBAL_LOG

all check: $(SUBDIRS) FORCE

clean: $(SUBDIRS) FORCE
	rm -rf results

$(SUBDIRS): FORCE
	$(MAKE) -C $@ $(MAKECMDGOALS) GLOBAL_LOG=$(GLOBAL_LOG)

clean_full: clean
	rm -rf results

FORCE:

.PHONY: all check clean clean_full

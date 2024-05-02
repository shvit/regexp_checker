SUBDIRS=feeds

GLOBAL_LOG:=$(shell date +"%Y-%m-%d_%Hh%Mm%Ss").log
export GLOBAL_LOG

all: $(SUBDIRS) FORCE

clean: $(SUBDIRS) FORCE
	rm -rf results

check: $(SUBDIRS) FORCE
	@echo "Full results:"
	@cat results/$(GLOBAL_LOG)
	@echo "Formatted results:"
	@src/show_results.sh results/$(GLOBAL_LOG)

$(SUBDIRS): FORCE
	$(MAKE) -C $@ $(MAKECMDGOALS) GLOBAL_LOG=$(GLOBAL_LOG)

clean_full: clean
	rm -rf results

FORCE:

.PHONY: all check clean clean_full

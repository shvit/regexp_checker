DIR_REPO=repo
DIR_BIN=bin

CHEKER=checker
APP=checker_$(NAME)

CFLAGS+=-Wall -fPIC -std=c++17 -MMD -g -O0
LDFLAGS+=-lstdc++

GLOBAL_LOG=$(shell date +"%Y-%m-%d_%Hh%Mm%Ss")-single.log
export GLOBAL_LOG

CWD=$(shell pwd)

all: $(APP)

get:
	@if [ -n "$(REPO)" ]; then \
		if [ -d "$(DIR_REPO)" ]; then \
			echo "Check repository '$(NAME)' ..."; \
			cd $(DIR_REPO) && git reset --hard && git clean -fd; \
		else \
			echo "Collect repository '$(NAME)' ..."; \
			git clone -q $(REPO) $(DIR_REPO); \
		fi \
	fi

$(APP): get
	@if [ -n "$(REPO)" ]; then \
		echo "Build external repository '$(NAME)' ..."; \
		if [ -n "$(REPO_BUILD)" ]; then \
			echo -n; $(REPO_BUILD) \
		fi \
	fi
	@echo "Build checker for '$(NAME)' ..."
	@cp ../../src/checker.cpp src/checker.cpp
	$(CXX) src/$(CHEKER).cpp ../../src/base.cpp $(REPO_LINKING) $(CFLAGS) -o $@ $(LDFLAGS)

check: $(APP)
	@echo "Run checker '$(NAME)' ..."
	@mkdir -p ../../results
	@cd ../.. && mkdir -p results && $(CWD)/$(APP) | tee -a results/$$GLOBAL_LOG

clean:
	rm -f src/checker.cpp # remove temporary file
	rm -f *.d
	rm -f $(APP)
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_REPO)

.PHONY: all get check clean
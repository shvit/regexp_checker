NAME=ximtech
REPO=https://github.com/$(NAME)/Regex

DIR_REPO=repo
DIR_BIN=bin

BASE=checker
APP=checker_$(NAME)

CFLAGS+=-Wall -fPIC -std=c++17 -MMD -g -O0
LDFLAGS+=-lstdc++

GLOBAL_LOG=$(shell date +"%Y-%m-%d_%Hh%Mm%Ss")-single.log
export GLOBAL_LOG

all: $(APP)

get:
	@if [ -d "$(DIR_REPO)" ]; then \
		echo "Check repository ..."; \
		cd $(DIR_REPO) && git reset --hard && git clean -fd; \
	else \
		echo "Collect repository ..."; \
		git clone -q $(REPO) $(DIR_REPO); \
	fi

$(APP): get
	@echo "Build ..."
	mkdir -p $(DIR_BIN) && cd $(DIR_BIN) && cmake ../$(DIR_REPO) && make
	$(CXX) src/$(BASE).cpp $(CFLAGS) -o $@ $(LDFLAGS) ../../bin/base.o bin/Regex.a

check: $(APP)
	@mkdir -p ../../results
	@./$(APP) | tee -a ../../results/$$GLOBAL_LOG

clean:
	rm -f *.d
	rm -f $(APP)
	rm -rf $(DIR_BIN)
	rm -rf $(DIR_REPO)

.PHONY: all get check clean

APP:=checker

DIR_SRC=src
DIR_BIN=bin

CFLAGS+=-Wall -fPIC -std=c++17 -MMD -g -O0
LDFLAGS+=-lstdc++

OBJ=$(patsubst $(DIR_SRC)/%.cpp,$(DIR_BIN)/%.o,$(wildcard $(DIR_SRC)/*.cpp))
DEP=$(OBJ:.o=.d)

APP_STDLIB=$(DIR_BIN)/$(APP)_stdlib

GLOBAL_LOG=$(shell date +"%Y-%m-%d_%Hh%Mm%Ss").log
export GLOBAL_LOG

all: $(APP_STDLIB)
	$(MAKE) -C feeds all

-include $(DEP)

dir_bin:
	@mkdir -p $(DIR_BIN)

clean:
	@rm -f $(APP_STDLIB)
	@rm -rf $(DIR_BIN)
	$(MAKE) -C feeds clean

$(DIR_BIN)/%.o: $(DIR_SRC)/%.cpp | dir_bin
	@echo "Compiling $@"
	$(CXX) -c $(CFLAGS) $< -o $@

$(APP_STDLIB): $(OBJ)
	@echo "Linking $@"
	$(CXX) $^ -o $@ $(LDFLAGS)

show:
	@echo "OBJ=$(OBJ)"
	@echo "DEP=$(DEP)"

check: $(APP_STDLIB)
	@mkdir -p results
	@$(APP_STDLIB) | tee -a results/$$GLOBAL_LOG
	$(MAKE) -C feeds $(MAKECMDGOALS) GLOBAL_LOG=$(GLOBAL_LOG)

clean_full: clean
	rm -rf results

.PHONY: all dir_bin clean chow check clean_full

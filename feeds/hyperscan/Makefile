NAME=hyperscan
REPO=https://github.com/intel/$(NAME)
REPO_BUILD=mkdir -p $(DIR_BIN) && cd $(DIR_BIN) && cmake ../$(DIR_REPO) && make;
REPO_LINKING=-I$(DIR_REPO)/src -I$(DIR_BIN) $(DIR_BIN)/lib/libhs.a

include ../build.mk

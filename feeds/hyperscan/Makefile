NAME=hyperscan
REPO=https://github.com/intel/$(NAME)
REPO_BUILD=$(BUILD_CMAKE)
REPO_LINKING=-I$(DIR_REPO)/src -I$(DIR_REPO)/$(DIR_BIN) $(DIR_REPO)/$(DIR_BIN)/lib/libhs.a

include ../build.mk

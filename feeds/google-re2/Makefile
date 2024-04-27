NAME=google-re2
REPO=https://github.com/google/re2
REPO_BUILD=$(BUILD_CMAKE)
DEPENDS_LIBS=absl_base absl_core_headers absl_fixed_array absl_flags absl_flat_hash_map absl_flat_hash_set absl_inlined_vector absl_optional absl_span absl_str_format absl_strings absl_synchronization
REPO_LINKING=-I$(DIR_REPO) $(DIR_REPO)/$(DIR_BIN)/libre2.a $(shell pkg-config $(DEPENDS_LIBS) --libs)

include ../build.mk

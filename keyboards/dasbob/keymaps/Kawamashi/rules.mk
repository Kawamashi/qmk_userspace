USERS_DIR := $(shell \
    dir="$(dir $(realpath $(lastword $(MAKEFILE_LIST))))"; \
    while [ "$$dir" != "/" ]; do \
        if [ -d "$$dir/users" ]; then \
            echo "$$dir/users"; \
            exit 0; \
        fi; \
        dir=$$(dirname "$$dir"); \
    done \
)
include $(USERS_DIR)/Kawamashi/rules.mk
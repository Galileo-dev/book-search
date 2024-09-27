# tool macros
CXX := g++
CXXFLAGS :=
DBGFLAGS := -g
CCOBJFLAGS := $(CXXFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

project_name := makefile-template

# compile macros
TARGET_NAME := main
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif

# define examples
EXAMPLES := stack tree linkedlist
TARGETS := $(addprefix $(BIN_PATH)/, $(EXAMPLES))
TARGETS_DEBUG := $(addprefix $(DBG_PATH)/, $(EXAMPLES))

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(foreach example, $(EXAMPLES), $(OBJ_PATH)/$(example).o)
OBJ_DEBUG := $(foreach example, $(EXAMPLES), $(DBG_PATH)/$(example).o)

# clean files list
DISTCLEAN_LIST := $(OBJ) $(OBJ_DEBUG)
CLEAN_LIST := $(TARGETS) $(TARGETS_DEBUG) $(DISTCLEAN_LIST)

# default rule
default: makedir all

builder-build :
	docker build -f builder.Dockerfile -t $(project_name)-builder:latest .

builder-run :
	docker run \
		--rm \
		-it \
		--platform linux/amd64 \
		--workdir /builder/mnt \
		-v ${PWD}:/builder/mnt \
		$(project_name)-builder:latest \
		/bin/bash

# non-phony targets
$(BIN_PATH)/%: $(OBJ_PATH)/%.o
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(CCOBJFLAGS) -o $@ $<

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(CCOBJFLAGS) $(DBGFLAGS) -o $@ $<

$(DBG_PATH)/%: $(DBG_PATH)/%.o
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) -o $@ $<

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGETS)

.PHONY: debug
debug: $(TARGETS_DEBUG)

.PHONY: run
run: all
	@echo "Running all examples"
	@for target in $(TARGETS); do \
		echo "Running $$target"; \
		$$target; \
	done

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)

# Compiler options
CC := gcc
CFLAGS := -w -pthread

# Directories
SRC_DIR := .
OBJ_DIR := obj
BOUNDED_QUEUE_OBJ_DIR := $(OBJ_DIR)/BoundedQueue

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
SRCS += $(wildcard $(SRC_DIR)/BoundedQueue/*.c)
SRCS += $(wildcard $(SRC_DIR)/CoEditor/*.c)
SRCS += $(wildcard $(SRC_DIR)/ControlAndData/*.c)
SRCS += $(wildcard $(SRC_DIR)/Producer/*.c)
SRCS += $(wildcard $(SRC_DIR)/UnboundedQueue/*.c)
SRCS += $(wildcard $(SRC_DIR)/Dispatcher/*.c)
SRCS += $(wildcard $(SRC_DIR)/ScreenManager/*.c)

OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: ex3.out

# Rule to compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule to link the executable
ex3.out: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@
	@rm -rf $(OBJ_DIR)

# Target to run the executable with conf.txt as argument
run: ex3.out
	@./ex3.out conf.txt

# Cleanup
clean:
	@rm -f ex3.out
	@rm -rf $(OBJ_DIR)

.PHONY: all run clean


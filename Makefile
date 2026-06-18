CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# MCU flags
MCU_FLAGS = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -nostdlib -nostartfiles -ffreestanding

ROOT = .
BUILD_DIR = build

# Common include paths (add more if needed)
INC = -Idrivers/gpio/include -Idrivers/i2c/include -Idrivers/uart/include -Iplatform/include -Ibsp/include -Ihw -Istartup -I.

# Find all .c sources (exclude build outputs)
SRCS := $(shell find . -type f -name '*.c' -not -path './$(BUILD_DIR)/*' -not -path './examples/*/build/*' -not -path './tests/*' | sed 's|^./||')
OBJS := $(patsubst %.c,$(BUILD_DIR)/objs/%.o,$(SRCS))

.PHONY: all clean list

all: $(OBJS)
	@echo "Compiled $(words $(OBJS)) objects (see build/ for .o files)"

$(BUILD_DIR)/objs/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(MCU_FLAGS) $(INC) -c $< -o $@

list:
	@echo "Sources to compile:"
	@printf '%s\n' $(SRCS)

clean:
	@rm -rf $(BUILD_DIR)

# Host test toolchain (for running unit tests on host)
HOST_CC     = gcc
HOST_CFLAGS = -I$(ROOT)/drivers/gpio/include -I$(ROOT)/platform/include -I$(ROOT)/hw -I$(ROOT)/tests -Wall -Wextra -g
HOST_LDFLAGS +=  -lcmocka

# Ensure driver is compiled for host tests with the MMIO stub: define UNIT_TEST_HOST and pre-include mmio_stub.h
HOST_CFLAGS += -DUNIT_TEST_HOST -include $(ROOT)/tests/mmio_stub.h -I/usr/include

TEST_BIN    = $(BUILD_DIR)/test_gpio

.PHONY: test run-tests

test: $(TEST_BIN)
	@echo "Running host unit tests..."
	@$(TEST_BIN)

$(TEST_BIN): $(ROOT)/tests/test_gpio.c $(ROOT)/tests/mmio_stub.h $(ROOT)/drivers/gpio/src/gpio.c $(ROOT)/tests/mmio_stub.c | $(BUILD_DIR)
	@echo "Compiling host tests..."
	@$(HOST_CC) $(HOST_CFLAGS) $(ROOT)/tests/test_gpio.c $(ROOT)/drivers/gpio/src/gpio.c $(ROOT)/tests/mmio_stub.c -o $(TEST_BIN) $(HOST_LDFLAGS)

run-tests: test
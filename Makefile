CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# MCU flags
MCU_FLAGS = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -nostdlib -nostartfiles -ffreestanding

ROOT = .
BUILD_DIR = build

# Common include paths
INC = -Idrivers/gpio/include -Idrivers/i2c/include -Idrivers/uart/include \
      -Iplatform/include -Ibsp/include -Ihw -Istartup -I.

# Find sources (firmware only)
SRCS := $(shell find . -type f -name '*.c' \
        -not -path './$(BUILD_DIR)/*' \
        -not -path './examples/*/build/*' \
        -not -path './tests/*' | sed 's|^./||')

OBJS := $(patsubst %.c,$(BUILD_DIR)/objs/%.o,$(SRCS))

.PHONY: all clean list test run-tests coverage

all: $(OBJS)
	@echo "Compiled $$(words $(OBJS)) objects"

$(BUILD_DIR)/objs/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "CC $<"
	$(CC) $(MCU_FLAGS) $(INC) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

list:
	@printf '%s\n' $(SRCS)

clean:
	@rm -rf $(BUILD_DIR) coverage.info coverage.filtered.info coverage-report *.gcda *.gcno

HOST_CC     = gcc
HOST_CFLAGS = -I$(ROOT)/drivers/gpio/include \
              -I$(ROOT)/platform/include \
              -I$(ROOT)/hw \
              -I$(ROOT)/tests \
              -Wall -Wextra -g \
              -DUNIT_TEST_HOST \
              -include $(ROOT)/tests/mmio_stub.h

HOST_LDFLAGS = -lcmocka

# coverage flags (ONLY for host tests)
HOST_COVERAGE_FLAGS = --coverage

TEST_BIN = $(BUILD_DIR)/test_gpio

test: $(TEST_BIN)
	@echo "Running host unit tests..."
	@$(TEST_BIN)

$(TEST_BIN): $(ROOT)/tests/test_gpio.c \
             $(ROOT)/drivers/gpio/src/gpio.c \
             $(ROOT)/tests/mmio_stub.c | $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling host tests..."
	$(HOST_CC) $(HOST_CFLAGS) $(HOST_COVERAGE_FLAGS) \
		$^ -o $@ $(HOST_LDFLAGS) $(HOST_COVERAGE_FLAGS)

run-tests: test

coverage: clean $(TEST_BIN)
	@echo "Running tests for coverage..."
	@$(TEST_BIN)

	@echo "Generating coverage report..."
	lcov --capture --directory $(BUILD_DIR) --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.filtered.info --ignore-errors unused
	genhtml coverage.filtered.info --output-directory coverage-report

	@echo "Coverage report generated at coverage-report/index.html"
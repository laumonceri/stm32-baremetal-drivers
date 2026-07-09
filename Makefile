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

# Every example folder that has its own Makefile (each links a real main.elf)
EXAMPLE_DIRS := $(sort $(dir $(wildcard examples/*/Makefile)))

.PHONY: all clean list test run-tests coverage examples

# $(OBJS) is a fast per-file syntax check across the whole tree, including
# files no example currently links (e.g. i2c). examples actually links each
# example's main.elf, which is the only thing that catches missing/renamed
# symbols across files, not just per-file syntax errors.
all: $(OBJS) examples

$(BUILD_DIR)/objs/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(MCU_FLAGS) $(INC) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Rebuilds every example from clean and reports all failures, not just the
# first one, so one broken example doesn't hide breakage in the others. The
# clean step is allowed to fail (e.g. a stale root-owned build/ directory)
# since a dirty leftover artifact must never mask a real build failure.
examples:
	@status=0; \
	for dir in $(EXAMPLE_DIRS); do \
		echo "==> $$dir"; \
		$(MAKE) -C $$dir clean >/dev/null 2>&1 || true; \
		$(MAKE) -C $$dir || status=1; \
	done; \
	if [ $$status -eq 0 ]; then \
		echo "All examples built successfully."; \
	else \
		echo "One or more examples FAILED to build."; \
	fi; \
	exit $$status

list:
	@printf '%s\n' $(SRCS)

clean:
	@rm -rf $(BUILD_DIR) coverage.info coverage.filtered.info coverage-report *.gcda *.gcno
	@for dir in $(EXAMPLE_DIRS); do $(MAKE) -C $$dir clean >/dev/null 2>&1 || true; done

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
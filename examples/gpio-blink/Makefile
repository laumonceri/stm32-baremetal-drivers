# Toolchain
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# MCU flags
MCU_FLAGS = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -nostdlib -nostartfiles -ffreestanding

# Paths
ROOT          = ../..
DRIVERS       = $(ROOT)/drivers
PLATFORM      = $(ROOT)/platform
HW            = $(ROOT)/hw
BSP           = $(ROOT)/bsp
STARTUP       = $(ROOT)/startup
BUILD_DIR     = build

# Source files
SRC = main.c \
      $(DRIVERS)/gpio/src/gpio.c \
      $(PLATFORM)/src/rcc.c \
      $(STARTUP)/startup_stm32l4xx.c

# Include paths
INC = -I$(DRIVERS)/gpio/include \
      -I$(PLATFORM)/include \
      -I$(HW)

# Output
ELF = $(BUILD_DIR)/main.elf
BIN = $(BUILD_DIR)/main.bin

# Host test toolchain (for running unit tests on host)
HOST_CC     = gcc
HOST_CFLAGS = -I$(ROOT)/drivers/gpio/include -I$(ROOT)/platform/include -I$(ROOT)/hw -I$(ROOT)/tests -Wall -Wextra -g
HOST_CFLAGS += -I/opt/homebrew/include
HOST_LDFLAGS += -L/opt/homebrew/lib -lcmocka

# Ensure driver is compiled for host tests with the MMIO stub: define UNIT_TEST_HOST and pre-include mmio_stub.h
HOST_CFLAGS += -DUNIT_TEST_HOST -include $(ROOT)/tests/mmio_stub.h

TEST_BIN    = $(BUILD_DIR)/test_gpio

.PHONY: all flash clean test run-tests

all: $(BIN)

$(ELF): $(SRC) | $(BUILD_DIR)
	@$(CC) $(MCU_FLAGS) $(INC) $(SRC) -T $(ROOT)/linker.ld -o $@

$(BIN): $(ELF)
	@$(OBJCOPY) -O binary $< $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

flash: $(BIN)
	@openocd -f interface/stlink.cfg -f target/stm32l4x.cfg \
		-c "adapter speed 480" \
		-c "program $(BIN) 0x08000000 verify reset exit"

clean:
	@rm -rf $(BUILD_DIR)

# Build and run host unit tests (requires libcmocka-dev / cmocka installed)
test: $(TEST_BIN)
	@echo "Running host unit tests..."
	@$(TEST_BIN)

$(TEST_BIN): $(ROOT)/tests/test_gpio.c $(ROOT)/tests/mmio_stub.h $(ROOT)/drivers/gpio/src/gpio.c $(ROOT)/tests/mmio_stub.c | $(BUILD_DIR)
	@echo "Compiling host tests..."
	@$(HOST_CC) $(HOST_CFLAGS) $(ROOT)/tests/test_gpio.c $(ROOT)/drivers/gpio/src/gpio.c $(ROOT)/tests/mmio_stub.c -o $(TEST_BIN) $(HOST_LDFLAGS)

run-tests: test

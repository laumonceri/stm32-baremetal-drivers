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
      $(DRIVERS)/spi/src/spi.c \
      $(DRIVERS)/gpio/src/gpio.c \
      $(PLATFORM)/src/rcc.c \
      $(STARTUP)/startup_stm32l4xx.c

# Include paths
INC = -I$(DRIVERS)/spi/include \
      -I$(DRIVERS)/gpio/include \
      -I$(PLATFORM)/include \
      -I$(HW)

# Output
ELF = $(BUILD_DIR)/main.elf
BIN = $(BUILD_DIR)/main.bin

.PHONY: all flash clean

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

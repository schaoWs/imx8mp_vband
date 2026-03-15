CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
PROJECT = m7_sovereignty

# Paths
SDK_DIR = /home/sharedaccount/mcux-sdk-imx8mp
FIRMWARE_DIR = /home/sharedaccount/m7_bridge/firmware
DEVICE_DIR = /home/sharedaccount/m7_bridge/devices/MIMX8ML8

# Compiler Flags
CFLAGS = -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -O0 -g \
         -I$(FIRMWARE_DIR) -I$(DEVICE_DIR) -DCPU_MIMX8ML8DVNLZ

# Linker Flags
LDFLAGS = -T$(FIRMWARE_DIR)/MIMX8ML8xxxLZ_ram.ld -nostartfiles --specs=nosys.specs -Wl,-Map=$(PROJECT).map

# Source Files
SRCS = $(FIRMWARE_DIR)/main.c $(FIRMWARE_DIR)/startup_MIMX8ML8_cm7.S

all: $(PROJECT).bin

$(PROJECT).elf: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	@echo "Build Complete: $(PROJECT).bin"

clean:
	rm -f *.elf *.bin *.map

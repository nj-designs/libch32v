APP ?= blinky

DEVICE ?= ch32v003f4p6

# Need gnuXX to allow __asm
CSTD = gnu2x

APP_DIR = app/$(APP)

CFG_INI_FILE = cfg/app_cfg.ini

DEVICE_CFG_FILE = libch32/cfg/devices.ini

# Read in required values from $(CFG_INI_FILE)
FAMILY=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) family)
ifeq ($(FAMILY),)
$(error Can't find value for $(DEVICE).FAMILY)
endif

HAL=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) hal)
ifeq ($(HAL),)
HAL = $(FAMILY)
endif

#  python3 scripts/get_device_cfg.py libch32/cfg/devices.ini ch32v003f4p6 flash2
FLASH_SIZE=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) flash_size)
ifeq ($(FLASH_SIZE),)
$(error Can't find value for $(DEVICE).flash_size)
endif

FLASH_START=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) flash_start)
ifeq ($(FLASH_START),)
$(error Can't find value for $(DEVICE).flash_start)
endif

SRAM_SIZE=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) sram_size)
ifeq ($(SRAM_SIZE),)
$(error Can't find value for $(DEVICE).sram_size)
endif

SRAM_START=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) sram_start)
ifeq ($(SRAM_START),)
$(error Can't find value for $(DEVICE).sram_start)
endif

LD_SCRIPT=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) ld_script)
ifeq ($(LD_SCRIPT),)
$(error Can't find value for $(DEVICE).ld_script)
endif

MARCH=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) march)
ifeq ($(MARCH),)
$(error Can't find value for $(DEVICE).march)
endif

MABI=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) mabi)
ifeq ($(MABI),)
$(error Can't find value for $(DEVICE).mabi)
endif

FLASH_PROG_ADDR=$(shell python3 scripts/get_device_cfg.py $(DEVICE_CFG_FILE) $(DEVICE) flash_prog_addr)
ifeq ($(FLASH_PROG_ADDR),)
$(error Can't find value for $(DEVICE).flash_prog_addr)
endif



$(info $$FAMILY: '${FAMILY}')
$(info $$HAL: '${HAL}')
$(info $$FLASH_START: '${FLASH_START}')
$(info $$FLASH_SIZE: '${FLASH_SIZE}' KiB)
$(info $$SRAM_START: '${SRAM_START}')
$(info $$SRAM_SIZE: '${SRAM_SIZE}' KiB)
$(info $$LD_SCRIPT: '${LD_SCRIPT}')
$(info $$MARCH: '${MARCH}')
$(info $$FLASH_PROG_ADDR: '${FLASH_PROG_ADDR}')

APP_C_SRCS := $(wildcard $(APP_DIR)/*.c)

# All build artifacts go here
BUILD = build

LIB_INC_DIR	= libch32/$(HAL)/include
LIB_SRC_DIR = libch32/$(HAL)/src

DEFS	= PRINTF_INCLUDE_CONFIG_H

OPTIMIZE = s

TOOL_CHAIN_PREFIX ?= riscv32-unknown-elf

# Include all WCH lib files and let linker drop unused code etc.
LIB_C_SRC = $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_A_SRC = $(wildcard $(LIB_SRC_DIR)/*.S)

CSRC = $(APP_C_SRCS) $(LIB_C_SRC)
ASRC = $(APP_A_SRCS) $(LIB_A_SRC)

ADEFS	= $(DEFS)

CC      = $(TOOL_CHAIN_PREFIX)-gcc
OBJCOPY = $(TOOL_CHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOL_CHAIN_PREFIX)-objdump
SIZE    = $(TOOL_CHAIN_PREFIX)-size
NM      = $(TOOL_CHAIN_PREFIX)-nm
FLASH_TOOL   = wlink

CFLAGS += -std=$(CSTD) -march=$(MARCH) -mabi=$(MABI) -ffreestanding -fno-pic
CFLAGS += -O$(OPTIMIZE)
CFLAGS += -Werror -g -Wall -Wextra
CFLAGS += $(addprefix -I,$(LIB_INC_DIR))
CFLAGS += -I$(BUILD)
CFLAGS += $(addprefix -D,$(DEFS))
CFLAGS += -ffunction-sections -fdata-sections

ASFLAGS += $(addprefix -D,$(ADEFS)) -Wa,-gstabs,-g$(DEBUG)
ALL_ASFLAGS = -march=$(MARCH) -mabi=$(MABI) -I. -x assembler-with-cpp $(ASFLAGS)

COBJ = $(CSRC:.c=.o)
AOBJ = $(ASRC:.S=.o)
COBJ := $(addprefix $(BUILD)/,$(COBJ))
AOBJ := $(addprefix $(BUILD)/,$(AOBJ))

LDFLAGS += -Wl,-Map,$(BUILD)/$(APP).map -nostdlib -nodefaultlibs -nolibc -nostartfiles
LDFLAGS +=-Wl,--no-relax -Wl,--gc-sections
LDFLAGS +=-Wl,--defsym=RAM_ORIGIN=$(SRAM_START) -Wl,--defsym=RAM_LENGTH=$(SRAM_SIZE)
LDFLAGS +=-Wl,--defsym=FLASH_ORIGIN=$(FLASH_START) -Wl,--defsym=FLASH_LENGTH=$(FLASH_SIZE)
LDFLAGS +=-Wl,-Tlibch32/$(HAL)/ld/link.ld

all: $(BUILD)/device_config.h elf lst sym bin

elf: $(BUILD)/$(APP).elf
lst: $(BUILD)/$(APP).lst
sym: $(BUILD)/$(APP).sym
bin: $(BUILD)/$(APP).bin
hex: $(BUILD)/$(APP).hex

%.elf:  $(COBJ) $(AOBJ)
	@echo
	@echo Linking...
	$(CC) $(CFLAGS) $(AOBJ) $(COBJ) $(LDFLAGS) --output $@
	$(SIZE) -A -x $@

%.lst: %.elf
	@echo
	$(OBJDUMP) -h -S -C $< > $@

size: $(BUILD)/$(APP).elf
	@echo
	$(SIZE) -A -x $<
	$(SIZE) $<

%.sym: %.elf
	@echo
	$(NM) -n $< > $@

%.bin: %.elf
	@echo
	$(OBJCOPY) -O binary $< $@

%.hex: %.elf
	@echo
	$(OBJCOPY) -O ihex $< $@

$(COBJ) : $(BUILD)/%.o : %.c $(BUILD)/device_config.h
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(CFLAGS) -Wp,-MP,-M,-MT,$@,-MF,$(BUILD)/$(*F).d  $< -o $@

$(AOBJ) : $(BUILD)/%.o : %.S $(BUILD)/device_config.h
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

$(BUILD)/device_config.h: $(DEVICE_CFG_FILE)
	@mkdir -p `dirname $@`
	@python3 scripts/gen_cfg2.py $< $(DEVICE) $@

flash: $(BUILD)/$(APP).bin
	$(FLASH_TOOL) flash --address $(FLASH_PROG_ADDR) $<

clean:
	@rm -rfv $(BUILD)

.PHONY: all clean size flash

# Include the dependency files.
-include $(wildcard $(BUILD)/*.d)

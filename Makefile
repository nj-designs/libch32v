APP ?= led-flash

DEVICE ?= ch32v203g6u6

# Need gnuXX to allow __asm
CSTD = gnu2x

APP_DIR = app/$(APP)

CFG_INI_FILE = cfg/app_cfg.ini

DEVICE_CFG_FILE = lib/cfg/devices.ini

APP_CFG_FILE = $(APP_DIR)/app.ini

DEVICE_CFG_TOOL = scripts/get_device_cfg.py
DEVICE_HEADER_TOOL = scripts/gen_device_header.py
APP_HEADER_TOOL = scripts/gen_app_header.py
APP_DEF_TOOL = scripts/gen_app_defines.py
DEV_DEF_TOOL = scripts/gen_device_defines.py

# Read in required values from $(CFG_INI_FILE)
FAMILY=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) family)
ifeq ($(FAMILY),)
$(error Can't find value for $(DEVICE).FAMILY)
endif


#  python3 $(DEVICE_CFG_TOOL) lib/cfg/devices.ini ch32v203g6u6 flash2
FLASH_SIZE=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) flash_size)
ifeq ($(FLASH_SIZE),)
$(error Can't find value for $(DEVICE).flash_size)
endif

FLASH_START=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) flash_start)
ifeq ($(FLASH_START),)
$(error Can't find value for $(DEVICE).flash_start)
endif

SRAM_SIZE=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) sram_size)
ifeq ($(SRAM_SIZE),)
$(error Can't find value for $(DEVICE).sram_size)
endif

SRAM_START=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) sram_start)
ifeq ($(SRAM_START),)
$(error Can't find value for $(DEVICE).sram_start)
endif

LD_SCRIPT=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) ld_script)
ifeq ($(LD_SCRIPT),)
$(error Can't find value for $(DEVICE).ld_script)
endif

MARCH=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) march)
ifeq ($(MARCH),)
$(error Can't find value for $(DEVICE).march)
endif

MABI=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) mabi)
ifeq ($(MABI),)
$(error Can't find value for $(DEVICE).mabi)
endif

FLASH_PROG_ADDR=$(shell python3 $(DEVICE_CFG_TOOL) $(DEVICE_CFG_FILE) $(DEVICE) flash_prog_addr)
ifeq ($(FLASH_PROG_ADDR),)
$(error Can't find value for $(DEVICE).flash_prog_addr)
endif

APP_DEFS=$(shell python3 $(APP_DEF_TOOL) $(APP_CFG_FILE))
ifeq ($(APP_DEFS),)
$(error Failed to generate app defines from $(APP_CFG_FILE))
endif

DEV_DEFS=$(shell python3 $(DEV_DEF_TOOL) $(DEVICE_CFG_FILE) $(DEVICE))
ifeq ($(DEV_DEFS),)
$(error Failed to generate app defines from $(DEVICE_CFG_FILE))
endif


# $(info $$FAMILY: '${FAMILY}')
# $(info $$FLASH_START: '${FLASH_START}')
# $(info $$FLASH_SIZE: '${FLASH_SIZE}' KiB)
# $(info $$SRAM_START: '${SRAM_START}')
# $(info $$SRAM_SIZE: '${SRAM_SIZE}' KiB)
# $(info $$LD_SCRIPT: '${LD_SCRIPT}')
# $(info $$MARCH: '${MARCH}')
# $(info $$FLASH_PROG_ADDR: '${FLASH_PROG_ADDR}')
# $(info $$APP_DEFS: '${APP_DEFS}')
$(info $$DEV_DEFS: '${DEV_DEFS}')

APP_C_SRCS := $(wildcard $(APP_DIR)/*.c)

# All build artifacts go here
BUILD = build

LIB_INC_DIR	= lib/include
LIB_BASE_SRC_DIR = lib/src
LIB_FAMILY_SRC_DIR = $(LIB_BASE_SRC_DIR)/$(FAMILY)

DEFS = $(APP_DEFS) $(DEV_DEFS)

OPTIMIZE = s

TOOL_CHAIN_PREFIX ?= riscv32-unknown-elf

# Include all WCH lib files and let linker drop unused code etc.
LIB_C_SRC = $(wildcard $(LIB_BASE_SRC_DIR)/*.c) $(wildcard $(LIB_FAMILY_SRC_DIR)/*.c)
LIB_A_SRC = $(wildcard $(LIB_BASE_SRC_DIR)/*.S) $(wildcard $(LIB_FAMILY_SRC_DIR)/*.S)


# $(info $$LIB_C_SRC: '${LIB_C_SRC}')
# $(info $$LIB_A_SRC: '${LIB_A_SRC}')
# $(info $$LIB_FAMILY_SRC_DIR: '${LIB_FAMILY_SRC_DIR}')

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
LDFLAGS +=-Wl,-Tlib/ld/link.ld

all: elf lst sym bin

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

$(COBJ) : $(BUILD)/%.o : %.c
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(CFLAGS) -Wp,-MP,-M,-MT,$@,-MF,$(BUILD)/$(*F).d  $< -o $@

$(AOBJ) : $(BUILD)/%.o : %.S
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

flash: $(BUILD)/$(APP).bin
	$(FLASH_TOOL) flash --address $(FLASH_PROG_ADDR) $<

clean:
	@rm -rfv $(BUILD)

.PHONY: all clean size flash

# Include the dependency files.
-include $(wildcard $(BUILD)/*.d)

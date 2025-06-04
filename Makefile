APP ?= hello-world

# DEVICE ?= ch32v307vct6
DEVICE ?= ch32v203c8t6

# Need gnuXX to allow __asm
CSTD = gnu2x

APP_DIR = app/$(APP)

DEVICE_CFG_FILE = lib/cfg/devices.ini

APP_CFG_FILE = $(APP_DIR)/app.ini


FLASH_HELPER_TOOL = scripts/flash-helper.py
CFG_HELPER_TOOL = scripts/config-helper.py
CCJ_HELPER_TOOL = scripts/gen-compile-commands.py
CCJ_FILE = compile_commands.json
CLANGD_CACHE_DIR = .cache

# $1 = DEVICE
# $2 = cfg_name
get_device_cfg = $(shell python3 $(CFG_HELPER_TOOL) get-device-cfg --device-file $(DEVICE_CFG_FILE) --device-id $(1) --cfg-name $(2))

FAMILY = $(call get_device_cfg,$(DEVICE),family)
ifeq ($(FAMILY),)
$(error Can't find value for $(DEVICE).FAMILY)
endif

FLASH_SIZE = $(call get_device_cfg,$(DEVICE),flash_size)
ifeq ($(FLASH_SIZE),)
$(error Can't find value for $(DEVICE).flash_size)
endif

FLASH_START = $(call get_device_cfg,$(DEVICE),flash_start)
ifeq ($(FLASH_START),)
$(error Can't find value for $(DEVICE).flash_start)
endif

SRAM_SIZE = $(call get_device_cfg,$(DEVICE),sram_size)
ifeq ($(SRAM_SIZE),)
$(error Can't find value for $(DEVICE).sram_size)
endif

SRAM_START = $(call get_device_cfg,$(DEVICE),sram_start)
ifeq ($(SRAM_START),)
$(error Can't find value for $(DEVICE).sram_start)
endif

LD_SCRIPT = $(call get_device_cfg,$(DEVICE),ld_script)
ifeq ($(LD_SCRIPT),)
$(error Can't find value for $(DEVICE).ld_script)
endif

MARCH = $(call get_device_cfg,$(DEVICE),march)
ifeq ($(MARCH),)
$(error Can't find value for $(DEVICE).march)
endif

MABI = $(call get_device_cfg,$(DEVICE),mabi)
ifeq ($(MABI),)
$(error Can't find value for $(DEVICE).mabi)
endif

FLASH_PROG_ADDR = $(call get_device_cfg,$(DEVICE),flash_prog_addr)
ifeq ($(FLASH_PROG_ADDR),)
$(error Can't find value for $(DEVICE).flash_prog_addr)
endif

APP_C_SRCS := $(wildcard $(APP_DIR)/*.c)

# All build artifacts go here
BUILD = build

LIBCH32V_CONFIG_FILE = $(BUILD)/libch32v-config.h

LIB_INC_DIR	= lib/include

LIB_BASE_SRC_DIR = lib/src
LIB_FAMILY_SRC_DIR = $(LIB_BASE_SRC_DIR)/$(FAMILY)

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

# ADEFS	= $(DEFS)

CC      = $(TOOL_CHAIN_PREFIX)-gcc
OBJCOPY = $(TOOL_CHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOL_CHAIN_PREFIX)-objdump
SIZE    = $(TOOL_CHAIN_PREFIX)-size
NM      = $(TOOL_CHAIN_PREFIX)-nm
GDB     = $(TOOL_CHAIN_PREFIX)-gdb

WLINK   = wlink

CFLAGS += -std=$(CSTD) -march=$(MARCH) -mabi=$(MABI) -ffreestanding -fno-pic
CFLAGS += -O$(OPTIMIZE)
CFLAGS += -Werror -g -Wall -Wextra
CFLAGS += $(addprefix -I,$(LIB_INC_DIR))
CFLAGS += -I$(BUILD)
CFLAGS += -ffunction-sections -fdata-sections

ASFLAGS += -Wa,-gstabs,-g$(DEBUG)
ASFLAGS += -I$(BUILD)
ALL_ASFLAGS = -march=$(MARCH) -mabi=$(MABI) -x assembler-with-cpp $(ASFLAGS)

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

%.elf: $(COBJ) $(AOBJ)
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

$(COBJ) : $(BUILD)/%.o : %.c $(LIBCH32V_CONFIG_FILE)
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(CFLAGS) -Wp,-MP,-M,-MT,$@,-MF,$(BUILD)/$(*F).d  $< -o $@

$(AOBJ) : $(BUILD)/%.o : %.S $(LIBCH32V_CONFIG_FILE)
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

$(LIBCH32V_CONFIG_FILE): $(DEVICE_CFG_FILE) $(APP_CFG_FILE)
	@mkdir -p `dirname $@`
	@python3 $(CFG_HELPER_TOOL) gen-header-file --app-file $(APP_CFG_FILE) --device-file $(DEVICE_CFG_FILE) --device-id $(DEVICE) --header-file $(LIBCH32V_CONFIG_FILE)

start-gdb: $(BUILD)/$(APP).elf
	$(GDB) --command run/gdb-init $<

ccj: $(LIBCH32V_CONFIG_FILE)
	@rm -fr $(CLANGD_CACHE_DIR)
	python3 $(CCJ_HELPER_TOOL) --family $(FAMILY) --out-file $(CCJ_FILE) --base-dir $(PWD) --app-name $(APP) --bld-dir $(BUILD) --cc "$(CC) -c $(CFLAGS)"

flash: $(BUILD)/$(APP).bin
	python3 $(FLASH_HELPER_TOOL) --verbose --device $(DEVICE) --address $(FLASH_PROG_ADDR) --file $<

clean:
	@rm -rfv $(BUILD) $(CCJ_FILE) $(CLANGD_CACHE_DIE)

.PHONY: all clean size flash start-gdb ccj

# Include the dependency files.
-include $(wildcard $(BUILD)/*.d)

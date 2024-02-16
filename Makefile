TARGET = ch32vtst

APP_DIR = app

APP_C_SRCS := $(wildcard $(APP_DIR)/*.c)

MARCH  = rv32ec_zicsr
MABI = ilp32e

# Need gnuXX to allow __asm
CSTD = gnu11

# All build artifacts go here
BUILD = build

INCDIRS	= libch32v/include/libch32v

DEFS	= PRINTF_INCLUDE_CONFIG_H

OPTIMIZE = s

TOOL_CHAIN_PREFIX ?= riscv32-unknown-elf

# Include all WCH lib files and let linker drop unused code etc.
LIB_C_SRC = $(wildcard libch32v/lib/src/*.c)
LIB_A_SRC = libch32v/lib/src/startup.S libch32v/lib/src/vector_ch32v003.S

CSRC = $(APP_C_SRCS) $(LIB_C_SRC)
ASRC = $(APP_A_SRCS) $(LIB_A_SRC)

ADEFS	= $(DEFS)

CC      = $(TOOL_CHAIN_PREFIX)-gcc
OBJCOPY = $(TOOL_CHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOL_CHAIN_PREFIX)-objdump
SIZE    = $(TOOL_CHAIN_PREFIX)-size
NM      = $(TOOL_CHAIN_PREFIX)-nm

CFLAGS += -std=$(CSTD) -march=$(MARCH) -mabi=$(MABI) -ffreestanding -fno-pic
CFLAGS += -O$(OPTIMIZE)
CFLAGS += -Werror -g
CFLAGS += $(addprefix -I,$(INCDIRS))
CFLAGS += $(addprefix -D,$(DEFS))
CFLAGS += -Wp,-M,-MP,-MT,$(BUILD)/$(*F).o,-MF,$(BUILD)/$(*F).d
CFLAGS += -ffunction-sections -fdata-sections

ASFLAGS += $(addprefix -D,$(ADEFS)) -Wa,-gstabs,-g$(DEBUG)
ALL_ASFLAGS = -march=$(MARCH) -mabi=$(MABI) -I. -x assembler-with-cpp $(ASFLAGS)

COBJ = $(CSRC:.c=.o)
AOBJ = $(ASRC:.S=.o)
COBJ := $(addprefix $(BUILD)/,$(COBJ))
AOBJ := $(addprefix $(BUILD)/,$(AOBJ))


LDFLAGS += -Wl,-Map,$(BUILD)/$(TARGET).map -nostdlib
LDFLAGS +=-Wl,--no-relax -Wl,--gc-sections
LDFLAGS +=-Wl,-Tlibch32v/linker/ch32v003.ld

all: elf lst sym

elf: $(BUILD)/$(TARGET).elf
lst: $(BUILD)/$(TARGET).lst
sym: $(BUILD)/$(TARGET).sym
bin: $(BUILD)/$(TARGET).bin
hex: $(BUILD)/$(TARGET).hex

%.elf:  $(COBJ) $(AOBJ)
	@echo
	@echo Linking...
	$(CC) $(CFLAGS) $(AOBJ) $(COBJ) $(LDFLAGS) --output $@
	$(SIZE) -A -x $@

%.lst: %.elf
	@echo
	$(OBJDUMP) -h -S -C $< > $@

size: $(BUILD)/$(TARGET).elf
	@echo
	$(SIZE) -A -x $<

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
	$(CC) -c $(CFLAGS) $< -o $@

$(AOBJ) : $(BUILD)/%.o : %.S
	@mkdir -p `dirname $@`
	@echo
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


clean:
	@rm -rfv $(BUILD)

# Include the dependency files.
-include $(shell mkdir $(OBJDIR) 2>/dev/null) $(wildcard $(OBJDIR)/*.d)
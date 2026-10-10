# CLAUDE.md

Guidance for Claude Code when working in this repository.

## What this is

A bare-metal C library and set of example apps for WCH CH32V RISC-V microcontrollers (CH32V003, CH32V203, CH32V307). No vendor HAL, no libc, no RTOS: everything is built with `-ffreestanding -nostdlib` from a single Makefile. Setup of the toolchain, `wlink` and OpenOCD is covered in `README.adoc`.

## Commands

```bash
make                                      # build default APP/DEVICE (see top of Makefile)
make APP=led-flash DEVICE=ch32v307vct6    # pick app (dir under app/) and device
make clean                                # removes build/, compile_commands.json and .cache/
make size                                 # section sizes
make sections                             # all section headers in the .elf (size, VMA, LMA, flags)
make ccj                                  # build, then write compile_commands.json for clangd
make flash                                # program via scripts/flash-helper.py + wlink (needs WCH-Link attached)
make start-gdb                            # gdb against OpenOCD on :3333 (start OpenOCD first, see README)
```

- Toolchain is `riscv32-unknown-elf-*` (override with `TOOL_CHAIN_PREFIX`). Python 3 is needed at build time for the config scripts.
- **Run `make clean` when changing `APP` or `DEVICE`.** Objects in `build/` are keyed by source path only, and `build/libch32v-config.h` is only regenerated when an INI file is newer than it, so a switch silently reuses the previous configuration.
- To build without disturbing `build/`, pass `BUILD=<other dir>`.
- There are no tests and no lint target. The only verification available without hardware is that it compiles (`-Wall -Wextra -Werror`) and links. Say so rather than claiming a change works on the device.
- Formatting follows `.clang-format` (2-space indent, attached braces, `int* p`, no column limit). `.clang-format-old` is the previous style, kept for reference.
- `make flash` asks `wlink` for the status of each attached WCH-Link and flashes the one whose chip matches `DEVICE`, so several probes can be connected at once.

## Build configuration flow

Two INI files are combined by `scripts/config-helper.py` into one generated header, `build/libch32v-config.h`, which defines everything as `#define`s. Nothing is passed as `-D`. Library headers include it themselves; any source that tests `LIBCH32_*` or `APP_*` directly must `#include "libch32v-config.h"`. The Makefile also calls the script at parse time (`get-device-cfg`) to read `family`, `march`, `mabi` and the flash/SRAM layout.

**`lib/cfg/devices.ini` → `LIBCH32_*`.** Lookup is by longest prefix of the device name: for `ch32v203c8t6` each key is taken from the most specific of `[ch32v203c8t6]`, `[ch32v203c8]`, `[ch32v203]`, `[ch32v]` that defines it. Every key becomes `LIBCH32_<KEY>`, plus:

- `LIBCH32_DEVICE_ID` = `WCH_<DEVICE>`, where `WCH_<DEVICE>` is a CRC32 of the name
- `LIBCH32_<FAMILY>_FAMILY` (e.g. `LIBCH32_V307_FAMILY`)
- `has_<periph> = 1` keys become `LIBCH32_HAS_<PERIPH>`, which gate peripheral instances
- `cpu_core_*`, `int_nest_depth`, `sys_tick_width`, `hsi_freq`, `max_sysclk` drive core, interrupt and clock code

`family` also selects the extra source dir `lib/src/<family>/` (vector table and `rcc.c`). The `ld_script` key is ignored; `lib/ld/link.ld` is always used.

**`app/<APP>/app.ini` → `APP_*`.** Keys in `[DEFAULT]` become `APP_<KEY>`; a bare key with no value becomes a flag define. The file must exist (it is a make prerequisite). Keys the library reads:

- `hse_freq`: external crystal frequency. If set, `rcc_init()` runs from HSE, otherwise from HSI
- `stdout_buffer_size`: size of the buffered stdout ring
- `printf_disable_support_*`: trim the bundled printf

Other keys still present in some `app.ini` files (`sysclk_freq`, `provide_printf`) are no longer read by the library.

Device-specific code is selected with `#if LIBCH32_DEVICE_ID == WCH_CH32V307VCT6`. An undefined `WCH_*` name evaluates to 0, so `DEVICE` must be spelled exactly as the code expects: `ch32v307vct6`, `ch32v203c8t6`, `ch32v203g6u6` or `ch32v003f4`. Supporting a new package means adding a branch in each of `gpio.h`, `gpio.c`, `usart.c`, `i2c.c`, `gptm.c` and `stdout-buffered.c`.

## Architecture

**Startup.** `_start` (in the family `vector.S`, section `.init`) jumps to `handle_reset` in `lib/src/reset.S`, which sets `gp`/`sp`, fills the stack with `0xDEAD`, copies `.data`, clears `.bss`, calls `init_system()` (`core_init()` then `rcc_init()`), enables interrupt nesting to `int_nest_depth`, sets `mtvec` for vectored interrupts, then `mret`s into `main`. Apps define `void main(void)` and call `stdout_init()` themselves before using `printf`.

**Peripheral registers are linker-placed structs, not pointer casts.** Each peripheral has a `struct XxxRegMap` of `volatile` fields in `lib/include/<periph>.h`, an instance in `lib/src/<periph>.c` tagged `__attribute__((section(".gpio_a")))` and guarded by `#ifdef LIBCH32_HAS_GPIOA`, and a matching `NOLOAD` output section at the peripheral's absolute address in `lib/ld/link.ld`. Code then writes `gpio_a.outdr = ...` directly. Adding a peripheral instance touches four places: header `extern`, `.c` definition, `link.ld` section, and `has_*` in `devices.ini`. If the `link.ld` section is missing the link still succeeds, but the struct is placed in RAM as an orphan section and writes never reach the hardware; check with `make sections`.

**Interrupts.** Each family has its own vector table in `lib/src/<family>/vector.S`. Every handler is a weak symbol; define a function with the same name, declared with `NJD_IRQ_ATTRIBUTE` (`__attribute__((interrupt))`, from `core.h`), to take it over. Names are mixed: some are `int_handler_*` (`int_handler_nmi`, `int_handler_hard_fault`, `int_handler_dma1_chan4`), the rest keep WCH's `*_IRQHandler` names. Check the table for the exact symbol; a typo links fine and simply never fires. `core.h` provides the PFIC helpers (enable, pending, priority, threshold); `core_init()` sets every interrupt to the lowest priority. Add the interrupt number to `enum PFICIntNum` if it is missing.

**Clocks.** `lib/src/<family>/rcc.c` implements `rcc_init()`, `rcc_set_peripheral_clk()` and `rcc_get_clk_freq()`. Drivers ask for bus frequencies through `rcc_get_clk_freq()` rather than assuming them. A peripheral's clock must be enabled before use, e.g. `rcc_set_peripheral_clk(RCC_IOPA_ID, 1)`.

**Everything in `lib/src/*.c` plus `lib/src/<family>/*.c` is compiled into every app**; `--gc-sections` drops what is unused. Consequence: every library file must compile cleanly for every device, even for peripherals the device lacks.

**GPIO pins** are `enum GPIOPinId` values (`PIN_PA9`) encoding port index and pin, with a separate enum per device in `gpio.h` listing only pins that package bonds out.

## Code conventions

- `-std=gnu2x`: digit separators (`8'000'000`), binary literals (`0b1'10'00`) and enums with a fixed underlying type (`enum Foo : uint8_t`) are used freely.
- Functions are `<module>_<verb>` snake_case (`gpio_pin_init`, `can_tx_req`); register structs are `XxxRegMap` with lowercase field names matching the reference manual; register bit constants are `static const uint32_t` named after the manual.
- Headers use `#pragma once` and Doxygen-style comments with the file header block seen in existing files.
- No libc: only freestanding headers (`stdint.h`, `stddef.h`, `stdbool.h`). The V003 has no M extension, so the libgcc helpers the compiler calls (`__mulsi3`, `__udivsi3`) are provided in `lib/src/v003/math.S`.

## Current state (verified by building every app for every device)

- **`ch32v307vct6` and `ch32v203c8t6`** build `can-example`, `hello-world`, `led-flash`, `TB6612FNG-demo`, `usart1-tx-dma` and `usart1-tx-polled`.
- **`ch32v203g6u6`** builds nothing: `gpio.c` references `gpio_d`, which `devices.ini` no longer enables for that package.
- **`ch32v003f4`** builds nothing: `core.h` has no PFIC priority definitions for the V2A core and stops at `#error "unsupported device"`. The V003 port has not been carried through the RCC/PFIC refactor.
- **Stale apps:** `blinky`, `buffered-stdio`, `demo` and `usart1-tx-interrupt` have no `app.ini`; `i2c-ds3231` and `i2c-scan` include a `device_config.h` that no longer exists; `ch32v003-esc` uses the removed `struct RCCCfgValues`.
- **Misplaced registers:** `link.ld` has no sections for `.usart3` to `.usart8`, so on the V307 (USART3-8) and V203C8 (USART3-4) those structs land in RAM.

The build log shown in `README.adoc` predates the generated config header and does not match current output.

## Reference material

`docs/` holds the WCH datasheets, reference manuals and board schematics (large PDFs; read specific page ranges). Register layouts and bit names in the headers follow these manuals. `ch32v003-notes.adoc` and `ch32v203-notes.adoc` are the author's scratch links. `app/can-example/` has a Python host-side test script (`test_cmd.py`, python-can).

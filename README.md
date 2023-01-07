# LIBCV32V

## OVERVIEW

A simple Makefile based project for WCH's range of CH32Vxxx [RISC-V uControllers](http://www.wch-ic.com/products/categories/47.html?pid=5).

There's not too much original work here, most of it has been collated from various file dumps etc. from WCH's own site and other pages.

Currently only supports CH32V003 devices.

FYI, the development board I have looks like this:

![Dev Board](/pics/ch32v003-dev-board.png)

This whole project is really early days, so the general layout etc. could and most probably will change as bugs are fixed, and support for other devices is added.

Also PRs are always welcome.

## TOOLS SETUP

Only tested on **Ubuntu 22.04**, but should work on any recent linux.

### GCC COMPILER

The following will install to ```${HOME}/opt/riscv``` ensure ```${HOME}/opt/riscv/bin``` is in ```${PATH}``` before running.

```bash
# Prereqs
sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev \
	libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils \
	bc zlib1g-dev libexpat-dev
# Get toolchain source
mkdir ~/github.com
cd ~/github.com
git clone https://github.com/riscv/riscv-gnu-toolchain riscv/riscv-gnu-toolchain
cd riscv/riscv-gnu-toolchain
# Confure build
./configure --prefix=${HOME}/opt/riscv --with-arch=rv32g --with-abi=ilp32
# build
make -j$(nproc)
```

### OPENOCD

WCH seems to need a modifed openocd. I've been uable to find the source code for it, but a "working binary" is provided in the **run** directory.

I found the easiest setup is to install the following packages first.

```bash
# Prereqs. for openocd
sudo apt install libjaylink-dev libjaylink0 libhidapi-dev libhidapi-hidraw0 libhidapi-libusb0 openocd
# udev rule for programmer
sudo cp run/60-libch32v.rules /etc/udev/rules.d/60-libch32v.rules
# reload rules
sudo udevadm control  --reload-rules
```

## BASIC WORKFLOW

Once all of the tools above are install/configured, you can build and test a version of **blinky** as follows.

#### BUILD

To build, simply run **make** in the root of this repo.

If all goes well, the last few lines of output shoud look something like this:
```
Linking...
riscv32-unknown-elf-gcc -std=gnu11 -march=rv32ec -mabi=ilp32e -ffreestanding -fno-pic -Os -Werror -g -Ilibch32v/include/libch32v -DPRINTF_INCLUDE_CONFIG_H -Wp,-M,-MP,-MT,build/ch32vtst.o,-MF,build/ch32vtst.d -ffunction-sections -fdata-sections build/libch32v/lib/src/startup.o build/libch32v/lib/src/vector_ch32v003.o build/app/main.o build/libch32v/lib/src/ch32v00x_adc.o build/libch32v/lib/src/ch32v00x_dbgmcu.o build/libch32v/lib/src/ch32v00x_dma.o build/libch32v/lib/src/ch32v00x_exti.o build/libch32v/lib/src/ch32v00x_flash.o build/libch32v/lib/src/ch32v00x_gpio.o build/libch32v/lib/src/ch32v00x_i2c.o build/libch32v/lib/src/ch32v00x_it.o build/libch32v/lib/src/ch32v00x_iwdg.o build/libch32v/lib/src/ch32v00x_misc.o build/libch32v/lib/src/ch32v00x_opa.o build/libch32v/lib/src/ch32v00x_pwr.o build/libch32v/lib/src/ch32v00x_rcc.o build/libch32v/lib/src/ch32v00x_spi.o build/libch32v/lib/src/ch32v00x_tim.o build/libch32v/lib/src/ch32v00x_usart.o build/libch32v/lib/src/ch32v00x_wwdg.o build/libch32v/lib/src/debug.o build/libch32v/lib/src/system_ch32v00x.o -Wl,-Map,build/ch32vtst.map -nostdlib -Wl,--no-relax -Wl,--gc-sections -Wl,-Tlibch32v/linker/ch32v003.ld --output build/ch32vtst.elf
riscv32-unknown-elf-size -A -x build/ch32vtst.elf
build/ch32vtst.elf  :
section                       size         addr
.vector                       0xa0          0x0
.text                        0x410         0xa0
.data                          0x4   0x20000000
.sbss.NVIC_Priority_Group      0x4   0x20000004
.sbss.p_ms                     0x2   0x20000008
.sbss.p_us                     0x1   0x2000000a
.stack                       0x200   0x20000600
.riscv.attributes             0x25          0x0
.comment                       0xf          0x0
.debug_line                 0x2796          0x0
.debug_info                 0x2168          0x0
.debug_abbrev                0xb57          0x0
.debug_aranges               0x2b8          0x0
.debug_str                   0xd88          0x0
.debug_ranges                 0x20          0x0
.debug_rnglists              0x2b1          0x0
.debug_line_str              0x1d1          0x0
.debug_frame                 0x4d4          0x0
.debug_loclists              0xd8d          0x0
Total                       0x8287



riscv32-unknown-elf-objdump -h -S -C build/ch32vtst.elf > build/ch32vtst.lst

riscv32-unknown-elf-nm -n build/ch32vtst.elf > build/ch32vtst.sym
```

### PROGRAM

To program what's just been built, run the following:

```
➜  libch32v git:(main) ✗ ./run/openocd -f run/wch-riscv.cfg  -c init -c halt  -c "program build/ch32vtst.elf" -c exit

Open On-Chip Debugger 0.11.0+dev-02215-gcc0ecfb6d-dirty (2022-10-10-10:35)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : only one transport option; autoselect 'jtag'
Ready for Remote Connections
Info : WCH-LinkE-CH32V307  mod:RV version 2.7
Info : wlink_init ok
Info : This adapter doesn't support configurable speed
Info : JTAG tap: riscv.cpu tap/device found: 0x00000001 (mfg: 0x000 (<invalid>), part: 0x0000, ver: 0x0)
Warn : Bypassing JTAG setup events due to errors
Info : [riscv.cpu.0] datacount=2 progbufsize=8
Info : Examined RISC-V core; found 1 harts
Info :  hart 0: XLEN=32, misa=0x40800014
[riscv.cpu.0] Target successfully examined.
Info : starting gdb server for riscv.cpu.0 on 3333
Info : Listening on port 3333 for gdb connections
Info : JTAG tap: riscv.cpu tap/device found: 0x00000001 (mfg: 0x000 (<invalid>), part: 0x0000, ver: 0x0)
Warn : Bypassing JTAG setup events due to errors
** Programming Started **
Info : device id = 0x3d05abcd
Info : flash size = 16kbytes
Info : Hart 0 unexpectedly reset!
** Programming Finished **
➜  libch32v git:(main) ✗
```

Just reset the target and you should be running the new code.

### DEBUG

If you want to debug your program, first program the device as above.

Run the following
```
➜  libch32v git:(main) ✗ ./run/openocd -f run/wch-riscv.cfg -c init -c halt -c wlink_reset_resume
Open On-Chip Debugger 0.11.0+dev-02215-gcc0ecfb6d-dirty (2022-10-10-10:35)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : only one transport option; autoselect 'jtag'
Ready for Remote Connections
Info : WCH-LinkE-CH32V307  mod:RV version 2.7
Info : wlink_init ok
Info : This adapter doesn't support configurable speed
Info : JTAG tap: riscv.cpu tap/device found: 0x00000001 (mfg: 0x000 (<invalid>), part: 0x0000, ver: 0x0)
Warn : Bypassing JTAG setup events due to errors
Info : [riscv.cpu.0] datacount=2 progbufsize=8
Info : Examined RISC-V core; found 1 harts
Info :  hart 0: XLEN=32, misa=0x40800014
[riscv.cpu.0] Target successfully examined.
Info : starting gdb server for riscv.cpu.0 on 3333
Info : Listening on port 3333 for gdb connections
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : Hart 0 unexpectedly reset!
```

In another terminal, you can connect a gdb session as follows:
```
➜  libch32v git:(main) ✗ riscv32-unknown-elf-gdb --command run/gdb-init build/ch32vtst.elf
GNU gdb (GDB) 12.1
Copyright (C) 2022 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "--host=x86_64-pc-linux-gnu --target=riscv32-unknown-elf".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from build/ch32vtst.elf...
_vector_table () at libch32v/lib/src/vector_ch32v003.S:6
6         j reset_handler                   // No.  0 : Reset Handler
(gdb) b main
Breakpoint 1 at 0x192: file app/main.c, line 47.
Note: automatically using hardware breakpoints for read-only addresses.
(gdb) c
Continuing.

Breakpoint 1, main () at app/main.c:47
47        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
(gdb) info reg
ra             0x11e    0x11e <init_data_done+36>
sp             0x20000800       0x20000800
gp             0x20000000       0x20000000 <SystemCoreClock>
tp             0x9848402        0x9848402
t0             0x80     128
t1             0x2dc6c00        48000000
t2             0x80000  524288
fp             0xe000f000       0xe000f000
s1             0x4d269250       1294373456
a0             0x20000004       536870916
a1             0x20000004       536870916
a2             0x20000  131072
a3             0x40021000       1073876992
a4             0x8      8
a5             0x8      8
a6             0x0      0
a7             0x0      0
s2             0x0      0
s3             0x0      0
s4             0x0      0
s5             0x0      0
s6             0x0      0
s7             0x0      0
s8             0x0      0
s9             0x0      0
s10            0x0      0
s11            0x0      0
t3             0x0      0
t4             0x0      0
t5             0x0      0
t6             0x0      0
pc             0x192    0x192 <main>
(gdb)
```

## TODO

In no particular order and just off the top of my head.

- [ ] Add examples for USART, I2C etc
- [ ] Investigate adding support for libc. [picolib](https://github.com/picolibc/picolibc) looks promising
- [ ] General bug fixes and tidy up
- [ ] Support other variants of WCH's CH32V range of uControllers
- [ ] Understand what changes WCH added to openocd to work with their devices
- [ ] Better documentaion

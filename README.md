# vint-kernel

### Required Software & Libraries

Before building the kernel, make sure you have the following packages installed:

* **nasm** (Assembler for `boot.asm`)
* **gcc / g++** (With 32-bit multilib support)
* **ld** (GNU Linker)
* **grub2** & **xorriso** (To generate the bootable ISO)
* **qemu-system-i386** & **SDL2** (To emulate and run the OS)

---

### How to build and run on Linux

Run the following command in the root directory of the project to compile all modules, create the ISO image, and launch it in QEMU:

```bash
nasm -f elf32 boot.asm -o boot.o && \
g++ -m32 -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-pic && \
g++ -m32 -c calculator.cpp -o calculator.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-pic && \
g++ -m32 -c sysinfo.cpp -o sysinfo.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-pic && \
g++ -m32 -c matrix.cpp -o matrix.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-pic && \
g++ -m32 -c reboot.cpp -o reboot.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-pic && \
ld -m elf_i386 -T linker.ld -o vintkrnl.bin boot.o kernel.o calculator.o sysinfo.o matrix.o reboot.o && \
mkdir -p iso/boot/grub && \
cp vintkrnl.bin iso/boot/ && \
echo -e 'menuentry "Vint OS" {\n\tmultiboot /boot/vintkrnl.bin\n\tboot\n}' > iso/boot/grub/grub.cfg && \
grub2-mkrescue -o myos.iso iso && \
qemu-system-i386 -cdrom myos.iso -display sdl

# mosdev-kernel
Unix-inspired operating system for i386 and x86_64. Developed as a hobby.

You will need a cross-compiler to build this project. The one I use is a modified version of GCC installed at ```/usr/local/cross```. The reason being that the standard GCC is built specifically for your operating system and will not generate a valid mosdev executable.

To compile:
```
make
```

This will generate the kernel binary in ```/bin``` as ```vmmos-i386```. The kernel binary is multiboot compatible and can be bootstrapped by any multiboot bootloader (such as GNU GRUB).

Once you have compiled the kernel:
```
make iso
```
Will generate a bootable ISO image at ```bin/vmmos-i386.iso```. You can use this to boot the operating system on real hardware.

Or run it in a virtual machine (I use QEMU):
```
qemu-system-i386 --cdrom bin/vmmos-i386.iso
```

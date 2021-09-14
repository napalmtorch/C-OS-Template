# C++-OS-Template
A simple operating system template for quickly getting started with the more interesting parts of OS dev. This is not perfect, but its purpose is more for personal use, however if you find it interesting, there ya go!

# Features/Components
- GRUB 2 Multiboot
- Serial controller(COM1-COM4)
- ISR and IDT, modified but originally from https://github.com/cfenollosa/os-tutorial
- PIT controller with millisecond calculation
- Simple debugging interface(String formatting mostly compatible with printf using variadic arguments)
- Dynamic memory manager(Flat memory model, allocate and free both supported)
- String library
- Math library(Unfinished)
- VGA Fonts(4 different styles, spacing support)

# Potential Implementations
- VGA(Text and graphics mode)
- VESA(Emulated text mode and graphics mode)
- Software multitasking(Context switching via PIT)
- PS/2 Keyboard & Mouse
- Real Time Clock
- ATA Controller
- Graphics rendering(Direct, buffered, and bitmap)
- System calls
- Bitmaps
- Console/Terminal
- Command line interface
- IOStreams
- File System(Homemade is more fulfillful, otherwise FAT32 and/or EXT2)
- Virtual machine(Personal choice for external code execution, however binary/ELF loading is likely preferred)

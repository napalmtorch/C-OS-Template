# Clear the console
clear

# Setup build directory
rm -r 'Build/Output'
mkdir 'Build/Output'
mkdir 'Build/Output/Objs/'

# NASM assembler files
nasm -felf32 'Source/Boot/Bootstrap.asm' -o 'Build/Output/Bootstrap.o'
nasm -felf32 'Source/Boot/GDT.asm' -o 'Build/Output/Objs/GDT.o'
nasm -felf32 'Source/Kernel/Hardware/Interrupts/IRQs.asm' -o 'Build/Output/Objs/IRQs.o'

# Entry C++ file
i686-elf-g++ -w -IInclude -c "Source/Boot/Entry.cpp" -o "Build/Output/Objs/Entry.o" -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable

# Kernel/Core/
for file in Source/Kernel/Core/*.cpp 
do
infile=$(basename $file)
outfile="$(echo $infile | sed 's/cpp/o/g')"
i686-elf-g++ -w -IInclude -c "Source/Kernel/Core/$infile" -o "Build/Output/Objs/$outfile" -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable
done

# Kernel/Hardware/
for file in Source/Kernel/Hardware/*.cpp 
do
infile=$(basename $file)
outfile="$(echo $infile | sed 's/cpp/o/g')"
i686-elf-g++ -w -IInclude -c "Source/Kernel/Hardware/$infile" -o "Build/Output/Objs/$outfile" -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable
done

# Kernel/Hardware/Management
for file in Source/Kernel/Hardware/Management/*.cpp 
do
infile=$(basename $file)
outfile="$(echo $infile | sed 's/cpp/o/g')"
i686-elf-g++ -w -IInclude -c "Source/Kernel/Hardware/Management/$infile" -o "Build/Output/Objs/$outfile" -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable
done

# Kernel/Hardware/Interrupts/
for file in Source/Kernel/Hardware/Interrupts/*.cpp 
do
infile=$(basename $file)
outfile="$(echo $infile | sed 's/cpp/o/g')"
i686-elf-g++ -w -IInclude -c "Source/Kernel/Hardware/Interrupts/$infile" -o "Build/Output/Objs/$outfile" -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable
done

# Kernel/Lib/
for file in Source/Kernel/Lib/*.cpp 
do
infile=$(basename $file)
outfile="$(echo $infile | sed 's/cpp/o/g')"
i686-elf-g++ -w -IInclude -c "Source/Kernel/Lib/$infile" -o "Build/Output/Objs/$outfile" -fno-use-cxa-atexit -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Wno-write-strings -Wno-unused-variable
done

# Link all files
cd 'Build/Output/Objs'
i686-elf-gcc -T '../../Linker.ld' -o '../Kernel.bin' -ffreestanding -O2 -nostdlib *.o '../Bootstrap.o' -lgcc
cd '../../../'

# Create ISO image
mkdir -p 'Build/Output/isodir/boot/grub'
cp 'Build/Output/Kernel.bin' 'Build/Output/isodir/boot/kernel.bin'
cp 'Build/GrubMenu.cfg' 'Build/Output/isodir/boot/grub/grub.cfg'
grub-mkrescue -o  'ScratchOS.iso' 'Build/Output/isodir'
cp 'ScratchOS.iso' 'ScratchOS.img'

# Run QEMU instance of operating system
qemu-system-i386 -m 256M -vga std -cdrom 'ScratchOS.iso' -serial stdio -boot d -soundhw all -device e1000 -enable-kvm -rtc base=localtime -cpu host
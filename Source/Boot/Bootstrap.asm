[extern KernelEntry]
[extern GDTEnable]
[global StackTop]
[global StackBottom]

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
    align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM

section .bss
    align 16
    StackBottom:
        resb 32768
    StackTop:

section .text
global _start

_start:
    cli
    call GDTEnable
    hlt

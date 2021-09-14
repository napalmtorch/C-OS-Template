[global GDTEnable]
[extern KernelEntry]
[extern GDTCode]
[extern StackTop]

GDTEnable:
    cli
    lgdt [GDTDescriptor]
    mov  eax, cr0
    or   eax, 0x1                   ; 3. set 32-bit mode bit in cr0
    mov  cr0, eax
    jmp  CODE_SEG:InitProtectedMode ; 4. far jump by using a different segment

GDTStart: ; don't remove the labels, they're needed to compute sizes and jumps
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
GDTCode: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
GDTData:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

GDTEnd:

; GDT descriptor
GDTDescriptor:
    dw GDTEnd - GDTStart - 1 ; size (16 bit), always one less of its true size
    dd GDTStart ; address (32 bit)

; define some constants for later use
CODE_SEG equ GDTCode - GDTStart
DATA_SEG equ GDTData - GDTStart

InitProtectedMode:
    mov ax, DATA_SEG ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, StackTop
    mov esp, ebp
    
    call KernelEntry

    cli
    hlt
    jmp $
    
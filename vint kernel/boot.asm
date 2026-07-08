section .multiboot
align 4
    dd 0x1BADB002
    dd 0x03
    dd -(0x1BADB002 + 0x03)

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:

section .text
global _start
global inb
global outb
global get_cpu_vendor
extern kernel_main

_start:
    mov esp, stack_top
    call kernel_main
.hang:
    hlt
    jmp .hang

inb:
    mov edx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

outb:
    mov edx, [esp + 4]
    mov eax, [esp + 8]
    out dx, al
    ret

get_cpu_vendor:
    mov eax, 0
    cpuid
    mov eax, [esp + 4]
    mov [eax], ebx
    mov [eax + 4], edx
    mov [eax + 8], ecx
    ret
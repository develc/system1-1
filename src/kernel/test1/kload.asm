MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
section .bss
align 16
stackb:
resb 16374 ; 16KB
stackt:

section .text

global _start:function (_start.end - _start)
_start:
	mov esp, stackt
	extern kmain
	call kmain
	cli
.halt:	hlt
	jmp .halt
.end:

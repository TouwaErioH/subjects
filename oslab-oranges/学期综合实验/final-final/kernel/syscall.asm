
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               syscall.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

%include "sconst.inc"

_NR_get_ticks       equ 0 ; 要跟 global.c 中 sys_call_table 的定义相对应！
_NR_get_crc equ 1 ;crc 的中断号为1.也就是在sys_call_table中的第二个中断。

INT_VECTOR_SYS_CALL equ 0x90

; 导出符号
global	get_ticks
global get_crc
bits 32
[section .text]

; ====================================================================
;                              get_ticks
; ====================================================================
get_ticks:
	mov	eax, _NR_get_ticks
	int	INT_VECTOR_SYS_CALL
	ret

get_crc:
	mov eax,_NR_get_crc ;eax 为1，这样操作系统知道要第1号中断
	int INT_VECTOR_SYS_CALL
	ret

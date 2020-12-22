;Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.

;You should have received a copy of the GNU General Public License
;along with this program.  If not, see <http://www.gnu.org/licenses/>

use32

%macro	SAVE_REGS 0
	pushad
	push ds
	push es
	push fs
	push gs
	push ebx    ;se cambia a los descritores del kernel
	mov bx,0x10  
	mov ds,bx
        mov es,bx
        mov fs,bx
        mov gs,bx
	pop ebx
%endmacro

%macro	RESTORE_REGS 0
	pop gs
	pop fs
	pop es
	pop ds
	popad
%endmacro

extern _c_passint;
global _asm_passint
_asm_passint:
	SAVE_REGS
	;call _c_passint
	mov al,0x20
	out 0x20,al
	RESTORE_REGS
	iret
;-------------------Interrupciones por Hardware---------------------------------
extern _c_int_timer
global _asm_int_timer
_asm_int_timer:
        cli
        SAVE_REGS
        call _c_int_timer  ;procedimiento que atiende a las interrupciones por hardware
        mov al,0x20
	out 0x20,al
	RESTORE_REGS
	iret

extern _c_int_keyboard
global _asm_int_keyboard
_asm_int_keyboard:
        SAVE_REGS
        call _c_int_keyboard  ;procedimiento que atiende a las interrupciones por hardware
        mov al,0x20
	out 0x20,al
	RESTORE_REGS
	iret

extern _c_int_disk0
global _asm_int_disk0
_asm_int_disk0:
        SAVE_REGS
        call _c_int_disk0  ;procedimiento que atiende a las interrupciones por hardware
        mov al,0x20
	out 0x20,al
	RESTORE_REGS
	iret

extern _c_gp
global _asm_gp
_asm_gp:
        SAVE_REGS
	call _c_gp  ;procedimiento que atiende a las interrupciones por hardware
	RESTORE_REGS
        add esp,4
	iret

extern _c_page_fault
global _asm_page_fault
_asm_page_fault:
        SAVE_REGS
	call _c_page_fault
	RESTORE_REGS
        add esp,4
	iret

;cuando se produce una interrupcion, se desactivan las interrupcciones para antender la llamada al sistema y luego se habilitan
extern _c_syscall
global _asm_syscall
_asm_syscall:
        cli 
        SAVE_REGS 
        call  _c_syscall  ;procedimiento que atiende a las llamadas del sistema  
        RESTORE_REGS 
        sti
	iret

;para cambio de tarea se recuperan todos los registros del estado de la tarea anterior
global _asm_restore
_asm_restore:
	mov al,0x20
	out 0x20,al
	pop gs
	pop fs
	pop es
	pop ds
        pop edi
        pop esi
        pop ebp
        pop ebx
        pop ebx
        pop edx
        pop ecx
        pop eax
	iret

extern _c_mouse_ps2
global _asm_int_mouse_ps2
_asm_int_mouse_ps2:
        SAVE_REGS
        call _c_mouse_ps2  ;procedimiento que atiende a las interrupciones por hardware
        mov al,0x20
	out 0x20,al
	RESTORE_REGS
	iret

global _asm_interrupt_apic_spurious
_asm_interrupt_apic_spurious:
       nop
       iret

global _asm_bootCPUs ;Se carga como dato, esta seccion contiene el codigo de trampolin para inicializar los procesadores
_asm_bootCPUs:
    INCBIN "../kernel/architecture/x86/bootCPUs.o"
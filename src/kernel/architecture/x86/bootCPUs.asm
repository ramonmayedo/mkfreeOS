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

;Este seccion de codigo se compila en el offset 0x1000 para inicializar los procesadores
[BITS 16]
[ORG 0x01000]
CLI
LGDT [GDT_TAB]          ;SOLO PARA CAMBIO A MODO PROTEGIDO DE 32 BIT
MOV EAX,CR0             ;CAMBIO A MODO PROTEGIDO
OR  EAX,0x1
MOV CR0,EAX
JMP SHORT $+2           ;RECOMENDADO POR INTEL VACIA LA LISTA DE INSTRUCCIONES
JMP 0x8:MODO_PROTEGIDO  ;ESTOY EN MODO PROTEGIDO INSTRUCCIONES DE 16 BIT 

MODO_PROTEGIDO:
JMP 0x10:CODE32         ;ESTOY EN MODO PROTEGIDO INSTRUCCIONES DE 32 BIT 

GDT_TABLE:          ;DESCRIPTOR NULO INDEX 0
NULL:
LIMITE_L DW 0
BASE_L   DW 0
BASE_M   DB 0
ACCES    DB 0
LIMITE_H DB 0
BASE_H   DB 0
ECS:                ;INDEX 1 CS PARA 16 BIT
DW 0FFFFH
DW 0
DB 0
DB 9BH
DB 08FH
DB 0
CS_FLAT_32:         ;INDEX 2 CS PARA 32 BIT  
DW 0FFFFh
DW 0
DB 0
DB 9BH
DB 0DFH
DB 0
END_GDT_TABLE:
GDT_TAB:           
dw END_GDT_TABLE - GDT_TABLE - 1
dd GDT_TABLE  

;A PARTIR DE AQUI LAS INSTRUCCIONES ESTAN A 32 BITS
use32
CODE32:
LGDT [KGDT]         ;SE CAMBIA AL AL LA GDT DEL KERNEL
MOV EAX,0x10        ;INDEX DEL DESCRIPTOR 2 PARA DATOS
MOV DS,EAX
MOV ES,EAX
MOV FS,EAX
MOV GS,EAX
JMP 0x8:CHANGE     ;CAMBIO A REGISTRO CS DEL KERNEL INDEX 1 
CHANGE:
;SE ACTIVA LA PAGINCION
XOR EAX,[directoryPages]
MOV CR3,EAX
MOV EAX,CR4
OR  EAX,0x00000010
MOV CR4,EAX
MOV EAX,CR0
OR  EAX,0x80000000
MOV CR0,EAX  
;SE PREPARA LA PILA SS DEL KERNEL INDEX 2
MOV EAX,0x18
MOV SS,EAX
MOV ESP,[stackCpu]
MOV EBP,0x0
;SE PREPARA EL TSS PARA EL CAMBIO DE TAREA
MOV AX,0x38
LTR AX
;NUMERO MAGICO QUE DICE AL KERNEL QUE LA CPU FUE INICIALIZADA
MOV AX,0x0507
MOV [bootCPUmagicNumber],AX
;YA ESTAMOS EN EL KERNEL!!!
JMP [bootCpusCode]

TIMES 492 - ($-$$ ) DB 0   ;VALOR DEFINIDO ES USADO POR EL KERNEL PARA LOCALIZAR EL AREA DE DATOS ES DECIR OFFSET 500

;ESTRUCTURA DEL KERNEL PARA ACCEDER A ESTE ESPACIO DE DIRECCIONES SptrBootCpusData
KGDT:
DW 0x0
DD 0x0
stackCpu            DD 0x0
directoryPages      DD 0x0
bootCpusCode        DD 0x0
bootCPUmagicNumber  DW 0x0507
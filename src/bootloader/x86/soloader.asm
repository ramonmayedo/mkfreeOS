;Copyright (C) 2019  Ram�n Mayedo Morales (ramonmayedo@gmail.com)

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

;------------CARGADOR DEL LOADER A MEMORIA---------------------
[BITS 16]
[ORG 0X7C00]
JMP LOADER

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
EDS:                ;INDEX 2 DS PARA 16 BIT     
DW 0FFFFH
DW 0
DB 0
DB 93H
DB 08FH
DB 0
CS_FLAT_32:         ;INDEX 3 CS PARA 32 BIT  
dw 0FFFFh
dw 0
db 0
db 9BH
db 0DFH
db 0
DS_FLAT_32:         ;INDEX 4 CS PARA 32 BIT 
dw 0FFFFh
dw 0
db 0
db 93H
db 0CFH
db 0
SS_FLAT_32:         ;INDEX 5 SS PARA 32 BIT 
dw 0
dw 0
db 0
db 97H
db 0D0H
db 0
END_GDT_TABLE:
GDT_TAB:           
dw END_GDT_TABLE - GDT_TABLE - 1
dd GDT_TABLE    

LOADER:
CLI
LGDT [GDT_TAB]

XOR EAX,EAX            ;SE GUARDA LA DIRECCION DE SALTO EN offset_32
MOV AX,CS              ;PARA EL SALTO EN MODO PROTEGIDO
SHL EAX,4
ADD EAX,CPY_LOADER
MOV [offset_32],EAX

MOV EAX,CR0             ;CAMBIO A MODO PROTEGIDO
OR  EAX,0x1
MOV CR0,EAX
JMP SHORT $+2           ;RECOMENDADO POR INTEL VACIA LA LISTA DE INSTRUCCIONES
JMP 0x8:MODO_PROTEGIDO  ;ESTOY EN MODO PROTEGIDO INSTRUCCIONES DE 16 BIT 

MODO_PROTEGIDO:
MOV AX, 0x10
MOV DS,AX
MOV ES,AX
MOV FS,AX
MOV SS,AX
MOV GS,AX

db 066h     			;CAMBIAR A MODO PROTEGIDO DE 32 BIT
db 0eah
offset_32 dd 0
dw 0x18

TIMES 510 - ($-$$ ) DB 0
DB 0x55,0xAA	

;A PARTIR DE AQUI LAS INSTRUCCIONES ESTAN A 32 BITS
use32
CPY_LOADER:
MOV AX,0x20    ;CAMBIO LOS DEMAS SEGMENTOS A 32 BITSS
MOV DS,AX
MOV ES,AX
MOV FS,AX
MOV GS,AX
MOV AX,0x28
MOV SS,AX
MOV ESP,0x000FFF0
MOV EBP,0x0
PUSH EAX
POP  ECX

MOV ECX,512          ;2 sectores a copiar
MOV ESI,CODELOADER   ;ORIGEN
MOV EDI,0x1000       ;DESTINO 
REP MOVSW
JMP 0x18:0x1000

CODELOADER:
INCBIN "x86/loader.bin"

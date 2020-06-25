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

[BITS 16]
[ORG 0X7C00]
;CLI
XOR AX,AX
MOV ES,AX
MOV DS,AX
MOV SS,AX
MOV SP,0xFFF8

BOOTLOADER: 
;STI
MOV [BOOTDRIVER],DL  ;INFORMACION DE QUE UNIDAD ES LA QUE SE ARRANCO

MOV AH,0x00          ;MODO TEXTO DE LA PANTALLA
MOV AL,0x03
INT 10H

MOV DI,CARTEL       ;IMPRIME CARTEL DE INCIO
CALL WRITE

;----CHEQUEO DE LA TABLA DE PARTICIONES DE LA UNIDAD---------------
CHECKPARTITION:
MOV BX,PARTITION1    ;COMIENZO DE LA TABLA DE PARTICIONES 0x01BE ES EL BYTE 446 DEL SECTOR 0
MOV CX,0x04          ;4 ENTRADA DE LA TABLA DE PARTICIONES, 4 POSIBLES PARTICIONES
CHKPARTITIONLOOP:    
MOV AL,BYTE [BX]
TEST AL,0x80         ;ES ARRANCABLE LA PARTICION SI EL BIT 7 DEL 1ER BYTE ESTA ACTIVO
JNZ  FOUNDPARTITION
ADD BX,0x10
LOOP CHKPARTITIONLOOP
JMP ERRORPARTITION
;------------------------------------------------------------------
;------------------------------------------------------------------
FOUNDPARTITION:
;---TOMAR INFORMACION DE LA PARTICION PARA COPIAR EN MEMORIA EL SECTOR DE ARRANQUE DE LA PARTICION--------
MOV DI,FOUNDED
CALL WRITE
CALL LOADER
NOTBOOTPARTITION:
MOV DI,NOTBOOT
CALL WRITE
JMP $
;-----------------------------------------------------------------------

;-----------------------------------------------------------------------
ERRORPARTITION:
MOV DI,NOTFOUNDED
CALL WRITE
JMP $

;----IMPRIME EN PANTALLA UNA CADENA---------------------------------------
WRITE:
CICLO:
MOV AL,BYTE [DI]
CMP AL,0
JZ  FIN
INC DI
MOV AH,0X0E
INT 10H
JMP CICLO
FIN:
RET
;-VARIABLES-------------------------------------------------------
BOOTDRIVER DB 00
PARTITIONOFFSET DW 00
CARTEL DB 'LOADER PARA SISTEMA OPERATIVO MKFREE',0DH,0AH,0
FOUNDED DB 'PARTICION ENCONTRADA',0DH,0AH,0
NOTFOUNDED DB 'PARTICION NO ENCONTRADA',0DH,0AH,0
NOTBOOT DB 'PARTICION NO ARRAQNCABLE',0DH,0AH,0
;---TABLA DE PARTICIONES------- PARTICION 1 ARRANCABLE---------------
TIMES (0x01BE-($-$$)) DB 00
PARTITION1 DB 0x80 
           DB 00,00,0x4
           DB 0x57
TIMES 11 DB 0
PARTITION2 TIMES 16 DB 0
PARTITION3 TIMES 15 DB 0
PARTITION4 TIMES 16 DB 0
TIMES 510 - ($-$$ ) DB 0
DB 0x55,0xAA 


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
MOV EAX,CR0             ;CAMBIO A MODO PROTEGIDO
OR  EAX,0x1
MOV CR0,EAX
JMP SHORT $+2           ;RECOMENDADO POR INTEL VACIA LA LISTA DE INSTRUCCIONES
JMP 0x18:CPY_LOADER  ;ESTOY EN MODO PROTEGIDO INSTRUCCIONES DE 16 BIT 

;A PARTIR DE AQUI LAS INSTRUCCIONES ESTAN A 32 BITS
use32
CPY_LOADER:
nop
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

MOV ECX,2048             ;2 sectores a copiar
MOV ESI,CODELOADER      ;ORIGEN
MOV EDI,0x10000         ;DESTINO 
REP MOVSW

XOR EAX,EAX
MOV AL,byte [BOOTDRIVER]
PUSH EAX
;mov al, byte [0x10000]
;and al,0xf
;add al,0x30
;mov al,[BOOTDRIVER]
;and al,0xf0

;add al,0x30
;mov esi,0xB8000
;mov [esi],al
;jmp $
JMP 0x18:0x10000

TIMES 512 - ($-GDT_TABLE) DB 0x0

CODELOADER:             ;Cargador del Sistema Operativo Contiene Driver Para Manejar CD-ROOM
INCBIN "x86/loader.bin"
TIMES (1024+2048) - ($-CODELOADER) DB 0x0 ;1024 bytes para el Cargador
;Aqui termina el primer sector del cd de 2048 byte y a continuacion esta el codigo del Kernel
INCBIN "../Kernel/kernel.bin"
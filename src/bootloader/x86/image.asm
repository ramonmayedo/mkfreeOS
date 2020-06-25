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

INCBIN "x86/bootloader.o"
TIMES 1024 - ($-$$ ) DB 0       ;2 SECTORES SECTOR 0 Y SECTOR 1
INCBIN "x86/soloader.o"
TIMES 3072 - ($-$$ ) DB 0       ;4 SECTOR 3-5 SECTORES PARA EL CODIGO TOTAL DEL CARGADOR 
TIMES 28672 - ($-$$ ) DB 0      ;56 - 6 = 50 SECTOR 6-55 SECTORES RESERVADOS, SE PUEDEN USAR PARA EXPANDIR EL SLOADER
INCBIN "../Kernel/kernel.bin"   ;
TIMES 1572864 - ( $ -$$ ) DB 0  ;3128 - 56 = 3072 SECTOR 56 - 3072 SE RESERVAN PARA EL CODIGO DEL KERNEL MAXIMO 1.5 M Byte
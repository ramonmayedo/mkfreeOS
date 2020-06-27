/*Copyright (C) 2019  Ramón Mayedo Morales (ramonmayedo@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "../architecture/x86/x86.h"

extern Score core;
extern Smaps maps;
extern Sx86 x86;
extern u8 map_keyboard1;//mapa de caracteres reservado en memoria desde asm

Ckeyboard::Ckeyboard() {

}

void Ckeyboard::initialize() {
    maps.keyMap.scanScode = &map_keyboard1;
    state = 0;
    ptr = 0;
    count = 0;
}

void Ckeyboard::IRQ() {
    //Atencion al teclado IRQ1  
    /*Registro de estado del teclado
     * | PARE | TIMEOUT | AUXB | KEYL | C/D | SYSF | INPB | OUTB |
     *  PUERTO 0x60 LECTURA Y ESCRITURA DEL BUFER DE SALIDA
     *  PUERTO 0x64 ESCRITURA DE COMANDO Y LECTURA DE ESTADO
     *  CUANDO SE PRESIONA UNA TECLA SE ENVIA EL CODIGO AL BUFFER DEL TECLADO
     * Y CUANDO SE SUELTA SE PONE A 1 EL BITS MAS SIGNIFICATIVO CON EL MISMO CODIGO
     */
    u8 codigo;
    u8 index;
    if ((maps.keyMap.keyState & 0x1) == 1)index = 1;
    else if ((maps.keyMap.keyState & 0x2) == 1)index = 2;
    else if ((maps.keyMap.keyState & 0x4) == 1)index = 3;
    else index = 0;

    do {
        codigo = x86.port.inb(maps.keyMap.port.regControl); //0x64  Se lee el estado del teclado
    } while ((codigo & 0x01) == 0);                           //Mientras no haya algo en el buffer espero  

    codigo = x86.port.inb(maps.keyMap.port.regBase);         //leo situacion del teclado;
    codigo--;

    if (codigo < 0x80) //Se ha pulsado la tecla
    {
        switch (codigo) {
            case 0x29:
            {
                maps.keyMap.keyState |= 0x1;
                break;
            } //Left  Shift
            case 0x35:
            {
                maps.keyMap.keyState |= 0x1;
                break;
            } //Right Shift
            case 0x37:
            {
                maps.keyMap.keyState |= 0x2;
                break;
            } //Alt     
            case 0x1c:
            {
                maps.keyMap.keyState |= 0x4;
                break;
            } //Ctr
            default:
            {
                u8 car = (u8) maps.keyMap.scanScode[index + codigo * 4]; //[index][codigo]                
                maps.keyMap.keyPress = car;
                buffer[count] = car;
                if (count < 31) count++;
                if (process != 0) {
                    core.adminProcess.unlockProcess(process, thread);
                    process = 0;
                }
                break;
            }
        }
    } else { //Se ha soltado la tecla
        codigo -= 0x80;
        switch (codigo) {
            case 0x29:
            {
                maps.keyMap.keyState &= 0xfe;
                break;
            } //Left  Shift
            case 0x35:
            {
                maps.keyMap.keyState &= 0xfe;
                break;
            } //Right Shift
            case 0x37:
            {
                maps.keyMap.keyState &= 0xfd;
                break;
            } //Alt  
            case 0x1c:
            {
                maps.keyMap.keyState &= 0xfb;
                break;
            } //Ctr
            default:
            {
                break;
            }
        }
        x86.pic8259.reconocer();
    }
    return;
}

int Ckeyboard::command(int acommand, int parameter1, int parameter2) {
    Cprocess *processRun = core.adminProcess.getRun();
    switch (acommand) {
        case cmkKeyEvent:
        {
            if (state == cmdKeyboardBlock) { //si esta bloqueado el teclado!!!
                unBlock(); //se desbloquea 
            } else if (count == 0) {
                ptr = 0;
                block(); //Se bloquea el teclado
                processRun->sendState(cmdKeyboardSleep); //se suspende el proceso a espera de una tecla
                return 0;
            }
            if (ptr == count - 1) count = 0;
            return buffer[ptr++];
        }
        default: return 0;
    }
}

int Ckeyboard::getState() {
    return state;
}

int Ckeyboard::block() {
    state = 1;
    process = core.adminProcess.getRun();
    thread = process->adminThread->getRun();
}

int Ckeyboard::unBlock() {
    state = 0;
}





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
extern u8 map_keyboard1; //mapa de caracteres reservado en memoria desde asm

Ckeyboard::Ckeyboard() {

}

void Ckeyboard::initialize() {
    keyMap.scanScode = &map_keyboard1;
    state = cmdKeyboardLock;
    ptr = 0;
    count = 0;
    locksProcess.initialize(); 
    lockProcess = 0;
    while (x86.port.inb(0x64) & 0x01)
         x86.port.inb(0x60);
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
    if ((keyMap.keyState & 0x1) == 1)index = 1;
    else if ((keyMap.keyState & 0x2) == 1)index = 2;
    else if ((keyMap.keyState & 0x4) == 1)index = 3;
    else index = 0;
    codigo = x86.port.inb(KB_CONTROL_REGISTER);
    if ((codigo & 0x1) && (codigo & 0x20) == 0x0) {
        codigo = x86.port.inb(KB_BASE_REGISTER); //leo situacion del teclado;
        codigo--;

    if (codigo < 0x80) //Se ha pulsado la tecla
    {
        switch (codigo) {
            case 0x29:
            {
                keyMap.keyState |= 0x1;
                break;
            } //Left  Shift
            case 0x35:
            {
                keyMap.keyState |= 0x1;
                break;
            } //Right Shift
            case 0x37:
            {
                keyMap.keyState |= 0x2;
                break;
            } //Alt     
            case 0x1c:
            {
                keyMap.keyState |= 0x4;
                break;
            } //Ctr
            default:
            {
                u8 car = (u8) keyMap.scanScode[index + codigo * 4]; //[index][codigo]                
                keyMap.keyPress = car;
                buffer[count] = car;
                if (count < 31) count++;
                unlock(); //Se desbloquea el teclado
                break;
            }
        }
    } else { //Se ha soltado la tecla
        codigo -= 0x80;
        switch (codigo) {
            case 0x29:
            {
                keyMap.keyState &= 0xfe;
                break;
            } //Left  Shift
            case 0x35:
            {
                keyMap.keyState &= 0xfe;
                break;
            } //Right Shift
            case 0x37:
            {
                keyMap.keyState &= 0xfd;
                break;
            } //Alt  
            case 0x1c:
            {
                keyMap.keyState &= 0xfb;
                break;
            } //Ctr
            default:
            {
                break;
            }
        }
    }
   }
    return;
}

int Ckeyboard::command(int acommand, int parameter1, int parameter2) {
    switch (acommand) {
        case cmkKeyEvent:
        {
            if (state == cmdKeyboardLock) { //si esta bloqueado el teclado!!!
                confirmUnlock();
                if (state == cmdKeyboardLock)
                    lock(); //Si esta bloqueado
            }  if (count == 0) {
                ptr = 0;
                lock(); //Se bloquea el proceso
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

int Ckeyboard::lock() {
    state = cmdKeyboardLock;
    SlockProcess *lockProcess = new SlockProcess;
    lockProcess->thread = core.adminProcess.getRun();
    lockProcess->state = cmdKeyboardLock;
    locksProcess.add(lockProcess);
    lockProcess->thread->sendState(cmdKeyboardSleep, 60); //se suspende el proceso a espera de una tecla
}

int Ckeyboard::unlock() {
    while (locksProcess.count()) { //Si hay procesos bloqueados
        lockProcess = (SlockProcess*) locksProcess.removeFirst(); //Tomo el primero de la lista
        //Si se desbloqueo satisfactoriamente el proceso
        if (core.adminProcess.unlockThread(lockProcess->thread, ADP_PRIORITY_HIGHT) != -1)
            break;
        delete(lockProcess);//Se libera la memoria ya que no existe este proceso
        lockProcess = 0;    //Se indica que no hay proceso 
    }
}

int Ckeyboard::confirmUnlock() {
    if (lockProcess) {
        //Si fue liberado por el hilo que se ejecuta 
        if (lockProcess->thread == core.adminProcess.getRun()) {
            state = cmdUnlock;   //Se desbloquea el teclado
            delete(lockProcess); //Se libera la memoria
            lockProcess = 0;     //Se indica que no hay procesos
            return 0;
        }            //Si el proceso fue eliminado, muevo la cola
        else if (core.adminProcess.isThreadExist(lockProcess->thread) == -1) {
            delete(lockProcess); 
            unlock();             //Se busca otro proceso para poner en listos
        }            //Si fue eliminado el hilo, muevo la cola
        if (lockProcess == 0)
            state = cmdUnlock;
    }
    return 0;
}
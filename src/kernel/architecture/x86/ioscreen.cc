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

#include "ioscreen.h"
#include "x86.h"
#include "../../uses/conversion.h"

extern Smaps maps;
extern Score core;

CioScreen::CioScreen() { //constructor por defecto

}

void CioScreen::initialize() {
    CurX = 0;
    CurY = 0;
    ShowCursor = false;
}

void CioScreen::putC(u8 aCar) {
    core.graphics.putc(0x0, 0xFFFFFF, aCar);
}

void CioScreen::printf(char *string, ...) {
 u32 ebp;
 asm("movl %%ebp,%0" : "=m"(ebp));
 printk(string,ebp);
 asm("movl %0,%%ebp" : "=m"(ebp));
}

void CioScreen::printk(char *string,u32 aebp,...){
    if (string == 0x0) return;
    char *ptrCar = string;
    int  *ptrEBP = (int*)aebp + 4; //A partir de aqui la pila contiene los otros paramtros del printf
    char buffer[14];
    char car = *(ptrCar++);

    while (car != 0x0) {
        switch (car) {
            case '/':
            { //codigo para ejecutar salto de linea           
                car = *(ptrCar++); //proximo caracterptrCar++;
                if (car == 'n' || car == 'N') //si es /n o /N 
                {
                    putC(CAR_JMPLINE);
                    break;
                } //se envia salto de linea
                putC('/');
                break; //si no se imprime /
            };
            case '%':
            {
                car = *(ptrCar++); //proximo caracterptrCar++;
                if (car == 'i' || car == 's' || car == 'c' || car == 'h') {
                    if (car == 'i') //si es un entero %i
                    {
                        core.conversion.IntToStrChar(*ptrEBP, buffer); //lo  convierto a string
                        printf(buffer); //lo muestro
                    } else if (car == 'h') {
                        core.conversion.IntToHexChar(*ptrEBP, buffer, 8); //lo  convierto a string
                        printf(buffer); //lo muestro
                    } else if (car == 's') //si es un string %s
                        printf((char*) *ptrEBP);
                    else if (car == 'c') //si es un char
                        putC((char) *ptrEBP);

                    ptrEBP += 0x1; //preparo puntero a proxima varible
                    break;
                }

                putC('%');
                break; //si no se imprime /
            };

            default:
            {
                putC(car);
                break;
            } //si es un car se imprime  
        };
        car = *(ptrCar++);
    }
}

void CioScreen::clearScreen() {
    core.graphics.clearScreen();
    CurX=0;
    CurY =0;
}

void CioScreen::setX(u32 aCurX) {
    CurX = aCurX;
}

void CioScreen::setY(u32 aCurY) {
    CurY = aCurY;
}

u32 CioScreen::getX() {
    return CurX;
}

u32 CioScreen::getY() {
    return CurY;
}

void CioScreen::showCursor(){
     ShowCursor = true;
     
}

void CioScreen::setTextColor(int newColor){
    CarAttrib = newColor;
}

int CioScreen::block() {
    state = 1;
    pidBlock =core.adminProcess.getRun()->process->getpid();
}

int CioScreen::unblock() {
    state = 0;
}

int CioScreen::getState() {
    return state; 
}

int CioScreen::getpidBlock() {
    return pidBlock;
}

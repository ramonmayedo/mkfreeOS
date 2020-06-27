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

#ifndef IOSCREEN_H
#define IOSCREEN_H
#include "../../defines/types.h"

#define CAR_JMPLINE       0x0A
#define CAR_RETCURSOR     0x0D
#define CAR_ATTRIBUTE     0x07
#define CAR_BACKSPACE     0x08

class CioScreen {    
private:   
    u8 CarAttrib;
    bool ShowCursor;
    bool modeText;
    u32 state;
    int  pidBlock;
    char charRead;
public:
   u32 CurX;
   u32 CurY;
   CioScreen();
   void initialize();
   void putC(u8 aCar);
   void setX(u32 aCurX);
   void setY(u32 aCurY);
   u32  getX();
   u32  getY();
   void showCursor();
   void printf(char *string,...);
   void printk(char *string,u32 aebp,...);
   void clearScreen(); 
   void restoreScreen(char *abuffer);
   void saveScreen(char *abuffer);
   void setTextColor(int newColor);
   int  block();
   int  unblock();
   int  getState();
   int  getpidBlock();
   void setRamScreen(u32 dirRamScreen);
protected:
   

};
  
#endif


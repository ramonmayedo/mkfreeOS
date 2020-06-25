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

#ifndef HBMP_H
#define HBMP_H
#include "../../defines/types.h"
struct SfileHeaderBMP{
    u8  fType[2];      //Contiene BM para verificar el tipo de fichero
    u32 fSize;         //Tamaño del archivo en byte
    u8  fReservado[4]; //Reservado
    u32 fOffset;       //Offset de los datos donde comienza la imagen
}__attribute__ ((packed));

struct SinfoHeaderBMP{
    u32 iHeaderSize;   //Tamaño de la cabecera
    u32 iWidth;        //Ancho de la Imagen
    u32 iHeight;       //Largo de la Imagen
    u16 iPlanes;       //Planos de Color 1 por defecto
    u16 iBitXPixel;
    u32 iCompress;
    u32 iImageSize;    //Tamaño de la Imagen
    u32 iXBitXm;
    u32 iYBitXm;
    u32 iColors;
    u32 iImpColors;
}__attribute__ ((packed));

struct SfileBMP{
    SfileHeaderBMP fileHeader;
    SinfoHeaderBMP infoHeader;
}__attribute__ ((packed));
#endif


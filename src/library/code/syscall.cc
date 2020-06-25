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

#include "../includes/syscall.h"

u32 sysCall_1(u32 anumber, u32 parameter1) {
    u32 ret;
    asm("push %ecx");
    asm("push %edx");
    asm("push %ebx");
    asm("push %esi");
    asm("push %edi"); 
    asm("movl %0,%%eax" : "=m"(anumber));
    asm("movl %0,%%ecx" : "=m"(parameter1));
    asm("int $0x80");
    asm("pop %edi");
    asm("pop %esi");
    asm("pop %ebx");
    asm("pop %edx");
    asm("pop %ecx"); 
    asm("movl %%eax,%0" : "=m"(ret));
    return ret;
}

u32 sysCall_2(u32 anumber, u32 parameter1, u32 parameter2) {
    u32 ret;
    asm("push %ecx");
    asm("push %edx");
    asm("push %ebx");
    asm("push %esi");
    asm("push %edi");
    asm("movl %0,%%eax" : "=m"(anumber));
    asm("movl %0,%%ecx" : "=m"(parameter1));
    asm("movl %0,%%edx" : "=m"(parameter2));
    asm("int $0x80");
    asm("pop %edi");
    asm("pop %esi");
    asm("pop %ebx");
    asm("pop %edx");
    asm("pop %ecx"); 
    asm("movl %%eax,%0" : "=m"(ret));
    return ret;
}

u32 sysCall_3(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3) {
    u32 ret;
    asm("push %ecx");
    asm("push %edx");
    asm("push %ebx");
    asm("push %esi");
    asm("push %edi");
    asm("movl %0,%%eax" : "=m"(anumber));
    asm("movl %0,%%ecx" : "=m"(parameter1));
    asm("movl %0,%%edx" : "=m"(parameter2));
    asm("movl %0,%%ebx" : "=m"(parameter3));
    asm("int $0x80");
    asm("pop %edi");
    asm("pop %esi");
    asm("pop %ebx");
    asm("pop %edx");
    asm("pop %ecx"); 
    asm("movl %%eax,%0" : "=m"(ret));
    return ret;
}

u32 sysCall_4(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3, u32 parameter4) {
    u32 ret;
    asm("push %ecx");
    asm("push %edx");
    asm("push %ebx");
    asm("push %esi");
    asm("push %edi");  
    asm("movl %0,%%eax" : "=m"(anumber));
    asm("movl %0,%%ecx" : "=m"(parameter1));
    asm("movl %0,%%edx" : "=m"(parameter2));
    asm("movl %0,%%ebx" : "=m"(parameter3));
    asm("movl %0,%%esi" : "=m"(parameter4));
    asm("int $0x80");
    asm("pop %edi");
    asm("pop %esi");
    asm("pop %ebx");
    asm("pop %edx");
    asm("pop %ecx"); 
    asm("movl %%eax,%0" : "=m"(ret));
    return ret;
}

u32 sysCall_5(u32 anumber, u32 parameter1, u32 parameter2, u32 parameter3, u32 parameter4, u32 parameter5) {
    u32 ret;
    asm("push %ecx");
    asm("push %edx");
    asm("push %ebx");
    asm("push %esi");
    asm("push %edi"); 
    asm("movl %0,%%eax" : "=m"(anumber));
    asm("movl %0,%%ecx" : "=m"(parameter1));
    asm("movl %0,%%edx" : "=m"(parameter2));     
    asm("movl %0,%%ebx" : "=m"(parameter3));
    asm("movl %0,%%esi" : "=m"(parameter4));  
    asm("movl %0,%%edi" : "=m"(parameter5));
    asm("int $0x80");
    asm("pop %edi");
    asm("pop %esi");
    asm("pop %ebx");
    asm("pop %edx");
    asm("pop %ecx"); 
    asm("movl %%eax,%0" : "=m"(ret));
    return ret;
}
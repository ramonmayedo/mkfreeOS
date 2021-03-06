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

#include "io.h"

Cio::Cio() {
} 
void Cio::outb(u32 addr,u8 value){
  asm volatile("movb %0,%%al" :"=m"(value));  
  asm volatile("mov %0,%%dx" :"=m"(addr));  
  asm volatile("outb %al, %dx");  
}
void Cio::outw(u32 addr,u16 value){
  asm volatile("movw %0,%%ax" :"=m"(value));  
  asm volatile("mov %0,%%dx" :"=m"(addr));  
  asm volatile("outw %ax, %dx");  
}
void Cio::outl(u32 addr,u32 value){
  asm volatile("movl %0,%%eax" :"=m"(value));  
  asm volatile("mov %0,%%dx" :"=m"(addr));  
  asm volatile("outl %eax, %dx");    
}
u8 Cio::inb(u32 addr){
  u8 value; 
  asm volatile("mov %0,%%dx" :"=m"(addr));   
  asm volatile("inb %dx,%al");
  asm volatile("movb %%al,%0" :"=m"(value)); 
  return value;
}
u16 Cio::inw(u32 addr){
  u16 value; 
  asm volatile("mov %0,%%dx" :"=m"(addr));   
  asm volatile("inw %dx, %ax");
  asm volatile("movw %%ax,%0" :"=m"(value)); 
  return value;   
}
u32 Cio::inl(u32 addr){
  u32 value; 
  asm volatile("mov %0,%%dx" :"=m"(addr));   
  asm volatile("inl %dx,%eax");
  asm volatile("movl %%eax,%0" :"=m"(value)); 
  return value;     
}

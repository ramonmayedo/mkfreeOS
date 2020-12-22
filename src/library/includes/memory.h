#ifndef MEMORY_H
#define MEMORY_H

#define u32 unsigned int

void* memcpy(void* des, const void* src, u32 size);
void *memset(char *dest, char val, int n);
int memcmp(const void* s1, const void* s2, u32 size);

#endif


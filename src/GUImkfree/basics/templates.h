#ifndef TEMPLATES_H
#define TEMPLATES_H

#define u32 unsigned int

//Plantilla para obtener el maximo
template <typename M>
inline M max(const M&a, const M&b) {
    return a > b ? a : b;
}
//Plantilla para el minimo
template <typename M>
inline M min(const M&a, const M&b) {
    return a < b ? a : b;
}

inline void fastCopy(u32* dest, const u32* src, u32 count) {
    asm volatile(
                "rep movsl\n"
                : "=S"(src), "=D"(dest), "=c"(count)
                : "S"(src), "D"(dest), "c"(count)
                : "memory");
}

inline void fastFill32(u32* dest, u32 value, u32 count) {
    asm volatile(
                "rep stosl\n"
                : "=D"(dest), "=c"(count)
                : "D"(dest), "c"(count), "a"(value)
                : "memory");
}


#endif


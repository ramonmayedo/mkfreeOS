use32

global _setjmp
_setjmp:
    mov eax,[esp+4]
    mov [eax + 0], ebp
    mov [eax + 4], ebx
    mov [eax + 8], edi
    mov [eax + 12],esi
    lea ecx,[esp+4]
    mov [eax + 16],ecx
    lea ecx,[esp]
    mov [eax + 20],ecx
    xor eax,eax
    ret

global _longjmp
_longjmp:
    mov edx,[esp+4]
    mov eax,[esp+8]
    mov ebp,[edx]
    mov ebx,[edx+4]
    mov edi,[edx+8]
    mov esi,[edx+12]
    mov ecx,[edx+16]
    mov esp,ecx
    mov ecx,[edx+20]
    test eax,eax
    jnz jmpzero 
    mov eax,1
jmpzero:
    jmp [ecx]
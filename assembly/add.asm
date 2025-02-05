section .data
    var1 dw 1              ; 16-bit value (WORD)
    var2 dw 2              ; 16-bit value (WORD)
    res  dd 0              ; 32-bit result (DWORD)

section .text
    global main

main:
    mov ax, var1
    add ax, var2
    mov [res], ax
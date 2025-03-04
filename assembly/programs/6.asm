TITLE Looping Constructs - FOR and WHILE loops.

.386
.model flat, stdcall
.stack 4096

include kernel32.inc
includelib kernel32.lib
includelib msvcrt.lib
option casemap:none

extern printf:proc
extern scanf:proc

.data
format db "%d",0
.code
main PROC
    mov ECX, 10
    mov EAX, 1
    while_loop:
        inc EAX
        push EAX
        push OFFSET format
        call printf
        LOOP while_loop
    
main endp
end main
TITLE Comparisons and Conditional Jumps - IF-ELSE logic.

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
    prompt db "Enter a number between 1 and 100:", 0
    format db "%d", 0
    number dd ?
    greater db "The number is greater than 50.", 0
    lesser db "The number is less than 50.", 0

.code
main PROC
    
    push OFFSET prompt
    call printf

    push OFFSET number
    push OFFSET format
    call scanf

 
    
main ENDP
end main
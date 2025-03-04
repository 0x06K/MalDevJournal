TITLE Input from User - Read user input and print it.

.386
.model flat, stdcall
.stack 4096
option casemap :none


include kernel32.inc

includelib kernel32.lib
includelib msvcrt.lib

extern printf:proc
extern scanf:proc

.data
    format db "%d", 0   ; Format string for scanf (integer)
    number dd ?         ; Variable to store input

.code
main PROC
    PUSH OFFSET number  ; Address where input will be stored
    PUSH OFFSET format  ; Format specifier
    CALL scanf          ; Read input from user

    ; Print the input value (Optional)
    PUSH number         ; Pass stored input to printf
    PUSH OFFSET format
    CALL printf

    ; Exit program
    PUSH 0
    CALL ExitProcess
main ENDP
END main
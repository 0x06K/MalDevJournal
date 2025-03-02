; Print "Hello, World!" using Windows API (WriteConsoleA)
.386
.model flat, stdcall
option casemap :none

include windows.inc
include kernel32.inc
includelib kernel32.lib

.data
    msg db "Hello, World!", 0
    bytesWritten dd 0

.code
start:
    ; Get handle to console output
    push -11
    call GetStdHandle  ; Returns handle in EAX

    ; Call WriteConsoleA
    push 0            ; Reserved
    push offset bytesWritten ; Store bytes written
    push 13           ; String length
    push offset msg   ; String to print
    push eax          ; Handle from GetStdHandle
    call WriteConsoleA

    ; Exit program
    push 0
    call ExitProcess

end start
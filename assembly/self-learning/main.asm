.386
.model flat, stdcall
option casemap:none

include windows.inc
include kernel32.inc
includelib kernel32.lib

.data
    buffer db 20 dup(0)  ; Buffer for user input
    bytesRead dd 0

.code
main proc
    push -10  ; STD_INPUT_HANDLE
    call GetStdHandle
    push 0
    push offset bytesRead
    push 20
    push offset buffer
    push eax
    call ReadConsoleA
    ret
main endp
end main

.386
.model flat, stdcall
.stack 4096

include kernel32.inc
includelib kernel32.lib

.code
main PROC
    mov EAX, 5          ; Load first operand
    add EAX, 10         ; Perform addition (5 + 10)
    sub EAX, 2
    invoke ExitProcess, 0  ; Exit program cleanly
main ENDP
END main

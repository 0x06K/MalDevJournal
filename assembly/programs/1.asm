TITLE Hello, World! - Basic output.
.386
.model flat, stdcall
option casemap :none


include kernel32.inc
includelib kernel32.lib
includelib msvcrt.lib

extern printf:proc

.data
    msg db "Hello, World!", 0


.code
start:
    ; Display message to console
    
    push OFFSET msg
    call printf    
    
    ; Exit program
    push 0
    call ExitProcess

end start
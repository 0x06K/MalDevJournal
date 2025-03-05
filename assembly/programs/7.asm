.386
.model flat, stdcall
.stack 4096
.data
    str1 db "HELLO", 0  ; Original string
    len equ $ - str1    ; Length of str1
    revStr db len dup(0) ; Buffer for reversed string

.code
main PROC
    mov esi, OFFSET str1     ; Source pointer
    mov edi, OFFSET revStr   ; Destination pointer
    add esi, len - 1         ; Move to last character
    mov ecx, len             ; Loop counter

reverse_loop:
    mov al, [esi]            ; Load character
    mov [edi], al            ; Store in reverse buffer
    dec esi                  ; Move backward
    inc edi                  ; Move forward
    loop reverse_loop        ; Repeat for all characters

    ret
main ENDP
END main

.386
.model flat, stdcall
.stack 4096
include windows.inc
include kernel32.inc
include msvcrt.inc
includelib kernel32.lib
includelib msvcrt.lib

.data
    num dd ?                 ; Input number
    result dd ?              ; Store factorial result
    fmt_input db "Enter number: ", 0
    fmt_output db "Factorial: %d", 10, 0
    fmt_scan db "%d", 0

.code
extern printf:proc, scanf:proc

factorial proc
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]  ; Get input (n)
    cmp eax, 1
    jg recurse
    mov eax, 1        ; Return 1 if n <= 1
    jmp end_rec

recurse:
    dec eax
    push eax
    call factorial    ; Recursive call factorial(n-1)
    add esp, 4        ; Clean stack

    mov ecx, [ebp+8]  ; Load n
    mul ecx           ; Multiply n * factorial(n-1)

end_rec:
    pop ebp
    ret 4             ; Cleanup argument
factorial endp

main proc
    ; Print prompt
    push offset fmt_input
    call printf
    add esp, 4

    ; Read input
    push offset num
    push offset fmt_scan
    call scanf
    add esp, 8

    ; Call factorial function
    push num
    call factorial
    mov result, eax

    ; Print result
    push result
    push offset fmt_output
    call printf
    add esp, 8

    ; Exit
    push 0
    call ExitProcess
main endp
end main

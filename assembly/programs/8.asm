.386
.model flat, stdcall
.stack 4096
include windows.inc
include kernel32.inc
include msvcrt.inc
includelib kernel32.lib
includelib msvcrt.lib

.data
    arr db 10 dup(?)            ; Array (max 10 elements)
    len dd ?                    ; Array length
    search_val db ?              ; Value to search
    sum_val dd 0                 ; Sum of elements
    format_input db "%d", 0      ; Format for scanf
    format_output db "Sum: %d", 10, 0
    format_sorted db "Sorted: ", 0
    format_number db "%d ", 0
    format_search db "Search %d: %s", 10, 0
    found_msg db "Found", 0
    not_found_msg db "Not Found", 0

.code
extern scanf:proc, printf:proc
main proc
    ; Get array size
    push offset len
    push offset format_input
    call scanf
    add esp, 8   ; Clean up stack

    ; Read array elements
    mov ecx, 0
input_loop:
    cmp ecx, len
    jge sum_calc

    lea eax, arr
    add eax, ecx
    push eax
    push offset format_input
    call scanf
    add esp, 8   ; Clean up stack

    inc ecx
    jmp input_loop

sum_calc:
    ; Compute sum
    xor ecx, ecx
    xor eax, eax
sum_loop:
    cmp ecx, len
    jge print_sum

    add al, [arr + ecx]
    inc ecx
    jmp sum_loop

print_sum:
    mov sum_val, eax
    push sum_val
    push offset format_output
    call printf
    add esp, 8   ; Clean up stack

bubble_sort:
    mov ecx, len
    dec ecx
outer_loop:
    cmp ecx, 0
    jle print_sorted

    mov ebx, 0
inner_loop:
    cmp ebx, ecx
    jge next_pass

    mov al, [arr + ebx]
    mov ah, [arr + ebx + 1]
    cmp al, ah
    jbe no_swap

    mov [arr + ebx], ah
    mov [arr + ebx + 1], al

no_swap:
    inc ebx
    jmp inner_loop

next_pass:
    dec ecx
    jmp outer_loop

print_sorted:
    push offset format_sorted
    call printf
    add esp, 4

    mov ecx, 0
print_loop:
    cmp ecx, len
    jge search_input

    push dword [arr + ecx]
    push offset format_number
    call printf
    add esp, 8

    inc ecx
    jmp print_loop

search_input:
    ; Get search value
    push offset search_val
    push offset format_input
    call scanf
    add esp, 8

    ; Search for element
    xor ecx, 0
    mov edx, offset not_found_msg
search_loop:
    cmp ecx, len
    jge print_search

    mov al, [arr + ecx]
    cmp al, search_val
    je found

    inc ecx
    jmp search_loop

found:
    mov edx, offset found_msg

print_search:
    push edx
    push search_val
    push offset format_search
    call printf
    add esp, 12

    ; Exit
    push 0
    call ExitProcess

main endp
end main

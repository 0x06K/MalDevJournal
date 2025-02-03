TITLE Add and Subtract
; Thi program adds and subtracts 32-bit integers

INCLUDE Irvine32.inc

.code

main PROC

    mov eax, 1000h
    add eax, 4000h
    sub eax, 2000h
    call DumpRegs
    exit
    
main ENDP
END main
    

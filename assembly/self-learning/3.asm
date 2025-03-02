.386
.model flat, stdcall
.stack 4096

.code
start:
    mov EAX, 6
    mul EAX
    mov EBX, 6
    div EBX

end start
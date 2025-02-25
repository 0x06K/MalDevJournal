MOV AL, [num1]
MOV BL, [num2]
MUL BL
MOV [result], AL
HLT
num1 DB 2
num2 DB 3
result DB ?
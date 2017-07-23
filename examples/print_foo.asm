; draw 'F00' to the screen

f_char ; the character 'F'

LB $F0
LB $80
LB $F0
LB $80
LB $80

o_char ; the character '0'

LB $F0
LB $90
LB $90
LB $90
LB $F0

start ; the beginning of the actual program

; draw the 'F'
ILOAD f_char ; load the 'F' into index
LOAD R0, $A ; load 10 into register 0
LOAD R1, $5 ; load 5 into register 1
DRAW R0, R1, $5 ; draw a 5 byte character

; draw the '0'
ILOAD o_char ; load the '0' into index
ADD R0, $10 ; add 10 into register 0
DRAW R0, R1, $5 ; draw a 5 byte character

; draw another '0'
ADD R0, $10
DRAW R0, R1, $5 ; draw a 5 byte character



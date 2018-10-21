; draw the string 'F00' to the screen
; (C) Tamer Aly, 2018

; the beginning of the actual program
start
    ; draw the 'F'
    ILOAD f_char ; load the 'F' into index
    LOAD R0, $A ; load 10 into register 0
    LOAD R1, $5 ; load 5 into register 1
    DRAW R0, R1, $5 ; draw a 5 byte character

    ; draw the '0'
    ILOAD o_char
    ADD R0, $A ; move 10 pixels to the right
    DRAW R0, R1, $5

    ; draw another '0'
    ADD R0, $A ; move 10 pixels to the right
    DRAW R0, R1, $5

end
    JMP end ; loop indefinitely

; the character buffer for 'F'
f_char
    LB $F0
    LB $80
    LB $F0
    LB $80
    LB $80

; the character buffer for '0' (poor man's zero)
o_char
    LB $F0
    LB $90
    LB $90
    LB $90
    LB $F0
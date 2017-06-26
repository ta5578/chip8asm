; This is a comment in the assembly file
; This will print the capital letter 'A' to the screen

start
    LOADI sprite ; load the sprite location into index
    LOAD r0,$A ; load 10 into register 0
    LOAD r1,$5 ; load 5 into register 1
    DRAW r0,r1,$5 ; draw a 5 byte sprite at (x,y) specified in r0, r1
    
end:
    JMP end ; loop indefinitely

; The capital letter 'A'
sprite 
    LB $F0
    LB $90
    LB $F0
    LB $90
    LB $90

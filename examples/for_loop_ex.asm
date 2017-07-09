; This is an example that simulates a for loop

clear    CLR ; clear the screen
start    LOAD    r1,$0 ; Clear contents of register 1
         ADD     r1,$1 ; Add 1 to the register
         SKE     r1,$A ; Check to see if we are at 10
         JMP    start ; Jump back to the start
end      JMP    end   ;    Loop forever

data     LB     $1A   ; One byte piece of data
data1    LB     $FBEE ; Two byte piece of data

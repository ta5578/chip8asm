# Chip8 VM Assembler &copy; Tamer Aly 2017

This is an assembler for the CHIP8 VM. It supports all 35 opcodes that the VM
supports.

## Supported Op Codes
[Wikipedia](https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description)

| Name | Opcode | Description |
| -----|--------|------------ |
|`SYS` | `0NNN` | System call. Not used |
|`CLR` | `00E0` | Clears the screen |
|`RET` | `00EE` | Returns from a subroutine |
|`JMP` | `1NNN` | Go to address NNN |
|`CALL`| `2NNN` | Calls routine at NNN |
|`SKE` | `3XNN` | Skips the next instruction if register X equals NN |
|`SKNE`| `4XNN` | Skips the next instruction if register X does NOT equal NN |
|`SKRE`| `5XY0` | Skips the next instruction if register X equals register Y |
|`LOAD`| `6XNN` | Sets register X to NN |
|`ADD` | `7XNN` | Adds NN to register X |
|`ASN` | `8XY0` | Assigns the value in register X to register Y |
|`OR`  | `8XY1` | Sets register X to register X \| register Y |
|`AND` | `8XY2` | Sets register X to register X & register Y |
|`XOR` | `8XY3` | Sets register X to register X ^ register Y |
|`ADDR`| `8XY4` | Adds register Y to register X. Register F set on carry |
|`SUB` | `8XY5` | Subtracts register Y from register X. Register F set borrow |
|`SHR` | `8XY6` | Register F set to LSB of register X. X is shifted right 1 |
|`SUBR`| `8XY7` | Register X is set to register Y - register X |
|`SHL` | `8XYE` | Register F set to MSB of register X. X is shifted left 1 |
|`SKRNE`| `9XY0` | Skips the next instruction if register X != register Y |
|`LOADI`| `ANNN` | Sets register I to value of NNN |
|`JMP0`| `BNNN` | Jumps to the address NNN plus the value in register 0 |
|`RAND`| `CXNN` | Sets register X to result of NN & random number |
|`DRAW`| `DXYN` | Draws a sprite at coord (X,Y) |
|`SKK`| `EX9E` | Skips the next instruction if the key stored in X is pressed |
|`SKNK`| `EXA1` | Skips the next instruction if key in X is not pressed |
|`DELA`| `FX07` | Sets register X to the value of the delay timer |
|`KEYW`| `FX0A` | A key press is awaited then stored in register X (BLOCKING)|
|`DELR`| `FX15` | Sets the delay timer to value of register X |
|`SNDR`| `FX18` | Sets the sound timer to value of register Y |
|`ADDI`| `FX1E` | Adds register X to register I |
|`SILS`| `FX29` | Sets I to the location of the sprite of character in X |
|`BCD` | `FC33` | Stores binary-coded decimal representation of register X |
|`DUMP`| `FX55` | Dumps the values of register 0 - X in memory starting at I
|`DUMPR`| `FX65` | Fills register 0 - X with values from memory starting at I | 

Although these aren't supported by the official VM documentation, these are
added for convenience by the assembler. Note the lack of an opcode.

| Name | Description |
| ---- | ----------- |
| LB   | Loads a hexadecimal byte value into memory. |

## Operands
Operands may be one of three different types:

| Type | Example | Description |
| ---- | ------- | ----------- |
| Register | `r4`| Registers are in the range 0 - F and start with `r` or `R`. |
| Hex Value | `$123` | Specifies a hex value. _Must_ begin with `$`. |
| Label | `label:` | Labels may be a string that _must_ end in a `:`. |

## Example
```
; This is a comment in the assembly file
; This will print the capital letter 'A' to the screen

start:
    LOADI sprite ; load the sprite location into index
    LOAD r0,$A ; load 10 into register 0
    LOAD r1,$5 ; load 5 into register 1
    DRAW r0,r1,$5 ; draw a 5 byte sprite at (x,y) specified in r0, r1
    
end:
    JMP end ; loop indefinitely

; The capital letter 'A'
sprite: 
    LB $F0
    LB $90
    LB $F0
    LB $90
    LB $90
```

## Referenced Libraries
* [CuTest C Unit Testing](http://cutest.sourceforge.net/)

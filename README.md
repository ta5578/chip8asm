# Chip8 VM Assembler &copy; Tamer Aly 2017

This is an assembler for the [chip8 VM](https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description). As input, it takes an input `.asm` assembly file and converts it into a `ROM` file that a chip8 VM can run.

## Building and Testing
The assembler source and tests are built using `cmake`:

```
mkdir build
cd build
cmake ..
```

By default, CMake will generate *release* versions of the build. To enable debug builds, build with CMAKE_BUILD_TYPE set to "debug". Note that this will only affect CMake single generators like _Make_ and _Ninja_. Other generators like the _Visual Studio_ generator allow you to customize the configuration type once the generator itself is built.

```
cmake .. -DCMAKE_BUILD_TYPE=debug
```

## Running the Assembler
As mentioned, the assembler takes a `.asm` assembly file (see below for
an example) as input and assembles it into a `ROM` file that a chip8 VM can emulate.
Additional options are supported by the assembler like dumping the assembled
statements with memory locations to stdout as well as dumping the list of
supported op codes. For a full list of options, run the assembler with the `-h`
flag.

Here is an example showing how to generate a Chip8 ROM called `print-foo.c8` from the `print-foo.asm` assembly
file under `/examples` and dump the opcodes:

```
./chip8asm print_foo.asm -o print-foo.c8 --dump-asm
```

See below for the assembly dump. The format of the dump is `<address> | <opcode> ; <actual statement>`.
```
-------- ASM Dump --------
0x0200 | 0x14A2 ; ILOAD 0x0214
0x0202 | 0x0A60 ; LOAD R0, $A
0x0204 | 0x0561 ; LOAD R1, $5
0x0206 | 0x15D0 ; DRAW R0, R1, $5
0x0208 | 0x19A2 ; ILOAD 0x0219
0x020A | 0x0A70 ; ADD R0, $A
0x020C | 0x15D0 ; DRAW R0, R1, $5
0x020E | 0x0A70 ; ADD R0, $A
0x0210 | 0x15D0 ; DRAW R0, R1, $5
0x0212 | 0x1212 ; JMP 0x0212
0x0214 | 0xF0 ; LB $F0
0x0215 | 0x80 ; LB $80
0x0216 | 0xF0 ; LB $F0
0x0217 | 0x80 ; LB $80
0x0218 | 0x80 ; LB $80
0x0219 | 0xF0 ; LB $F0
0x021A | 0x90 ; LB $90
0x021B | 0x90 ; LB $90
0x021C | 0x90 ; LB $90
0x021D | 0xF0 ; LB $F0
-------- End Dump --------
```

Here is the output `print-foo.c8` file when viewed on a little-endian machine using
`hexdump`:

```
00000000  a2 14 60 0a 61 05 d0 15  a2 19 70 0a d0 15 70 0a  |..`.a.....p...p.|
00000010  d0 15 12 12 f0 80 f0 80  80 f0 90 90 90 f0        |..............|
0000001e
```

Finally, here's what the ROM would do when it is run on a [Chip8 emulator](https://github.com/tamerfrombk/chip8emu):

![](/rsc/foo.png?raw=true "Print FOO")

## Supported Op Codes
The Chip8 does not have official mnemonics for all the opcodes it supports; this means that I've had to make them
up for this assembler. Luckily for you, I'm a reasonable guy and the mnemonic closely matches its functionality :)

| Name | Opcode | Count | Description |
| -----|--------|-------|----------------------------------- |
|`SYS` | `0NNN` | 1 | System call. Not used |
|`CLR` | `00E0` | 0 | Clears the screen |
|`RET` | `00EE` | 0 | Returns from a subroutine |
|`JMP` | `1NNN` | 1 | Go to address NNN |
|`CALL`| `2NNN` | 1 | Calls routine at NNN |
|`SKE` | `3XNN` | 2 | Skips the next instruction if register X equals NN |
|`SKNE`| `4XNN` | 2 | Skips the next instruction if register X does NOT equal NN |
|`SKRE`| `5XY0` | 2 | Skips the next instruction if register X equals register Y |
|`LOAD`| `6XNN` | 2 | Sets register X to NN |
|`ADD` | `7XNN` | 2 | Adds NN to register X |
|`ASN` | `8XY0` | 2 | Assigns the value in register X to register Y |
|`OR`  | `8XY1` | 2 | Sets register X to register X \| register Y |
|`AND` | `8XY2` | 2 | Sets register X to register X & register Y |
|`XOR` | `8XY3` | 2 | Sets register X to register X ^ register Y |
|`RADD`| `8XY4` | 2 | Adds register Y to register X. Register F set on carry |
|`SUB` | `8XY5` | 2 | Subtracts register Y from register X. Register F set borrow |
|`SHR` | `8X06` | 1 | Register F set to LSB of register X. X is shifted right 1 |
|`RSUB`| `8XY7` | 2 | Register X is set to register Y - register X |
|`SHL` | `8X0E` | 1 | Register F set to MSB of register X. X is shifted left 1 |
|`SKRNE`| `9XY0` | 2 | Skips the next instruction if register X != register Y |
|`ILOAD`| `ANNN` | 1 | Sets register I to value of NNN |
|`ZJMP`| `BNNN` | 1 | Jumps to the address NNN plus the value in register 0 |
|`RAND`| `CXNN` | 2 | Sets register X to result of NN & random number |
|`DRAW`| `DXYN` | 3 | Draws a sprite at coord (X,Y) |
|`SKK`| `EX9E` | 1 | Skips the next instruction if the key stored in X is pressed |
|`SKNK`| `EXA1` | 1 | Skips the next instruction if key in X is not pressed |
|`DELA`| `FX07` | 1 | Sets register X to the value of the delay timer |
|`KEYW`| `FX0A` | 1 | A key press is awaited then stored in register X (BLOCKING)|
|`DELR`| `FX15` | 1 | Sets the delay timer to value of register X |
|`SNDR`| `FX18` | 1 | Sets the sound timer to value of register X |
|`IADD`| `FX1E` | 1 | Adds register X to register I |
|`SILS`| `FX29` | 1 | Sets I to the location of the sprite of character in X |
|`BCD` | `FX33` | 1 | Stores binary-coded decimal representation of register X |
|`DUMP`| `FX55` | 1 | Dumps the values of register 0 - X in memory starting at I |
|`IDUMP`| `FX65` | 1 | Fills register 0 - X with values from memory starting at I |

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
| Label | `label` | Labels are simply strings used to denote a specific block of code. |

## Example
```
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
```

See the `/examples` directory for more examples.

## Credits
* [CMake](https://cmake.org/)
* [CATCH](https://github.com/catchorg/Catch2)

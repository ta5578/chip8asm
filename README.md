# Chip8 VM Assembler &copy; Tamer Aly 2017

This is an assembler for the [chip8 VM](https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description). As input, it takes an input `.asm` assembly file and converts it into a `ROM` file that a chip8 VM can run.

## Building and Testing
The assembler source and tests are built using `cmake`. Unit testing is done via `UnitTest++`. To build the source the proper 
way (in an out-of-source build), create a directory and run cmake.

```
mkdir build
cd build
cmake ..
```
This will build the sources into the build directory and run unit tests as part of the build. 
Note that you must have [UnitTest++ installed](https://github.com/unittest-cpp/unittest-cpp/wiki/Building-Using-CMake) in order for the unit tests to build and run. 
If `UnitTest++` is not installed, unit tests will be skipped over and only the sources will be built.
Tests can also be run ad-hoc by typing `make test` after building the project.

## Running the Assembler
As mentioned, the assembler takes a `.asm` assembly file (see below for
an example) as input and assembles it into a `ROM` file that a chip8 VM can emulate.
Additional options are supported by the assembler like dumping the assembled
statements with memory locations to stdout as well as dumping the list of
supported op codes. For a full list of options, run the assembler with the `-h`
flag. Here is an example that will run the assembler by processing `myasm.asm`
and spitting out a ROM file `myrom.rom`.

```
./chip8asm myasm.asm -o myrom.rom
```
Here is an example of dumping the assembly on the `for_loop_ex.asm` file found
under `/examples`.

```
./chip8asm ../examples/for_loop_ex.asm --dump-asm

Reading from '../examples/for_loop_ex.asm' and writing to 'a.rom'.
Dump ASM: true
Verbose: false
-------- Asm Dump --------
0x200 -- 0xE0 ; CLR
0x210 -- 0x6100 ; LOAD r1, $0
0x220 -- 0x7101 ; ADD r1, $1
0x230 -- 0x310A ; SKE r1, $A
0x240 -- 0x1A00 ; JMP start
0x250 -- 0x1E0D ; JMP end
0x260 -- 0x1A ; LB $1A
0x270 -- 0xFBEE ; LB $FBEE
-------- End Dump --------
Binary ROM successfully generated!
```

## Supported Op Codes

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
; This is a comment in the assembly file
; This will print the capital letter 'A' to the screen

start
    ILOAD sprite ; load the sprite location into index
    LOAD r0,$A ; load 10 into register 0
    LOAD r1,$5 ; load 5 into register 1
    DRAW r0,r1,$5 ; draw a 5 byte sprite at (x,y) specified in r0, r1

end
    JMP end ; loop indefinitely

; The capital letter 'A'
sprite
    LB $F0
    LB $90
    LB $F0
    LB $90
    LB $90
```

See the `/examples` directory for more examples.

## Libraries and Tools
* [CMake](https://cmake.org/)
* [UnitTest++](https://github.com/unittest-cpp/unittest-cpp/wiki/Home)

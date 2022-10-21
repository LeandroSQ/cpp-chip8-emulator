# CHIP8 Emulator/Interpreter
This is a [CHIP8](https://en.wikipedia.org/wiki/CHIP-8) emulator/interpreter written in C++.
Using [SDL2](https://www.libsdl.org/) for graphics and input.

<p align="center">
    <img src=".github/spaceinvaders.png" height="200px">
    <img src=".github/maze.png" height="200px">
    <img src=".github/breakout.png" height="200px">
    <img src=".github/ufo.png" height="200px">
</p>

## How to run
### Linux/Mac
1. Install [Conan](https://conan.io/)
2. Install [CMake](https://cmake.org/)
3. Run `sh release.sh`
4. Run `./build/bin/chip8` passing the ROM file as argument
    1. Example: `./build/bin/chip8 'roms/invaders.bin'`

## Memory map
<pre align="left">
+-----------------------+
| 0x0050 - 0x01FF       | - Reserved memory (contains font set in emu)
+-----------------------+
| 0x0200 - 0x0FFF       | - Program ROM and work RAM
+-----------------------+
| 0x1FFF - 0xFFFF       | - Not used
+-----------------------+
</pre>

## Input map
The CHIP-8 has a HEX based keypad (0x0-0xF), which maps to the following key-matrix:
<pre align="center">
╔═══╦═══╦═══╦═══╗                        ╔═══╦═══╦═══╦═══╗
║ 1 ║ 2 ║ 3 ║ C ║                        ║ 1 ║ 2 ║ 3 ║ 4 ║
╠═══╬═══╬═══╬═══╣                        ╠═══╬═══╬═══╬═══╣
║ 4 ║ 5 ║ 6 ║ D ║                        ║ q ║ w ║ e ║ r ║
╠═══╬═══╬═══╬═══╣  which translates to:  ╠═══╬═══╬═══╬═══╣
║ 7 ║ 8 ║ 9 ║ E ║   (in the keyboard)    ║ a ║ s ║ d ║ f ║
╠═══╬═══╬═══╬═══╣                        ╠═══╬═══╬═══╬═══╣
║ A ║ 0 ║ B ║ F ║                        ║ z ║ x ║ c ║ v ║
╚═══╩═══╩═══╩═══╝                        ╚═══╩═══╩═══╩═══╝
</pre>

## Verbose mode

You can `#define VERBOSE` for extra debug information.
Which includes all the opcodes, their arguments and an explanation of what they do. In real time, performance impact is negligible.
<p align="center">
    <img src=".github/debug.png">
</p>
In the future, using a logic like this to implement a disassembler would be nice.

## Audio
This emulator uses [SDL2](https://www.libsdl.org/) for audio. Generating a sine wave with a frequency of 440Hz for 1/60th of a second. This is the same frequency as the original CHIP-8.

## Instruction set

| Instruction | Description |
| -- | -- |
|0NNN|Execute machine language subroutine at address NNN|
|00E0|Clear the screen|
|00EE|Return from a subroutine|
|1NNN|Jump to address NNN|
|2NNN|Execute subroutine starting at address NNN|
|3XNN|Skip the following instruction if the value of register VX equals NN|
|4XNN|Skip the following instruction if the value of register VX is not equal to NN|
|5XY0|Skip the following instruction if the value of register VX is equal to the value of register VY|
|6XNN|Store number NN in register VX|
|7XNN|Add the value NN to register VX|
|8XY0|Store the value of register VY in register VX|
|8XY1|Set VX to VX OR VY|
|8XY2|Set VX to VX AND VY|
|8XY3|Set VX to VX XOR VY|
|8XY4|Add the value of register VY to register VX. <br/>Set VF to 01 if a carry occurs. <br/>Set VF to 00 if a carry does not occur|
|8XY5|Subtract the value of register VY from register VX. <br/>Set VF to 00 if a borrow occurs. <br/>Set VF to 01 if a borrow does not occur|
|8XY6|Store the value of register VY shifted right one bit in register VX¹. <br/>Set register VF to the least significant bit prior to the shift. <br/>VY is unchanged|
|8XY7|Set register VX to the value of VY minus VX. <br/>Set VF to 00 if a borrow occurs. <br/>Set VF to 01 if a borrow does not occur|
|8XYE|Store the value of register VY shifted left one bit in register VX¹. <br/>Set register VF to the most significant bit prior to the shift. <br/>VY is unchanged|
|9XY0|Skip the following instruction if the value of register VX is not equal to the value of register VY|
|ANNN|Store memory address NNN in register I|
|BNNN|Jump to address NNN + V0|
|CXNN|Set VX to a random number with a mask of NN|
|DXYN|Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I. <br/>Set VF to 01 if any set pixels are changed to unset, and 00 otherwise|
|EX9E|Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed|
|EXA1|Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed|
|FX07|Store the current value of the delay timer in register VX|
|FX0A|Wait for a keypress and store the result in register VX|
|FX15|Set the delay timer to the value of register VX|
|FX18|Set the sound timer to the value of register VX|
|FX1E|Add the value stored in register VX to register I|
|FX29|Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX|
|FX33|Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2|
|FX55|Store the values of registers V0 to VX inclusive in memory starting at address I. <br/>I is set to I + X + 1 after operation|
|FX65|Fill registers V0 to VX inclusive with the values stored in memory starting at address I. <br/>I is set to I + X + 1 after operation|

## Roadmap

For when I have spare time:

- [ ] Create a Graphical User Interface using Dear ImGui
  - [ ] Load ROM files within the GUI
  - [ ] Implement a register view within the GUI
  - [ ] Implement a stack view within the GUI
  - [ ] Implement a dissasembler view within the GUI
  - [ ] Implement a memory view within the GUI
  - [ ] Implement a keypad view within the GUI
  - [ ] Implement a screen view within the GUI
  - [ ] Implement a debug view within the GUI
  - [ ] Implement a settings view within the GUI
    - [ ] Define a CLOCK hz
    - [ ] Define a Frame Rate hz
    - [ ] Define the screen size
    - [ ] Define the screen scale
    - [ ] Define the screen color
    - [ ] Define the audio frequency

## References
| Name | Description |
| -- | -- |
| [Corax89's - CHIP8 Test room](https://github.com/corax89/chip8-test-rom) | Used for testing |
| [Zophar's domain - CHIP8 Game room pack](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html) | Used for rooms |
| [Austin Morlan's - CHIP8 article](https://austinmorlan.com/posts/chip8_emulator/#16-8-bit-registers) | Used as reference |
| [Matt Mikolay's - CHIP8 Reference](https://github.com/mattmikolay/chip-8/wiki/) | Used as reference |
| [Wikipedia - CHIP8 Page](https://en.wikipedia.org/wiki/CHIP-8) | Used as reference |

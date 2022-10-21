#include "cpu.hpp"
#include "logger.hpp"
#include "random.hpp"
#include "util.hpp"

CPU::CPU() { }

CPU::~CPU() { }

void CPU::execute(uint16_t opcode, Memory& memory, Input& input, Renderer& renderer) {
	// TODO: Move this to each instruction
	const uint16_t nnn = opcode & 0x0FFF;
	const uint16_t nn = opcode & 0x00FF;
	const uint8_t n = opcode & 0x000F;
	const uint8_t x = (opcode & 0x0F00) >> 8;
	const uint8_t y = (opcode & 0x00F0) >> 4;

	switch ((opcode & 0xF000) >> 12) {
		case 0x0:
			switch (opcode & 0x000F) {
				case 0x0:
					// 00E0 - CLS
					opcode00E0(renderer);
					break;
				case 0xE:
					// 00EE - RET
					opcode00EE();
					break;
				default:
					Log::error("[CPU] Unknown opcode: 0x", toHex(opcode, 4));
					break;
			}
			break;
		case 0x1:
			// 1NNN - JP addr
			opcode1NNN(nnn);
			break;
		case 0x2:
			// 2NNN - CALL addr
			opcode2NNN(nnn);
			break;
		case 0x3:
			// 3XNN - SE Vx, byte
			opcode3XNN(x, nn);
			break;
		case 0x4:
			// 4XNN - SNE Vx, byte
			opcode4XNN(x, nn);
			break;
		case 0x5:
			// 5xy0 - SE Vx, Vy
			opcode5XY0(x, y);
			break;
		case 0x6:
			// 6XNN - LD Vx, byte
			opcode6XNN(x, nn);
			break;
		case 0x7:
			// 7XNN - ADD Vx, byte
			opcode7XNN(x, nn);
			break;
		case 0x8: // Operations on registers
			switch (opcode & 0x000F) {
				case 0x0:
					// 8XY0 - MOV Vx, Vy
					opcode8XY0(x, y);
					break;
				case 0x1:
					// 8XY1 - OR Vx, Vy
					opcode8XY1(x, y);
					break;
				case 0x2:
					// 8XY2 - AND Vx, Vy
					opcode8XY2(x, y);
					break;
				case 0x3:
					// 8XY3 - XOR Vx, Vy
					opcode8XY3(x, y);
					break;
				case 0x4:
					// 8XY4 - ADD Vx, Vy
					opcode8XY4(x, y);
					break;
				case 0x5:
					// 8XY5 - SUB Vx, Vy
					opcode8XY5(x, y);
					break;
				case 0x6:
					// 8XY6 - SHR Vx {, Vy}
					opcode8XY6(x);
					break;
				case 0x7:
					// 8XY7 - SUBN Vx, Vy
					opcode8XY7(x, y);
					break;
				case 0xE:
					// 8XYE - SHL Vx {, Vy}
					opcode8XYE(x);
					break;
				default:
					Log::error("[CPU] Unknown opcode: 0x", toHex(opcode, 4));
					break;
			}
			break;
		case 0x9:
			// 9XY0 - SNE Vx, Vy
			opcode9XY0(x, y);
			break;
		case 0xA:
			// ANNN - LD I, addr
			opcodeANNN(nnn);
			break;
		case 0xB:
			// BNNN - JP V0, addr
			opcodeBNNN(nnn);
			break;
		case 0xC:
			// CXNN - RND Vx, byte
			opcodeCXNN(x, nn);
			break;
		case 0xD:
			// DXYN - DRW Vx, Vy, nibble
			opcodeDXYN(x, y, n, memory, renderer);
			break;
		case 0xE: // Input
			switch (opcode & 0x00FF) {
				case 0x9E:
					// EX9E - SKP Vx
					opcodeEX9E(x, input);
					break;
				case 0xA1:
					// EXA1 - SKNP Vx
					opcodeEXA1(x, input);
					break;
				default:
					Log::error("[CPU] Unknown opcode: 0x", toHex(opcode, 4));
					break;
			}
			break;
		case 0xF:
			switch (opcode & 0x00FF) {
				case 0x07:
					// FX07 - LD Vx, DT
					opcodeFX07(x);
					break;
				case 0x0A:
					// FX0A - LD Vx, K
					opcodeFX0A(x, input);
					break;
				case 0x15:
					// FX15 - LD DT, Vx
					opcodeFX15(x);
					break;
				case 0x18:
					// FX18 - LD ST, Vx
					opcodeFX18(x);
					break;
				case 0x1E:
					// FX1E - ADD I, Vx
					opcodeFX1E(x);
					break;
				case 0x29:
					// FX29 - LD F, Vx
					opcodeFX29(x);
					break;
				case 0x33:
					// FX33 - LD B, Vx
					opcodeFX33(x, memory);
					break;
				case 0x55:
					// FX55 - LD [I], Vx
					opcodeFX55(x, memory);
					break;
				case 0x65:
					// FX65 - LD Vx, [I]
					opcodeFX65(x, memory);
					break;
				default:
					Log::error("[CPU] Unknown opcode: 0x", toHex(opcode, 4));
					break;
			}
			break;

		default:
			Log::error("[CPU] Unknown opcode: 0x", toHex(opcode, 4));
			break;
	}
}

#pragma region Opcodes
void CPU::opcode00E0(Renderer& renderer) {
	// Clear the display
	Log::debug("[CPU] 00E0 - Clearing the display");

	// Fills with the black color
	renderer.fill(0x00);
}

void CPU::opcode00EE() {
	// Return from a subroutine
	Log::debug("[CPU] 00EE - Returning from subroutine");

	programCounter = stack[--stackPointer];
}

void CPU::opcode1NNN(uint16_t address) {
	// Jump to address NNN
	Log::debug("[CPU] 1NNN - Jumping to address 0x", toHex(address, 3));

	programCounter = address;
}

void CPU::opcode2NNN(uint16_t address) {
	// Call subroutine at address NNN
	Log::debug("[CPU] 2NNN - Gosub to address 0x", toHex(address, 3));

	// Check if the stack is full
	if (stackPointer >= sizeof(stack) / sizeof(stack[0])) {
		Log::error("[CPU] Stack overflow");
		return;
	}

	// Push the current program counter to the stack
	stack[stackPointer++] = programCounter;

	// Set the program counter to the address NNN
	programCounter = address;
}

void CPU::opcode3XNN(uint8_t x, uint8_t nn) {
	// Skip the next instruction if Vx == NN
	Log::debug("[CPU] 3XNN - Skipping next instruction if V", toHex(x, 2), " == 0x", toHex(nn, 2));

	if (registers[x] == nn) {
		// Each opcode is 2 bytes long, so skip the next one
		programCounter += sizeof(uint16_t);
	}
}

void CPU::opcode4XNN(uint8_t x, uint8_t nn) {
	// Skip the next instruction if Vx != NN
	Log::debug("[CPU] 4XNN - Skipping next instruction if V", toHex(x, 2), " != 0x", toHex(nn, 2));

	if (registers[x] != nn) {
		// Each opcode is 2 bytes long, so skip the next one
		programCounter += sizeof(uint16_t);
	}
}

void CPU::opcode5XY0(uint8_t x, uint8_t y) {
	// Skip the next instruction if Vx == Vy
	Log::debug("[CPU] 5XY0 - Skipping next instruction if V", toHex(x, 0), " == V", toHex(y, 0));

	if (registers[x] == registers[y]) {
		// Each opcode is 2 bytes long, so skip the next one
		programCounter += sizeof(uint16_t);
	}
}

void CPU::opcode6XNN(uint8_t x, uint8_t nn) {
	// Set Vx to NN
	Log::debug("[CPU] 6XNN - Set V", toHex(x, 0), " to 0x", toHex(nn, 2));

	registers[x] = nn;
}

void CPU::opcode7XNN(uint8_t x, uint8_t nn) {
	// Add NN to Vx
	Log::debug("[CPU] 7XNN - Add 0x", toHex(nn, 2), " to V", toHex(x, 0));

	registers[x] += nn;
}

void CPU::opcode8XY0(uint8_t x, uint8_t y) {
	// Set Vx to the value of Vy
	Log::debug("[CPU] 8XY0 - Set V", toHex(x, 0), " to the value of V", toHex(y, 0));

	registers[x] = registers[y];
}

void CPU::opcode8XY1(uint8_t x, uint8_t y) {
	// Set Vx to Vx OR Vy
	Log::debug("[CPU] 8XY1 - Set V", toHex(x, 0), " to V", x, " OR V", toHex(y, 0));

	registers[x] |= registers[y];
}

void CPU::opcode8XY2(uint8_t x, uint8_t y) {
	// Set Vx to Vx AND Vy
	Log::debug("[CPU] 8XY2 - Set V", toHex(x, 0), " to V", toHex(x, 0), " AND V", toHex(y, 0));

	registers[x] &= registers[y];
}

void CPU::opcode8XY3(uint8_t x, uint8_t y) {
	// Set Vx to Vx XOR Vy
	Log::debug("[CPU] 8XY3 - Set V", toHex(x, 0), " to V", toHex(x, 0), " XOR V", toHex(y, 0));

	registers[x] ^= registers[y];
}

void CPU::opcode8XY4(uint8_t x, uint8_t y) {
	// Add Vy to Vx, set VF to 1 if there is a carry, and 0 if there isn't
	Log::debug("[CPU] 8XY4 - Add V", toHex(y, 0), " to V", toHex(x, 0), ", set VF to 1 if there is a carry, and 0 if there isn't");

	registers[0xF] = (registers[x] + registers[y] > 0xFF) ? 1 : 0;
	registers[x] += registers[y];
}

void CPU::opcode8XY5(uint8_t x, uint8_t y) {
	// Subtract Vy from Vx, set VF to 0 if there is a borrow, and 1 if there isn't
	Log::debug(
		"[CPU] 8XY5 - Subtract V", toHex(y, 0), " from V", toHex(x, 0), ", set VF to 0 if there is a borrow, and 1 if there isn't"
	);

	registers[0xF] = (registers[x] > registers[y]) ? 1 : 0;
	registers[x] -= registers[y];
}

void CPU::opcode8XY6(uint8_t x) {
	// Store the least significant bit of Vx in VF and then shift Vx to the right by 1
	Log::debug(
		"[CPU] 8XY6 - Store the least significant bit of V",
		toHex(x, 0),
		" in VF and then shift V",
		toHex(x, 0),
		" to the right by 1"
	);

	registers[0xF] = registers[x] & 0x1;
	registers[x] >>= 1;
}

void CPU::opcode8XY7(uint8_t x, uint8_t y) {
	// Set Vx to Vy minus Vx, set VF to 0 if there is a borrow, and 1 if there isn't
	Log::debug(
		"[CPU] 8XY7 - Set V",
		toHex(x, 0),
		" to V",
		toHex(y, 0),
		" minus V",
		toHex(x, 0),
		", set VF to 0 if there is a borrow, and 1 if there isn't"
	);

	registers[0xF] = (registers[y] > registers[x]) ? 1 : 0;
	registers[x] = registers[y] - registers[x];
}

void CPU::opcode8XYE(uint8_t x) {
	// Store the most significant bit of Vx in VF and then shift Vx to the left by 1
	Log::debug(
		"[CPU] 8XYE - Store the most significant bit of V", toHex(x, 0), " in VF and then shift V", toHex(x, 0), " to the left by 1"
	);

	registers[0xF] = registers[x] >> 7;
	registers[x] <<= 1;
}

void CPU::opcode9XY0(uint8_t x, uint8_t y) {
	// Skip the next instruction if Vx != Vy
	Log::debug("[CPU] 9XY0 - Skipping next instruction if V", toHex(x, 0), " != V", toHex(y, 0));

	if (registers[x] != registers[y]) {
		// Each opcode is 2 bytes long, so skip the next one
		programCounter += sizeof(uint16_t);
	}
}

void CPU::opcodeANNN(uint16_t nnn) {
	// Set I to the address NNN
	Log::debug("[CPU] ANNN - Set I to the address 0x", toHex(nnn, 2));

	indexRegister = nnn;
}

void CPU::opcodeBNNN(uint16_t nnn) {
	// Jump to the address NNN + V0
	Log::debug("[CPU] BNNN - Jump to the address 0x", toHex(nnn, 2), " + V0");

	programCounter = nnn + registers[0];
}

void CPU::opcodeCXNN(uint8_t x, uint8_t nn) {
	// Set Vx to a random number and NN
	Log::debug("[CPU] CXNN - Set V", toHex(x, 0), " to a random number and 0x", toHex(nn, 2));

	registers[x] = Random::byte() & nn;
}

void CPU::opcodeDXYN(uint8_t x, uint8_t y, uint8_t n, Memory& memory, Renderer& renderer) {
	// Draw a sprite at coordinate (Vx, Vy) that has a width of 8 pixels and a height of N pixels
	Log::debug("[CPU] DXYN - Draw sprite at (V", toHex(x, 0), ", V", toHex(y, 0), ") with width 8 and height 0x", toHex(n, 0));

	// Clamp the position within the viewport
	uint8_t positionX = registers[x] % Renderer::width;
	uint8_t positionY = registers[y] % Renderer::height;

	// Reset the collision flag
	registers[0xF] = 0;

	// Loop through the sprite's rows
	for (uint8_t row = 0; row < n; row++) {
		uint8_t spriteByte = memory.read(indexRegister + row);
		for (uint8_t col = 0; col < 8; col++) {
			// Check if the pixel is set
			if ((spriteByte & (0x80 >> col)) != 0x0) {
				// Check if the pixel is already set
				if (renderer.getPixel(positionX + col, positionY + row)) {
					// There's a collision, set the collision flag to 1
					registers[0xF] = 1;
				}

				// Toggle the pixel
				renderer.togglePixel(positionX + col, positionY + row);
			}
		}
	}
}

void CPU::opcodeEX9E(uint8_t x, Input& input) {
	// Skip the next instruction if the key stored in Vx is pressed
	Log::debug("[CPU] EX9E - Skipping next instruction if the key stored in V", toHex(x, 0), " is pressed");

	const bool isKeyDown = input.isKeyDown(registers[x]);
	if (isKeyDown) {
		// Each opcode is 2 bytes long, so skip the next one
		programCounter += sizeof(uint16_t);
	}
}

void CPU::opcodeEXA1(uint8_t x, Input& input) {
	// Skip the next instruction if the key stored in Vx isn't pressed
	Log::debug("[CPU] EXA1 - Skipping next instruction if the key stored in V", toHex(x, 0), " isn't pressed");

	const bool isKeyUp = input.isKeyUp(registers[x]);
	if (isKeyUp) {
		// Each opcode is 2 bytes long, so skip the next one
		programCounter += sizeof(uint16_t);
	}
}

void CPU::opcodeFX07(uint8_t x) {
	// Set Vx to the value of the delay timer
	Log::debug("[CPU] FX07 - Set V", toHex(x, 0), " to the value of the delay timer");

	registers[x] = delayTimer;
}

void CPU::opcodeFX0A(uint8_t x, Input& input) {
	// Wait for a key press, store the value of the key in Vx
	Log::debug("[CPU] FX0A - Wait for a key press, store the value of the key in V", toHex(x, 0));

	const uint8_t key = input.getFirstKeyDownIndex();

	// Get the first key that is down and store to Vx
	if (key != 0xFF) {
		registers[x] = key;
	} else {
		// If no key is down, repeat this instruction
		programCounter -= sizeof(uint16_t);
	}
}

void CPU::opcodeFX15(uint8_t x) {
	// Set the delay timer to Vx
	Log::debug("[CPU] FX15 - Set the delay timer to V", toHex(x, 0));

	delayTimer = registers[x];
}

void CPU::opcodeFX18(uint8_t x) {
	// Set the sound timer to Vx
	Log::debug("[CPU] FX18 - Set the sound timer to V", toHex(x, 0));

	soundTimer = registers[x];
}

void CPU::opcodeFX1E(uint8_t x) {
	// Add Vx to I
	Log::debug("[CPU] FX1E - Add V", toHex(x, 0), " to I");

	indexRegister += registers[x];
}

void CPU::opcodeFX29(uint8_t x) {
	// Set I to the location of the sprite for the character in Vx
	Log::debug("[CPU] FX29 - Set I to the location of the sprite for the character in V", toHex(x, 0));

    constexpr uint8_t spriteSize = 5;
    indexRegister = Memory::fontSetStart + registers[x] * spriteSize;
}

void CPU::opcodeFX33(uint8_t x, Memory& memory) {
	// Store the binary-coded decimal representation of Vx at the addresses I, I + 1, and I + 2
	Log::debug(
		"[CPU] FX33 - Store the binary-coded decimal representation of V", toHex(x, 0), " at the addresses I, I + 1, and I + 2"
	);

	uint8_t value = registers[x];
	memory.write(indexRegister + 2, value % 10); // Ones
	value /= 10;
	memory.write(indexRegister + 1, value % 10); // Tens
	value /= 10;
	memory.write(indexRegister, value % 10); // Hundreds
}

void CPU::opcodeFX55(uint8_t x, Memory& memory) {
	// Store V0 to Vx in memory starting at address I
	Log::debug("[CPU] FX55 - Store V0 to V", toHex(x, 0), " in memory starting at address I");

	for (uint8_t i = 0; i <= x; i++) {
		memory.write(indexRegister + i, registers[i]);
	}
	indexRegister = indexRegister + x + 1;
}

void CPU::opcodeFX65(uint8_t x, Memory& memory) {
	// Fill V0 to Vx with values from memory starting at address I
	Log::debug("[CPU] FX65 - Fill V0 to V", toHex(x, 0), " with values from memory starting at address I");

	for (uint8_t i = 0; i <= x; i++) {
		registers[i] = memory.read(indexRegister + i);
	}
	indexRegister = indexRegister + x + 1;
}
#pragma endregion Opcodes

void CPU::updateTimers(double deltaTime) {
	// Update the timer 60 times per second
	constexpr double timerFrequency = 1.0 / 60.0;

	timerAccumulator += deltaTime;

	// Update the timers if enough time has passed
	if (timerAccumulator >= timerFrequency) {
		// Decrement the timers if they are greater than 0
		if (delayTimer > 0) delayTimer--;
		if (soundTimer > 0) soundTimer--;

		// Reset the accumulator
		timerAccumulator -= timerFrequency;
	}
}

void CPU::reset() {
    // Reset the registers and memory
    memset(registers, 0, sizeof(registers));
    memset(stack, 0, sizeof(stack));

    // Reset the timers
    delayTimer = 0;
    soundTimer = 0;

    // Reset the program counter and index register
    programCounter = Memory::programStart;
    indexRegister = 0;

    // Reset the stack pointer
    stackPointer = 0;

    // Reset the accumulator
    timerAccumulator = 0.0;
}
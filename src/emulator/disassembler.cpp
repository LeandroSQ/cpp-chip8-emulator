#include "disassembler.hpp"
#include "../utils/logger.hpp"
#include "../utils/util.hpp"

Instruction::Instruction(uint8_t opcode, uint16_t address, std::string name, std::string description)
		: opcode(opcode), address(address), name(name), description(description) { }

inline void handleUnkownOpcode(std::vector<Instruction>& buffer, uint16_t opcode, uint16_t address) {
    buffer.emplace_back(opcode, address, "_" + toHex(address, 4) + " dw 0x" + toHex(opcode, 4), "Unknown opcode, treating as data");
}

std::vector<Instruction> Disassembler::disassembleData(uint8_t* data, size_t size) {
	std::vector<Instruction> buffer;
	uint16_t programCounter = 0x0000;

	// Loop through the program
	while (programCounter < size) {
		// Fetch the opcode
		uint16_t opcode = data[programCounter] << 8 | data[programCounter + 1];
        programCounter += sizeof(uint16_t);

		// Decode possible arguments
		const uint16_t nnn = opcode & 0x0FFF;
		const uint16_t nn = opcode & 0x00FF;
		const uint8_t n = opcode & 0x000F;
		const uint8_t x = (opcode & 0x0F00) >> 8;
		const uint8_t y = (opcode & 0x00F0) >> 4;

		switch ((opcode & 0xF000) >> 12) {
			case 0x0:
				switch (opcode & 0x00FF) {
					case 0xE0:
						// 00E0 - CLS
						buffer.emplace_back(opcode, programCounter, "CLS", "Clear the display");
						break;
					case 0xEE:
						// 00EE - RET
						buffer.emplace_back(opcode, programCounter, "RET", "Return from a subroutine");
						break;
					default:
						Log::warn("[Disassembler] Unknown opcode: 0x", toHex(opcode, 4));
                        handleUnkownOpcode(buffer, opcode, programCounter);
						break;
				}
				break;
			case 0x1:
				// 1NNN - JP addr
                buffer.emplace_back(opcode, programCounter, "JP 0x" + toHex(nnn, 3), "Jump to address 0x" + toHex(nnn, 3));
				break;
			case 0x2:
				// 2NNN - CALL addr
                buffer.emplace_back(opcode, programCounter, "GOSUB 0x" + toHex(nnn, 3), "Call subroutine at 0x" + toHex(nnn, 3));
				break;
			case 0x3:
				// 3XNN - SE Vx, byte
                buffer.emplace_back(opcode, programCounter, "SE V" + toHex(x, 1) + ", 0x" + toHex(nn, 2), "Skip next instruction if V" + toHex(x, 1) + " == 0x" + toHex(nn, 2));
				break;
			case 0x4:
				// 4XNN - SNE Vx, byte
                buffer.emplace_back(opcode, programCounter, "SNE V" + toHex(x, 1) + ", 0x" + toHex(nn, 2), "Skip next instruction if V" + toHex(x, 1) + " != 0x" + toHex(nn, 2));
				break;
			case 0x5:
				// 5xy0 - SE Vx, Vy
                buffer.emplace_back(opcode, programCounter, "SE V" + toHex(x, 1) + ", V" + toHex(y, 1), "Skip next instruction if V" + toHex(x, 1) + " == V" + toHex(y, 1));
				break;
			case 0x6:
				// 6XNN - LD Vx, byte
                buffer.emplace_back(opcode, programCounter, "LD V" + toHex(x, 1) + ", 0x" + toHex(nn, 2), "Set V" + toHex(x, 1) + " = 0x" + toHex(nn, 2));
				break;
			case 0x7:
				// 7XNN - ADD Vx, byte
                buffer.emplace_back(opcode, programCounter, "ADD V" + toHex(x, 1) + ", 0x" + toHex(nn, 2), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " + 0x" + toHex(nn, 2));
				break;
			case 0x8: // Operations on registers
				switch (opcode & 0x000F) {
					case 0x0:
						// 8XY0 - MOV Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "MOV V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(y, 1));
						break;
					case 0x1:
						// 8XY1 - OR Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "OR V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " | V" + toHex(y, 1));
						break;
					case 0x2:
						// 8XY2 - AND Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "AND V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " & V" + toHex(y, 1));
						break;
					case 0x3:
						// 8XY3 - XOR Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "XOR V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " ^ V" + toHex(y, 1));
						break;
					case 0x4:
						// 8XY4 - ADD Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "ADD V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " + V" + toHex(y, 1));
						break;
					case 0x5:
						// 8XY5 - SUB Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "SUB V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " - V" + toHex(y, 1));
						break;
					case 0x6:
						// 8XY6 - SHR Vx {, Vy}
                        buffer.emplace_back(opcode, programCounter, "SHR V" + toHex(x, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " >> 1");
						break;
					case 0x7:
						// 8XY7 - SUBN Vx, Vy
                        buffer.emplace_back(opcode, programCounter, "SUBN V" + toHex(x, 1) + ", V" + toHex(y, 1), "Set V" + toHex(x, 1) + " = V" + toHex(y, 1) + " - V" + toHex(x, 1));
						break;
					case 0xE:
						// 8XYE - SHL Vx {, Vy}
                        buffer.emplace_back(opcode, programCounter, "SHL V" + toHex(x, 1), "Set V" + toHex(x, 1) + " = V" + toHex(x, 1) + " << 1");
						break;
					default:
						Log::warn("[Disassembler] Unknown opcode: 0x", toHex(opcode, 4));
                        handleUnkownOpcode(buffer, opcode, programCounter);
						break;
				}
				break;
			case 0x9:
				// 9XY0 - SNE Vx, Vy
                buffer.emplace_back(opcode, programCounter, "SNE V" + toHex(x, 1) + ", V" + toHex(y, 1), "Skip next instruction if V" + toHex(x, 1) + " != V" + toHex(y, 1));
				break;
			case 0xA:
				// ANNN - LD I, addr
                buffer.emplace_back(opcode, programCounter, "LD I, 0x" + toHex(nnn, 3), "Set I = 0x" + toHex(nnn, 3));
				break;
			case 0xB:
				// BNNN - JP V0, addr
                buffer.emplace_back(opcode, programCounter, "JP V0, 0x" + toHex(nnn, 3), "Jump to 0x" + toHex(nnn, 3) + " + V0");
				break;
			case 0xC:
				// CXNN - RND Vx, byte
                buffer.emplace_back(opcode, programCounter, "RND V" + toHex(x, 1) + ", 0x" + toHex(nn, 2), "Set V" + toHex(x, 1) + " = random byte AND 0x" + toHex(nn, 2));
				break;
			case 0xD:
				// DXYN - DRW Vx, Vy, nibble
                buffer.emplace_back(opcode, programCounter, "DRW V" + toHex(x, 1) + ", V" + toHex(y, 1) + ", 0x" + toHex(n, 1), "Display n-byte sprite starting at memory location I at (V" + toHex(x, 1) + ", V" + toHex(y, 1) + "), set VF = collision");
				break;
			case 0xE: // Input
				switch (opcode & 0x00FF) {
					case 0x9E:
						// EX9E - SKP Vx
                        buffer.emplace_back(opcode, programCounter, "SKP V" + toHex(x, 1), "Skip next instruction if key with the value of V" + toHex(x, 1) + " is pressed");
						break;
					case 0xA1:
						// EXA1 - SKNP Vx
                        buffer.emplace_back(opcode, programCounter, "SKNP V" + toHex(x, 1), "Skip next instruction if key with the value of V" + toHex(x, 1) + " is not pressed");
						break;
					default:
						Log::warn("[Disassembler] Unknown opcode: 0x", toHex(opcode, 4));
                        handleUnkownOpcode(buffer, opcode, programCounter);
						break;
				}
				break;
			case 0xF:
				switch (opcode & 0x00FF) {
					case 0x07:
						// FX07 - LD Vx, DT
                        buffer.emplace_back(opcode, programCounter, "LD V" + toHex(x, 1) + ", DT", "Set V" + toHex(x, 1) + " = delay timer value");
						break;
					case 0x0A:
						// FX0A - LD Vx, K
                        buffer.emplace_back(opcode, programCounter, "LD V" + toHex(x, 1) + ", K", "Wait for a key press, store the value of the key in V" + toHex(x, 1));
						break;
					case 0x15:
						// FX15 - LD DT, Vx
                        buffer.emplace_back(opcode, programCounter, "LD DT, V" + toHex(x, 1), "Set delay timer = V" + toHex(x, 1));
						break;
					case 0x18:
						// FX18 - LD ST, Vx
                        buffer.emplace_back(opcode, programCounter, "LD ST, V" + toHex(x, 1), "Set sound timer = V" + toHex(x, 1));
						break;
					case 0x1E:
						// FX1E - ADD I, Vx
                        buffer.emplace_back(opcode, programCounter, "ADD I, V" + toHex(x, 1), "Set I = I + V" + toHex(x, 1));
						break;
					case 0x29:
						// FX29 - LD F, Vx
                        buffer.emplace_back(opcode, programCounter, "LD F, V" + toHex(x, 1), "Set I = location of sprite for digit V" + toHex(x, 1));
						break;
					case 0x33:
						// FX33 - LD B, Vx
                        buffer.emplace_back(opcode, programCounter, "LD B, V" + toHex(x, 1), "Store BCD representation of V" + toHex(x, 1) + " in memory locations I, I+1, and I+2");
						break;
					case 0x55:
						// FX55 - LD [I], Vx
                        buffer.emplace_back(opcode, programCounter, "LD [I], V" + toHex(x, 1), "Store registers V0 through V" + toHex(x, 1) + " in memory starting at location I");
						break;
					case 0x65:
						// FX65 - LD Vx, [I]
                        buffer.emplace_back(opcode, programCounter, "LD V" + toHex(x, 1) + ", [I]", "Read registers V0 through V" + toHex(x, 1) + " from memory starting at location I");
						break;
					default:
						Log::warn("[Disassembler] Unknown opcode: 0x", toHex(opcode, 4));
                        handleUnkownOpcode(buffer, opcode, programCounter);
						break;
				}
				break;

			default:
				Log::warn("[Disassembler] Unknown opcode: 0x", toHex(opcode, 4));
                handleUnkownOpcode(buffer, opcode, programCounter);
				break;
		}
	}

    return buffer;
}

std::vector<Instruction> Disassembler::disassembleFile(const char* filename) {
	// Read the binary file content
    uint8_t* data;
    size_t size;
    if (readFile(filename, data, size) < 0) {
        Log::error("[Disassembler] Failed to read file: ", filename);
        return { };
    }

    // Disassemble the binary data
    return disassembleData(data, size);
}

void Disassembler::print(std::vector<Instruction> instructions) {
    std::cout << Log::RESET << Log::STYLE_BOLD
              << "ADDR"
              << Log::COLOR_BLUE << " OPCODE "
              << Log::COLOR_MAGENTA << "ASM\t\t"
              << Log::COLOR_GRAY << "DESC"
              << Log::RESET << std::endl;

    for (auto i = instructions.begin(); i != instructions.end(); i++) {
        std::cout << Log::STYLE_BOLD << Log::STYLE_UNDERLINE << toHex(i->address, 4)
                  << Log::RESET << Log::COLOR_BLUE << " [" << toHex(i->opcode, 4) << "] "
                  << Log::RESET << Log::COLOR_MAGENTA << i->name
                  << Log::RESET << Log::COLOR_GRAY << " ; " << i->description << Log::RESET << std::endl;
    }
}
#pragma once
#include "input.hpp"
#include "memory.hpp"
#include "renderer.hpp"

class CPU {
  public:
	// 16 8-bit registers, V0 - VF
	uint8_t registers[16] = { 0 };

	// Special register, used for memory addresses
	uint16_t indexRegister = 0;

	// Program counter, used to store the currently executing address, starts at 0x200
	uint16_t programCounter = Memory::programStart;

	// Stack holds the address that the interpreter should return to when finished with a subroutine
	uint16_t stack[16];
	// Stack pointer, points to the topmost level of the stack
	uint8_t stackPointer = 0;

	// Delay timer, counts down to 0 at 60Hz
	uint8_t delayTimer = 0;
	uint8_t soundTimer = 0;

	CPU();

	~CPU();

	void execute(uint16_t opcode, Memory& memory, Input& input, Renderer& renderer);

	void updateTimers();

	void reset();

  private:
	inline void opcode00E0(Renderer& renderer);
	inline void opcode00EE();
	inline void opcode1NNN(uint16_t address);
	inline void opcode2NNN(uint16_t address);
	inline void opcode3XNN(uint8_t x, uint8_t value);
	inline void opcode4XNN(uint8_t x, uint8_t value);
	inline void opcode5XY0(uint8_t x, uint8_t y);
	inline void opcode6XNN(uint8_t x, uint8_t value);
	inline void opcode7XNN(uint8_t x, uint8_t value);
	inline void opcode8XY0(uint8_t x, uint8_t y);
	inline void opcode8XY1(uint8_t x, uint8_t y);
	inline void opcode8XY2(uint8_t x, uint8_t y);
	inline void opcode8XY3(uint8_t x, uint8_t y);
	inline void opcode8XY4(uint8_t x, uint8_t y);
	inline void opcode8XY5(uint8_t x, uint8_t y);
	inline void opcode8XY6(uint8_t x);
	inline void opcode8XY7(uint8_t x, uint8_t y);
	inline void opcode8XYE(uint8_t x);
	inline void opcode9XY0(uint8_t x, uint8_t y);
	inline void opcodeANNN(uint16_t nnn);
	inline void opcodeBNNN(uint16_t nnn);
	inline void opcodeCXNN(uint8_t x, uint8_t nn);
	inline void opcodeDXYN(uint8_t x, uint8_t y, uint8_t n, Memory& memory, Renderer& renderer);
	inline void opcodeEX9E(uint8_t x, Input& input);
	inline void opcodeEXA1(uint8_t x, Input& input);
	inline void opcodeFX07(uint8_t x);
	inline void opcodeFX0A(uint8_t x, Input& input);
	inline void opcodeFX15(uint8_t x);
	inline void opcodeFX18(uint8_t x);
	inline void opcodeFX1E(uint8_t x);
	inline void opcodeFX29(uint8_t x);
	inline void opcodeFX33(uint8_t x, Memory& memory);
	inline void opcodeFX55(uint8_t x, Memory& memory);
	inline void opcodeFX65(uint8_t x, Memory& memory);
};
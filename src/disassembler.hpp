#pragma once
#include <string>
#include <vector>

struct Instruction {
	uint8_t opcode;
	uint16_t address;
	std::string name;
	std::string description;

	Instruction(uint8_t opcode, uint16_t address, std::string name, std::string description);
};

class Disassembler {
  private:
    static std::vector<Instruction> disassembleData(uint8_t* data, size_t size);

  public:
    static std::vector<Instruction> disassemble(const char* filename);

    static void print(std::vector<Instruction> instructions);
};
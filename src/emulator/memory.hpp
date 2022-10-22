#pragma once

#include <cstdint>

class Memory {
  public:
	static constexpr uint16_t fontSetStart = 0x0050;
	static constexpr uint16_t size = 4096;
	static constexpr uint16_t programStart = 0x0200;

  private:
	uint8_t data[Memory::size] = { 0 };

  public:
	Memory();

	~Memory();

	int8_t init();

	uint8_t read(uint16_t address);

	void write(uint16_t address, uint8_t value);

	void write(uint16_t address, uint8_t* value);

	void reset();

  private:
	void loadFontset();
};
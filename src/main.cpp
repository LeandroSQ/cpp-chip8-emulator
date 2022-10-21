#include "app.hpp"
#include "logger.hpp"
#include "disassembler.hpp"
#include <vector>
#include "util.hpp"

int main(int argc, char** argv) {
    Log::debug("[Main] Parsing arguments...");
	Log::debug("[Main] Argc: ", argc);
	for (int i = 0; i < argc; i++) {
		Log::debug("[Main] Argv[", i, "]: ", argv[i]);
	}

	if (argc < 2) {
		Log::error("No ROM path provided");
		return -1;
	}

    if (argc == 3 && std::string(argv[1]) == "--disassemble") {
        Log::debug("[Main] Disassembling ROM...");

        std::vector<Instruction> instructions = Disassembler::disassemble(argv[2]);
        Disassembler::print(instructions);
    } else {
        App app;
        if (app.init() < 0) return -1;
        if (app.loadROM(argv[1]) < 0) return -1;
        app.run();
    }


	return 0;
}
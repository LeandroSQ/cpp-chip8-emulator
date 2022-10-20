#pragma once
#include <iostream>
#include <stdio.h>

class Log {
  private:
	enum class Level { Info, Error };

	static const char* COLOR_RED;
	static const char* COLOR_BLUE;
	static const char* COLOR_MAGENTA;
	static const char* COLOR_YELLOW;
	static const char* COLOR_RESET;

  public:

    template<typename... Args>
	static void info(Args... m) {
		// Print the log level colored
        std::cout << COLOR_MAGENTA << "Info: " << COLOR_RESET;
        (std::cout << ... << m);
        std::cout << std::endl;
	}

	template<typename... Args>
	static void error(Args... m) {
		// Print the log level colored
        std::cout << COLOR_RED << "Error: " << COLOR_RESET;
        (std::cout << ... << m);
        std::cout << std::endl;
	}
};
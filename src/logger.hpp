#pragma once
#include <iostream>

#define VERBOSE

class Log {
  private:
	static const char* COLOR_RED;
	static const char* COLOR_BLUE;
	static const char* COLOR_MAGENTA;
	static const char* COLOR_YELLOW;
	static const char* COLOR_RESET;

  public:
	template <typename... Args> static void info(Args... m) {
		// Print the log level colored
		std::cout << COLOR_MAGENTA << "Info:\t" << COLOR_RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
	}

	template <typename... Args> static void error(Args... m) {
		// Print the log level colored
		std::cerr << COLOR_RED << "Error:\t" << COLOR_RESET;
		(std::cerr << ... << m);
		std::cerr << std::endl;
	}

	template <typename... Args> static void debug(Args... m) {
#ifdef VERBOSE
		// Print the log level colored
		std::cout << COLOR_YELLOW << "Debug:\t" << COLOR_RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
#endif
	}
};
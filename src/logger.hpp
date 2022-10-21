#pragma once
#include <iostream>
#include "util.hpp"

#define VERBOSE

class Log {
  public:
    static const char* COLOR_GRAY;
    static const char* COLOR_RED;
	static const char* COLOR_BLUE;
	static const char* COLOR_MAGENTA;
	static const char* COLOR_YELLOW;
	static const char* RESET;
	static const char* STYLE_BOLD;
	static const char* STYLE_UNDERLINE;

	template <typename... Args> static void info(Args... m) {
		// Print the log level colored
		std::cout << STYLE_BOLD << COLOR_MAGENTA << "Info:\t" << RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
	}

	template <typename... Args> static void error(Args... m) {
		// Print the log level colored
		std::cerr << STYLE_BOLD << COLOR_RED << "Error:\t" << RESET;
		(std::cerr << ... << m);
		std::cerr << std::endl;
	}

    template <typename... Args> static void warn(Args... m) {
		// Print the log level colored
		std::cerr << STYLE_BOLD << COLOR_YELLOW << "Warn:\t" << RESET;
		(std::cerr << ... << m);
		std::cerr << std::endl;
	}

    // When the VERBOSE macro is undefined, this function will be empty, which will trigger a compiler warning, just ignoring that :p
    DISABLE_WARNING_PUSH
    DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
	template <typename... Args> static void debug(Args... m) {
#ifdef VERBOSE
		// Print the log level colored
		std::cout << STYLE_BOLD << COLOR_BLUE << "Debug:\t" << RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
#endif
	}
    DISABLE_WARNING_POP

};
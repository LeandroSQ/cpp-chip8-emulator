#pragma once

#include "util.hpp"
#include <iostream>
#include <string>

// #define LOG_VERBOSE
#define ENABLE_LOG_BUFFER

class Log {
  private:

#ifdef ENABLE_LOG_BUFFER
	static void rotateBuffer() {
        constexpr int maxBufferSize = 2000;

		if (buffer.length() > maxBufferSize) {
			buffer = buffer.substr(buffer.length() - maxBufferSize);
		}
	}

	template <typename T> static void append(T t) {
		buffer += t;
		rotateBuffer();
	}

	static void append(char const* t) {
		buffer += t;
		rotateBuffer();
	}


	static void append(int t) {
		buffer += std::to_string(t);
		rotateBuffer();
	}

    static void append(float t) {
		buffer += std::to_string(t);
		rotateBuffer();
	}

#endif

  public:
	static const char* COLOR_GRAY;
	static const char* COLOR_RED;
	static const char* COLOR_BLUE;
	static const char* COLOR_MAGENTA;
	static const char* COLOR_YELLOW;
	static const char* RESET;
	static const char* STYLE_BOLD;
	static const char* STYLE_UNDERLINE;

	static std::string buffer;

	template <typename... Args> static void info(Args... m) {
#ifdef ENABLE_LOG_BUFFER
		Log::append("Info: ");
		(Log::append(m), ...);
		Log::append("\n");
#endif

		// Print the log level colored
		std::cout << STYLE_BOLD << COLOR_MAGENTA << "Info:\t" << RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
	}

	template <typename... Args> static void error(Args... m) {
#ifdef ENABLE_LOG_BUFFER
		Log::append("Error: ");
		(Log::append(m), ...);
		Log::append("\n");
#endif

		// Print the log level colored
		std::cerr << STYLE_BOLD << COLOR_RED << "Error:\t" << RESET;
		(std::cerr << ... << m);
		std::cerr << std::endl;
	}

	template <typename... Args> static void warn(Args... m) {
#ifdef ENABLE_LOG_BUFFER
		Log::append("Warn: ");
		(Log::append(m), ...);
		Log::append("\n");
#endif

		// Print the log level colored
		std::cout << STYLE_BOLD << COLOR_YELLOW << "Warn:\t" << RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
	}

	// When the LOG_VERBOSE macro is undefined, this function will be empty, which will trigger a compiler warning, just
	// ignoring that :p
	DISABLE_WARNING_PUSH
	DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
	template <typename... Args> static void debug(Args... m) {
#ifdef LOG_VERBOSE
 #ifdef ENABLE_LOG_BUFFER
		Log::append("Debug: ");
		(Log::append(m), ...);
		Log::append("\n");
 #endif

		// Print the log level colored
		std::cout << STYLE_BOLD << COLOR_BLUE << "Debug:\t" << RESET;
		(std::cout << ... << m);
		std::cout << std::endl;
#endif
	}
	DISABLE_WARNING_POP
};
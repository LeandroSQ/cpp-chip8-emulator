#include "logger.hpp"

// Initialize the static variables
const char* Log::COLOR_GRAY = "\u001b[30;1m";
const char* Log::COLOR_RED = "\u001b[31m";
const char* Log::COLOR_BLUE = "\u001b[34m";
const char* Log::COLOR_MAGENTA = "\u001b[35m";
const char* Log::COLOR_YELLOW = "\u001b[33m";
const char* Log::RESET = "\u001b[0m";
const char* Log::STYLE_BOLD = "\u001b[1m";
const char* Log::STYLE_UNDERLINE = "\u001b[4m";

std::string Log::buffer = "";
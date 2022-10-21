#include "util.hpp"
#include <sstream>

std::string toHex(int n, int padding) {
	std::stringstream stream;
	stream << std::hex << n;
	std::string output = stream.str();
    std::transform(output.begin(), output.end(), output.begin(), ::toupper);

	// Pad the output with zeros, so it's always 4 characters long
	while (output.length() < padding)
		output = "0" + output;

	return output;
}

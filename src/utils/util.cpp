#include "util.hpp"
#include "logger.hpp"
#include <sstream>
#include <fstream>
#include <filesystem>

std::string toHex(int n, int padding) {
	std::stringstream stream;
	stream << std::hex << n;
	std::string output = stream.str();
	std::transform(output.begin(), output.end(), output.begin(), ::toupper);

	// Pad the output with zeros, so it's always 4 characters long
	while (output.length() < (size_t) padding) output = "0" + output;

	return output;
}

int8_t readFile(const char* path, uint8_t*& buffer, size_t& size) {
	Log::info("[IO] Reading file '", path, "'...");
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	// Check if the file was opened successfully
	if (!file.good()) {
		Log::error("[IO] Failed to open file: ", path);
		return -1;
	}

	// Get file size
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	Log::info("[IO] File size: ", fileSize, " bytes");
    size = fileSize;

	// Allocate memory for the data
	buffer = new uint8_t[fileSize];

	// Read the data
	if (!file.read((char*)buffer, fileSize)) {
		Log::error("[IO] Failed to read file: ", path);
		delete[] buffer;
		file.close();
		return -1;
	}

	// Close the file
	file.close();

	return 0;
}

std::string getFileName(const char* path) {
    return std::filesystem::path(path).stem();
}
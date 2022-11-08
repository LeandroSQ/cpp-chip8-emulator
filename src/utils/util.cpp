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

uint8_t* extractColorChannels(uint32_t color) {
    uint8_t* channels = new uint8_t[4];
    channels[0] = (color >> 24) & 0xFF;
    channels[1] = (color >> 16) & 0xFF;
    channels[2] = (color >> 8) & 0xFF;
    channels[3] = color & 0xFF;

    return channels;
}

float* extractColorChannelsF(uint32_t color) {
    float* channels = new float[4];
    channels[0] = (float) ((color >> 24) & 0xFF) / 255.0f;
    channels[1] = (float) ((color >> 16) & 0xFF) / 255.0f;
    channels[2] = (float) ((color >> 8) & 0xFF) / 255.0f;
    channels[3] = (float) (color & 0xFF) / 255.0f;

    return channels;
}

uint32_t combineColorChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

uint32_t combineColorChannelsF(float r, float g, float b, float a) {
    return ((uint8_t) (r * 255.0f) << 24) | ((uint8_t) (g * 255.0f) << 16) | ((uint8_t) (b * 255.0f) << 8) | (uint8_t) (a * 255.0f);
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
	Log::info("[IO] File size: ", (int) fileSize, " bytes");
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

DISABLE_WARNING_PUSH
DISABLE_WARNING_FORMAT_SECURITY
void ImGui::TextCentered(const char* fmt, ...) {
    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(fmt).x) / 2.0f + ImGui::GetStyle().ItemSpacing.x);
    ImGui::Text(fmt);
}

void ImGui::ColorButtonPicker(const char* label, uint32_t& ref) {
    ImGui::Text(label);
    ImGui::SameLine();
    ImGui::PushItemWidth(44.0f);// Define the Input width

    // Convert the color
    float* color = extractColorChannelsF(ref);
    if (ImGui::ColorButton(label, ImVec4(color[0], color[1], color[2], color[3]))) {
        // Force the color picker to be opened
        ImGui::OpenPopup(label);
    }

    if (ImGui::BeginPopupContextItem(label)) {
        ImGui::ColorPicker4(label, color);
        ImGui::EndPopup();

        ref = combineColorChannelsF(color[0], color[1], color[2], color[3]);
    }

    delete[] color;
}
DISABLE_WARNING_POP
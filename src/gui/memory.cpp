#include "memory.hpp"

#include <imgui.h>

const ImVec4 color(0.3f, 0.3f, 0.3f, 1.0f);
const ImVec4 zeroColor(0.17f, 0.17f, 0.17f, 1.0f);
const ImVec4 selectedColor(0.2f, 0.7f, 0.2f, 1.0f);

void UI::renderMemory(Emulator& emulator) {
    if (ImGui::Begin("Memory")) {
        // Calculate text size
        const float addressTextWidth = ImGui::CalcTextSize("0000:").x;
        const float colSize = ImGui::CalcTextSize("00 ").x;
        const float availableWidth = ImGui::GetContentRegionAvail().x;
        const float padding = ImGui::GetStyle().ItemSpacing.x * 1.1f;

        // Calculate how many columns can be drawn
        int amountPerRow = floorf(abs(availableWidth - addressTextWidth - padding) / (colSize) );
        if (amountPerRow < 4) amountPerRow = 4; // Minimum of 4 columns

        // Draw memory
        bool firstCol = true;
        for (int i = 0; i < emulator.memory.size; i++) {
            if (firstCol) {
                ImGui::Text("%04X:", i);
                ImGui::SameLine();
                firstCol = false;
            }

            // Read memory
            uint8_t value = emulator.memory.read(i);

            // If it is one of the two bytes that make up the opcode, highlight it
            if (i == emulator.cpu.programCounter || i == emulator.cpu.programCounter + 1) {
                // When drawing address pointed by PC, highlight it
                ImGui::PushStyleColor(ImGuiCol_Text, selectedColor);
            } else if (value <= 0) {
                // When drawing zeros, dim them
                ImGui::PushStyleColor(ImGuiCol_Text, zeroColor);
            } else {
                // Otherwise, draw normally
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            }

            ImGui::Text("%02X", value);

            // Wrap if needed
            if (i % amountPerRow != amountPerRow - 1 && i < emulator.memory.size - 1) {
                ImGui::SameLine();
            } else {
                firstCol = true;
            }

            ImGui::PopStyleColor();
        }


    }
    ImGui::End();
}

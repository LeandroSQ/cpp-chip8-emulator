#include "dissasembler.hpp"

#include <imgui.h>
#include "../emulator/disassembler.hpp"

const ImVec4 color(0.3f, 0.3f, 0.3f, 1.0f);
const ImVec4 selectedColor(0.2f, 0.7f, 0.2f, 1.0f);

void UI::renderDisassembler(App& app) {
    if (ImGui::Begin("Disassembler", NULL)) {
        for (auto it = app.disassembledData.begin(); it != app.disassembledData.end(); it++) {
            ImGui::Text("%04X:", it->address);

            ImGui::SameLine();

            // If it is one of the two bytes that make up the opcode, highlight it
            if (it->address >= app.emulator.cpu.programCounter && it->address < app.emulator.cpu.programCounter + 2) {
                ImGui::PushStyleColor(ImGuiCol_Text, selectedColor);
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            }

            ImGui::Text("%s", it->mnemonic.c_str());

            // Reset color
            ImGui::PopStyleColor();
        }
    }
    ImGui::End();
}
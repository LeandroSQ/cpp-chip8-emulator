#include "dissasembler.hpp"

#include <imgui.h>
#include "../emulator/disassembler.hpp"

const ImVec4 color(0.3f, 0.3f, 0.3f, 1.0f);

void UI::renderDisassembler(App& app) {
    if (ImGui::Begin("Disassembler", NULL)) {
        for (auto it = app.disassembledData.begin(); it != app.disassembledData.end(); it++) {
            ImGui::Text("%04X:", it->address);

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::Text("%s", it->name.c_str());
            ImGui::PopStyleColor();
        }
    }
    ImGui::End();
}
#include "input.hpp"

#include "../utils/util.hpp"
#include <imgui.h>
#include <imgui_internal.h>

constexpr float maxButtonSize = 75.0f;
const uint8_t keypad[16] = {
    0x1, 0x2, 0x3, 0xC,
    0x4, 0x5, 0x6, 0xD,
    0x7, 0x8, 0x9, 0xE,
    0xA, 0x0, 0xB, 0xF
};

void UI::renderInput(App& app) {

    if (ImGui::Begin("Input", NULL, ImGuiWindowFlags_NoScrollbar)) {
        // If the window is not docked, fix the size
        bool isDocked = ImGui::IsWindowDocked();
        if (!isDocked) ImGui::SetWindowSize(ImVec2(231.0f, 243.0f));

        const ImVec2 padding = ImGui::GetStyle().ItemSpacing;
        const ImVec2 available = ImGui::GetContentRegionAvail();
        const float size = min(min(available.x - padding.x * 2.5f, available.y - padding.y * 2.5f) / 4.0f, maxButtonSize);
        const ImVec2 spacing = ImVec2((available.x - size * 4.0f) / 3.0f, (available.y - padding.y * 2.5f - size * 4.0f) / 3.0f);
        const ImVec2 buttonSize(size, size);

        char label[2];

        ImGuiStyle& style = ImGui::GetStyle();

        for (uint8_t i = 0; i < 4; i++) {
            for (uint8_t j = 0; j < 4; j++) {
                // Translate the key index to the keypad index
                const uint8_t index = keypad[i * 4 + j];

                // Format the button label
                snprintf(label, sizeof(label), "%X", index);

                // Keeps buttons on the same row
                if (j > 0) ImGui::SameLine(0.0f, spacing.x);

                // If the key is pressed, change the color of the button
                bool isKeyDownByUser = app.emulator.input.isKeyDown(index);
                if (isKeyDownByUser) {
                    ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonActive]);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
                }

                ImGui::PushID(index);
                ImGui::Button(label, buttonSize);
                if (ImGui::IsItemActive()) {
                    app.emulator.input.setKey(index, true);
                } else if (ImGui::IsItemDeactivated()) {
                    app.emulator.input.setKey(index, false);
                }
                ImGui::PopID();

                // Reset color
                ImGui::PopStyleColor(3);
            }

            if (i < 3) {
                const float y = ImGui::GetCursorPosY();
                ImGui::SetCursorPosY(y + spacing.y);
            }
        }
    }

    ImGui::End();
}

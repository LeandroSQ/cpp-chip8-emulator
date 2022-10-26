#include "cpu.hpp"

#include <imgui.h>

inline void input(char* label, uint8_t* value) {
    ImGui::InputScalar(label, ImGuiDataType_U8, value, NULL, NULL, "%02X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_NoUndoRedo);
}

inline void input(char* label, uint16_t* value) {
    ImGui::InputScalar(label, ImGuiDataType_U16, value, NULL, NULL, "%04X", ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_NoUndoRedo);
}

inline void renderCPURegisters(Emulator& emulator) {
    constexpr unsigned long registerCount = sizeof(emulator.cpu.registers) / sizeof(emulator.cpu.registers[0]);

    char label[6];

    // Draw centered text
    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("CPU Registers").x) / 2.0f);
    ImGui::Text("CPU Registers");

    // Define the Input width
    ImGui::PushItemWidth(22.5f);
    float cursorX = 0.0f;

    // Padding
    ImGui::Separator();
    ImGui::NewLine();

    // Calculate how many registers can be drawn on a single line
    const float availableWidth = ImGui::GetContentRegionAvail().x;
    const float colSize = 22.5f * 2.6f;
    const int amountPerRow = availableWidth / colSize;

    for (unsigned long i = 0; i < registerCount; i++) {
        ImGui::Text("V%X:", (unsigned int) i);
        ImGui::SameLine();

        snprintf(label, sizeof(label), "##V%X", (unsigned int) i);
        input(label, &emulator.cpu.registers[i]);

        // Wrap if needed
        if (i % amountPerRow != amountPerRow - 1 && i < registerCount - 1) ImGui::SameLine();
    }

    // Special registers - SP
    ImGui::Text("SP:");
    ImGui::SameLine();
    snprintf(label, sizeof(label), "##SP");
    input(label, &emulator.cpu.stackPointer);

    // Special registers - DT
    ImGui::SameLine();
    ImGui::Text("DT:");
    ImGui::SameLine();
    snprintf(label, sizeof(label), "##DT");
    input(label, &emulator.cpu.delayTimer);

    // Special registers - ST
    ImGui::SameLine();
    ImGui::Text("ST:");
    ImGui::SameLine();
    snprintf(label, sizeof(label), "##ST");
    input(label, &emulator.cpu.soundTimer);

    // Define the Input width
    ImGui::PushItemWidth(36.0f);

    // Special registers - I
    ImGui::Text("I :");
    ImGui::SameLine();
    snprintf(label, sizeof(label), "##I ");
    input(label, &emulator.cpu.indexRegister);

    // Special registers - PC
    ImGui::SameLine();
    ImGui::Text("PC:");
    ImGui::SameLine();
    snprintf(label, sizeof(label), "##PC");
    input(label, &emulator.cpu.programCounter);
}

inline void renderCPUStack(Emulator& emulator) {
    constexpr unsigned long stackSize = sizeof(emulator.cpu.stack) / sizeof(emulator.cpu.stack[0]);

    // Draw centered text
    ImGui::NewLine();
    ImGui::NewLine();
    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("CPU Stack").x) / 2.0f);
    ImGui::Text("CPU Stack");
    // Padding
    ImGui::Separator();
    ImGui::NewLine();

    ImGui::BeginTable("stack", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 50);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 50);
    ImGui::TableSetupScrollFreeze(0, 1);
    ImGui::TableHeadersRow();

    for (uint16_t i = 0; i < stackSize; i++) {
        ImGui::TableNextRow();

        ImGui::TableNextColumn();
        ImGui::Text("%02X", i);

        ImGui::TableNextColumn();
        ImGui::Text("%04X", emulator.cpu.stack[i]);
    }

    ImGui::EndTable();
}

void UI::renderCPU(Emulator& emulator) {
    if (ImGui::Begin("CPU", NULL, ImGuiWindowFlags_NoCollapse)) {
        renderCPURegisters(emulator);
        renderCPUStack(emulator);
    }

    ImGui::End();
}

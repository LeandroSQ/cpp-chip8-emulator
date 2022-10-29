#include "log.hpp"

#include "../utils/logger.hpp"
#include <imgui.h>

void UI::renderLog() {
    if (ImGui::Begin("Log", NULL, ImGuiWindowFlags_NoCollapse)) {
        ImGui::TextUnformatted(Log::buffer.c_str());

        // Auto scroll to the bottom
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
    }

    ImGui::End();
}
#include "viewport.hpp"

#include <imgui.h>

void UI::renderViewport(Window& window) {
    // Disable padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    if (ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoCollapse)) {
        ImVec2 imageSize = ImVec2(window.logicalViewport.width, window.logicalViewport.height);
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        // Get title bar height
        float titleBarHeight = ImGui::GetWindowHeight() - ImGui::GetContentRegionAvail().y;

        ImGui::SetCursorPos(ImVec2(
            (viewportSize.x - imageSize.x) / 2.0f,
            (viewportSize.y - imageSize.y) / 2.0f + titleBarHeight
        ));

        ImGui::Image(
            (void*)window.frameBuffer, imageSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1)
        );
    }

    ImGui::End();

    // Re-enable padding
    ImGui::PopStyleVar();
}
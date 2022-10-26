#pragma once

#include "../app/app.hpp"
#include <imgui.h>
#include <implot.h>

namespace UI {

	// Hold the frame time history to be plotted
	constexpr int maxFrameTimeHistory = 60;
	float frameTimeHistory[maxFrameTimeHistory] = { 0.0f };
	int frameTimeHistoryIndex = 0;

	// Hold the last time the frame time history was updated
	uint32_t lastUpdateTime = 0;

	inline void renderOverlay(App& app) {
		// Check if one second has passed since the last update
		uint32_t currentTime = SDL_GetTicks();
		if (currentTime - lastUpdateTime > 1000) {
			// Update the frame time history
			frameTimeHistory[frameTimeHistoryIndex] = app.maxFrameTime;
			frameTimeHistoryIndex = (frameTimeHistoryIndex + 1) % maxFrameTimeHistory;

			// Update the last update time
			lastUpdateTime = currentTime;
		}

		// Draw window
		if (ImGui::Begin("Debug", NULL)) {
			ImGui::Text("FPS: %d", app.fps);
			ImGui::Text("Cycles/s: %d", app.cycles);
			ImGui::Text("Frame time (max): %.3f ms", app.maxFrameTime);

			ImGui::NewLine();

			// Plot the frame time history
            constexpr auto flags = ImPlotFlags_NoTitle | ImPlotFlags_NoMenus | ImPlotFlags_NoChild;
			if (ImPlot::BeginPlot("Max frame time", ImVec2(-1, 200), flags)) {
				// Setup axes
				ImPlot::SetupAxes("Frame", "Time (ms)");
				ImPlot::SetupAxesLimits(0, maxFrameTimeHistory, 0, 30, ImGuiCond_Always);

				// Plot the reference line
				float targetFrameTime = 1000.0f / (float)app.settings.frameRate;
				float referenceLine[2] = { targetFrameTime, targetFrameTime };
				ImPlot::PlotLine("Reference", referenceLine, 2, maxFrameTimeHistory);

				// Plot the frame time line
				ImPlot::PushStyleColor(
					ImPlotCol_Line,
					frameTimeHistory[frameTimeHistoryIndex] > targetFrameTime ? ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
																			  : ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
				);
				ImPlot::PlotLine("Frame time", frameTimeHistory, frameTimeHistoryIndex);
				ImPlot::PopStyleColor();

				ImPlot::EndPlot();
			}

			/* ImGui::PlotHistogram(
				"##Frame time",
				lastFrameTimeHistory,
				IM_ARRAYSIZE(lastFrameTimeHistory),
				historyIndex,
				"Frame time (ms)",
				0.05f,
				17.0f,
				ImVec2(0, 50)
			); */

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();
            ImGui::Text("ROM: %s", app.currentRom.c_str());

            // ImGui::Checkbox("Frame interpolation", &(app.settings.isFrameInterpolationEnabled));
            app.settings.isFrameInterpolationEnabled = true;

            if (ImGui::Button("Clear screen")) {
                app.emulator.renderer.fill(0x00);
            }
        }
	}

}; // namespace UI
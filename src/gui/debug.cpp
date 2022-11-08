#include "debug.hpp"

#include "../utils/util.hpp"
#include <imgui.h>
#include <implot.h>

// Hold the frame time history to be plotted
constexpr int maxFrameTimeHistory = 60;
float frameTimeHistory[maxFrameTimeHistory] = { 0.0f };
int frameTimeHistoryIndex = 0;

// Hold the last time the frame time history was updated
uint32_t lastUpdateTime = 0;
uint16_t maxFrameTime = 0;

void UI::renderOverlay(App& app) {
	// Check if one second has passed since the last update
	uint32_t currentTime = SDL_GetTicks();
	if (currentTime - lastUpdateTime > 1000) {
		// Update the frame time history
		frameTimeHistory[frameTimeHistoryIndex] = app.maxFrameTime;
		frameTimeHistoryIndex = (frameTimeHistoryIndex + 1) % maxFrameTimeHistory;

		// Update the maximum frame time, for resizing the chart
		if (app.maxFrameTime > maxFrameTime || frameTimeHistoryIndex <= 0) {
			maxFrameTime = app.maxFrameTime;
		}

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
			// Calculate the reference target frame time
			float targetFrameTime = 1000.0f / (float)app.settings.frameRate;
			float referenceLine[2] = { targetFrameTime, targetFrameTime };

			// Setup axes
			ImPlot::SetupAxes("Frame", "Time (ms)");
			ImPlot::SetupAxesLimits(
				0,
				maxFrameTimeHistory,
				0,
				maxFrameTime > targetFrameTime ? maxFrameTime : targetFrameTime * 1.5,
				ImGuiCond_Always
			);

			// Plot the reference line
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

		ImGui::TextCentered(("ROM: " + app.currentRom).c_str());
		ImGui::NewLine();

        ImGui::Text("Target FPS: ");
        ImGui::SameLine();
        ImGui::PushItemWidth(44.0f);// Define the Input width
        uint16_t temp = app.settings.frameRate;
        ImGui::InputScalar("##Target FPS", ImGuiDataType_U16, &temp, NULL, NULL, "%d", ImGuiInputTextFlags_CharsDecimal);
        app.settings.frameRate = max(temp, (uint16_t) 15u);

        ImGui::Text("Clock rate: ");
        ImGui::SameLine();
        ImGui::PushItemWidth(44.0f);// Define the Input width
        temp = app.settings.cpuClockSpeed;
        ImGui::InputScalar("##Clock rate", ImGuiDataType_U16, &temp, NULL, NULL, "%d", ImGuiInputTextFlags_CharsDecimal);
        app.settings.cpuClockSpeed = max(temp, (uint16_t) 100u);
        ImGui::NewLine();

        ImGui::Separator();
        ImGui::NewLine();

		ImGui::Checkbox("Frame interpolation", &(app.settings.isFrameInterpolationEnabled));
        ImGui::Text("Interpolation amount: ");
        ImGui::SameLine();

        ImGui::PushItemWidth(50.0f);// Define the Input width
        float decay = (float(app.settings.frameInterpolationDecay) / 255.0f) * 100.0f;
        ImGui::SliderFloat("##Interpolation amount", &decay, 0.0f, 100.0f, "%.0f%%", 1.0f);
        app.settings.frameInterpolationDecay = uint8_t(decay / 100.0f * 255.0f);
		ImGui::NewLine();

        ImGui::ColorButtonPicker("Foreground color:", app.settings.foregroundColor);
        ImGui::ColorButtonPicker("Background color:", app.settings.backgroundColor);

		if (ImGui::Button("Clear screen")) {
			app.emulator.renderer.fill(0x00);
		}
	}
}
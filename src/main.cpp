#include "window.hpp"
#include "renderer.hpp"
#include "logger.hpp"

int main(void) {
    Log::info("[App] Starting...");

    // Declare variables
    Window window;
    Renderer renderer;

    // Initialize the window
    if (window.init() < 0) return -1;

    bool isRunning = true;
    while (isRunning) {
        // Capture events
        window.pollEvents();

        // Stops execution if the window is trying to close
        if (window.isClosed) isRunning = false;

        // Start rendering the frame
        window.startFrame();
        renderer.render(window);
        window.endFrame();

        // Delay
        SDL_Delay(100);
        Log::info("[App] Frame");
    }

    Log::info("[App] Stopping...");

    return 0;
}
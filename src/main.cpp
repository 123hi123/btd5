#include "App.hpp"
#include "Core/Context.hpp"
#include <chrono>
#include <thread>


int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;
    context -> Setup();
    while (!context->GetExit()) {
        int target_fps = app.GetFPS();
        auto frame_duration = std::chrono::microseconds(1000000 / target_fps);
        auto frame_start = std::chrono::high_resolution_clock::now();
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;

            case App::State::UPDATE:
                app.Update();
                break;

            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
        auto frame_end = std::chrono::high_resolution_clock::now();
        auto elapsed = frame_end - frame_start;
        if (target_fps == 60) {
            if (elapsed < frame_duration) {
                std::this_thread::sleep_for(frame_duration - elapsed);
            }
        }
    }

    return 0;
}

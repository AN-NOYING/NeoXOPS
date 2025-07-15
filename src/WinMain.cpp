#include "System/Application.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    system::Application app;
    if (!app.Initialize(hInstance)) {
        return 1;
    }

    app.Run();

    return 0;
}
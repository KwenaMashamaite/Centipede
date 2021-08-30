#include "src/GameLoop/Game.h"

#ifdef NDEBUG
    #include "windows.h"
#endif

int main() {

    // Hide console window in release mode
#ifdef NDEBUG
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);
#endif

    centpd::Game centipedeGame{};
    centipedeGame.initialize();
    centipedeGame.start();

    return 0;
}

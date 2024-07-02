
#include "engine/GUI/ApplicationGUI.h"

extern ModLauncher::Application *ModLauncher::CreateApplication(int argc, char **argv);

bool g_ApplicationRunning = true;

namespace ModLauncher {
    int Main(int argc, char **argv) {
        while (g_ApplicationRunning) {
            // run app
        }
        return 0;
    }
}


#if defined(RELEASE_DIST)

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    return ModLauncher::Main(__argc,__argv);
}

#else
int main(int argc, char **argv) {
    return ModLauncher::Main(argc, argv);
}

#endif

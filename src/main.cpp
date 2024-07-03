
#include "engine/GUI/ApplicationGUI.h"

extern Marika::Application *Marika::CreateApplication(int argc, char **argv);

bool g_ApplicationRunning = true;


class MainLayer : public Marika::Layer {
public:
    virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImGui::Text("The monkeys will always win");
    }
};

Marika::Application *Marika::CreateApplication(int argc, char **argv) {
    Marika::ApplicationSpecification spec;
    spec.Name = "Marika";
    spec.CustomTitlebar = true;
    spec.CenterWindow = true;

    Marika::Application *app = new Marika::Application(spec);
    app->PushLayer<MainLayer>();

    return app;
}


namespace Marika {
    int Main(int argc, char **argv) {
        while (g_ApplicationRunning) {
            Marika::Application *app = Marika::CreateApplication(argc, argv);
            app->Run();
            delete app;
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
    return Marika::Main(argc, argv);
}

#endif

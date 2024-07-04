
#include "engine/GUI/ApplicationGUI.h"
#include "BaseUI/BaseUI.h"

extern Marika::Application *Marika::CreateApplication(int argc, char **argv);

bool g_ApplicationRunning = true;

static bool persistence = true;

class MainLayer : public Marika::Layer {
public:
    virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        BaseUI basedUi(windowSize, windowPos, drawList);


        basedUi.RenderBackground();
        ImVec2 textSize = ImGui::CalcTextSize("Marika");
        ImGui::SetCursorPos(ImVec2(windowSize.x / 2.0f - textSize.x / 2.0f, 10.0f));
        ImGui::Text("Marika");

        if (RenderLaunchButton(ImVec2(windowSize.x / 2.0f - 100.0f, windowSize.y / 2.0f - 50.0f), ImVec2(200.0f, 100.0f), "Launch")) {
            if (!persistence) {
                ImGui::OpenPopup("No persistence enabled");
            } else {
                // todo: Lauch and track exe to cleanup
            }
        }
        RenderConfirmPopup(windowSize);

        ImVec2 configButtonSize = ImGui::CalcTextSize("Remove Configuation");
        configButtonSize = ImVec2(configButtonSize.x + 15.0f, configButtonSize.y + 15.0f);
        if (RenderLaunchButton(ImVec2(windowSize.x - configButtonSize.x - 10.0f, windowSize.y - configButtonSize.y - 10.0f), configButtonSize, "Remove Configuation")) {
        }

        ImGui::SetCursorPos(ImVec2(15.0f, 375.0f));
        ImGui::Checkbox("Persistence", &persistence);
    }
};

Marika::Application *Marika::CreateApplication(int argc, char **argv) {
    Marika::ApplicationSpecification spec;
    spec.Name = "Marika";
    spec.CustomTitlebar = true;
    spec.Height = 400;
    spec.Width = 600;
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

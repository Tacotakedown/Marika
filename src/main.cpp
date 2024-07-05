
#include "engine/GUI/ApplicationGUI.h"
#include "BaseUI/BaseUI.h"
#include "Utils/DllWriter.h"
#include "Utils/Launcher.h"
#include "Utils/SteamUtils.h"
#include "files/steam_api64.hpp"
#include "files/steam_api64_official.hpp"

extern Marika::Application *Marika::CreateApplication(int argc, char **argv);

bool g_ApplicationRunning = true;

static bool persistence = true;

static float s_elapsedTime = 0.0f;

static std::chrono::steady_clock::time_point s_lastTimestamp = std::chrono::steady_clock::now();

class MainLayer : public Marika::Layer {
public:
    virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        auto start = std::chrono::steady_clock::now();

        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(start - s_lastTimestamp).count() / 1000.0f;

        s_lastTimestamp = start;

        std::wstring gameFolderPath = FindEldenRingPath();
        std::wstring gameLauncherPath = gameFolderPath + L"\\modengine2_launcher.exe";
        //std::wstring gameFolderPath = L"D:\\Games\\ELDEN RING Shadow of the Erdtree\\Game";
        //std::wstring gameLauncherPath = L"D:\\Games\\ELDEN RING Shadow of the Erdtree\\Game\\modengine2_launcher.exe";


        std::mutex mtx;
        static bool gameExited = false;
        static bool launchAvailable = true;

        STARTUPINFOW si = {sizeof(si)};
        PROCESS_INFORMATION pi;


        ImDrawList *drawList = ImGui::GetWindowDrawList();
        BaseUI basedUi(windowSize, windowPos, drawList);


        basedUi.RenderBackground();
        ImVec2 textSize = ImGui::CalcTextSize("Marika");
        ImGui::SetCursorPos(ImVec2(windowSize.x / 2.0f - textSize.x / 2.0f, 10.0f));
        ImGui::Text("Marika");


        ImGui::PushTextWrapPos(600.0f);
        ImVec2 cursorPos = ImGui::GetCursorPos();

        ImVec2 warningSize = ImGui::CalcTextSize("After closing the game, DO NOT close Marika until it says so");
        ImGui::SetCursorPos(ImVec2(windowSize.x / 2.0f - warningSize.x / 2.0f, 300.0f));
        ImGui::Text("After closing the game, DO NOT close Marika until it says so");
        ImGui::SetCursorPos(cursorPos);
        if (launchAvailable) {
            if (RenderLaunchButton(ImVec2(windowSize.x / 2.0f - 100.0f, windowSize.y / 2.0f - 50.0f), ImVec2(200.0f, 100.0f), "Launch")) {
                if (!persistence) {
                    ImGui::OpenPopup("No persistence enabled");
                } else {
                    launchAvailable = false;
                    std::wstring steamDllName = L"steam_api64.dll";
                    std::wstring renameTo = L"steam_api64.dll.steam";

                    DeleteDllFile(gameFolderPath, steamDllName);


                    if (!CreateDllFromMemory(gameFolderPath, L"steam_api64.dll", g_goldbergSteamEmulatorDLL, sizeof(g_goldbergSteamEmulatorDLL))) {
                        std::cerr << "Cant create dll";
                        launchAvailable = true;
                        return;
                    }

                    if (!CheckIntegrityOfModEngine(gameFolderPath)) {
                        std::cerr << "Invalid file structure";
                        launchAvailable = true;
                        return;
                    }

                    if (!CreateProcessW(gameLauncherPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                        std::cerr << "Failed to start game launcher: " << GetLastError() << std::endl;
                        launchAvailable = true;
                        return;
                    }

                    CloseHandle(pi.hThread);
                    CloseHandle(pi.hProcess);

                    std::thread gameMonitorThread(FindAndStartMonitor, std::ref(mtx), std::ref(gameExited));
                    gameMonitorThread.detach();
                }
            }
        } else {
            basedUi.RenderLoadingCircle(ImVec2(windowSize.x / 2.0f, 210.0f), 3.0f, 20.0f);
        }


        RenderConfirmPopup(windowSize, [&]() {
            DeleteDllFile(gameFolderPath, L"steam_api64.dll"); // we can really just ignore the res of this

            if (!CreateDllFromMemory(gameFolderPath, L"steam_api64.dll", g_goldbergSteamEmulatorDLL, sizeof(g_goldbergSteamEmulatorDLL))) {
                std::cerr << "Can't create dll";
                return;
            }

            if (!CheckIntegrityOfModEngine(gameFolderPath)) {
                std::cerr << "Invalid file structure";
                return;
            }

            if (!CreateProcessW(gameLauncherPath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                std::cerr << "Failed to start game launcher: " << GetLastError() << std::endl;
                return;
            }
        });
        static bool showSafeClose = false;

        if (showSafeClose) {
            ImVec2 closeMessageSize = ImGui::CalcTextSize("You may now close Marika");
            ImGui::SetCursorPos(ImVec2(windowSize.x / 2 - closeMessageSize.x / 2, 100.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7, 0.4, 0.4, 1.0));
            ImGui::Text("You may now close Marika");
            ImGui::PopStyleColor();
        }

        if (gameExited) {
            static bool stop = false;
            if (!stop) {
                DeleteDllFile(gameFolderPath, L"steam_api64.dll");
                CreateDllFromMemory(gameFolderPath, L"steam_api64.dll", g_officialSteamDll, sizeof(g_officialSteamDll));
                launchAvailable = true;
                showSafeClose = true;
            }

            stop = true;
        }


        ImVec2 configButtonSize = ImGui::CalcTextSize("Remove Configuation");
        configButtonSize = ImVec2(configButtonSize.x + 15.0f, configButtonSize.y + 15.0f);
        if (RenderLaunchButton(ImVec2(windowSize.x - configButtonSize.x - 10.0f, windowSize.y - configButtonSize.y - 10.0f), configButtonSize, "Remove Configuation")) {
            if (!gameExited) {
                DeleteDllFile(gameFolderPath, L"steam_api64.dll");
                CreateDllFromMemory(gameFolderPath, L"steam_api64.dll", g_officialSteamDll, sizeof(g_officialSteamDll));
            }
        }

        ImGui::SetCursorPos(ImVec2(15.0f, 375.0f));
        ImGui::Checkbox("Persistence", &persistence);


        if (s_elapsedTime > 5.0f) {
            s_elapsedTime = 0.0f;
            showSafeClose = false;
        } else {
            s_elapsedTime += elapsedTime;
        }
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

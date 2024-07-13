
#include "engine/GUI/ApplicationGUI.h"
#include "BaseUI/BaseUI.h"
#include "Utils/DllWriter.h"
#include "Utils/Launcher.h"
#include "Utils/SteamUtils.h"
#include "files/steam_api64_official.hpp"
#include "ini/ini.h"
#include <codecvt>
//#include "Utils/ModEngineTools.h"
#include "files/OnlineFix64.h"
#include "files/OnlineFixIni.h"
#include "toml/toml.hpp"

extern Marika::Application *Marika::CreateApplication(int argc, char **argv);

bool g_ApplicationRunning = true;

static float s_elapsedTime = 0.0f;

static std::chrono::steady_clock::time_point s_lastTimestamp = std::chrono::steady_clock::now();

static float launchButtonOffset = 0.0f;
static const float animationSpeed = 5.0f;


std::wstring getExecutableDirectory() {
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
    return std::wstring(buffer).substr(0, pos);
}

void saveConfig(std::wstring &path, bool persistence, bool steamInstall) {
    std::wstring exeDir = getExecutableDirectory();
    std::wstring configPath = exeDir + L"\\Marika.ini";

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string configPathString = converter.to_bytes(configPath);

    mINI::INIFile file(configPathString);
    mINI::INIStructure ini;

    ini["Settings"]["Path"] = converter.to_bytes(path);
    ini["Settings"]["Persistence"] = persistence ? "true" : "false";
    ini["Settings"]["steamInstall"] = steamInstall ? "true" : "false";

    file.write(ini);
}

std::string WStringToStr(const std::wstring &wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

void addOnlineFixDll(const std::wstring &gameFolderPath) {
    std::string tomlPathString = WStringToStr(gameFolderPath) + "\\config_eldenring.toml";

    try {
        toml::value mo2Config = toml::parse(tomlPathString);

        auto &external_dlls = toml::find<toml::array>(mo2Config, "modengine", "external_dlls");

        std::string new_dll = "OnlineFix64.dll";
        bool dll_exists = false;

        for (const auto &dll: external_dlls) {
            if (dll.as_string() == new_dll) {
                dll_exists = true;
                break;
            }
        }

        if (!dll_exists) {
            external_dlls.push_back(new_dll);

            std::ofstream ofs(tomlPathString);
            if (!ofs.is_open()) {
                std::cerr << "Could not open the file for writing!" << std::endl;
                return;
            }

            ofs << toml::format(mo2Config);
            ofs.close();
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


void loadConfig(std::wstring &path, bool &persistence, bool &steamInstall) {
    std::wstring exeDir = getExecutableDirectory();
    std::wstring configPath = exeDir + L"\\Marika.ini";

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::string configPathString = converter.to_bytes(configPath);

    std::ifstream file(configPathString);
    if (!file) {
        return;
    }

    mINI::INIFile iniFile(configPathString);
    mINI::INIStructure ini;
    iniFile.read(ini);

    if (ini.has("Settings")) {
        path = converter.from_bytes(ini["Settings"]["Path"]);
        persistence = (ini["Settings"]["Persistence"] == "true");
        steamInstall = (ini["Settings"]["steamInstall"] == "true");
    }
}


class MainLayer : public Marika::Layer {
public:
    virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        auto start = std::chrono::steady_clock::now();

        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(start - s_lastTimestamp).count() / 1000.0f;

        s_lastTimestamp = start;


        std::mutex mtx;
        static bool gameExited = false;
        static bool launchAvailable = true;
        static bool steamInstall = true;
        static bool persistence = true;
        static std::wstring path = L"";

        static bool configLoaded = false;
        if (!configLoaded) {
            loadConfig(path, persistence, steamInstall);
            configLoaded = true;
        }
        std::wstring gameFolderPath;
        std::wstring gameLauncherPath;
        if (steamInstall) {
            gameFolderPath = GetSteamPathW();
            gameLauncherPath = gameFolderPath + L"\\modengine2_launcher.exe";
        } else {
            gameFolderPath = path;
            gameLauncherPath = path + L"\\modengine2_launcher.exe";
        }


        STARTUPINFOW si = {sizeof(si)};
        PROCESS_INFORMATION pi;


        ImDrawList *drawList = ImGui::GetWindowDrawList();
        BaseUI basedUi(windowSize, windowPos, drawList);


        basedUi.RenderBackground();
        ImVec2 textSize = ImGui::CalcTextSize("Marika");
        ImGui::SetCursorPos(ImVec2(windowSize.x / 2.0f - textSize.x / 2.0f, 10.0f));
        ImGui::Text("Marika");

        // ImGui::Text("Game folder found at: %s", gameFolderPath.c_str());

        static bool settingsMenu = false;

        float settingsPositionX = basedUi.RenderSettingsMenu(settingsMenu, persistence, gameExited, gameFolderPath);


        if (basedUi.RenderSettingsIcon(ImVec2(10.0f, 50.0f), settingsMenu, false)) {
            if (settingsMenu) {
                settingsMenu = false;
            } else {
                settingsMenu = true;
            }
        }


        ImGui::PushTextWrapPos(600.0f);
        ImVec2 cursorPos = ImGui::GetCursorPos();

        ImVec2 warningSize = ImGui::CalcTextSize("After closing the game, DO NOT close Marika until it says so");
        ImGui::SetCursorPos(ImVec2(windowSize.x / 2.0f - warningSize.x / 2.0f, 300.0f));
        //ImGui::Text("After closing the game, DO NOT close Marika until it says so");
        ImGui::SetCursorPos(cursorPos);
        if (launchAvailable) {
            float offset = settingsMenu ? 120.0f : 0.0f;

            float targetOffset = offset;
            if (std::abs(launchButtonOffset - targetOffset) > 0.1f) {
                float delta = (targetOffset - launchButtonOffset) * animationSpeed * ImGui::GetIO().DeltaTime;
                if (std::abs(delta) < 0.1f) {
                    launchButtonOffset = targetOffset; // Snap to target if close enough
                } else {
                    launchButtonOffset += delta;
                }
            } else {
                launchButtonOffset = targetOffset;
            }

            if (RenderLaunchButton(ImVec2(windowSize.x / 2.0f - 100.0f + launchButtonOffset, windowSize.y / 2.0f - 50.0f), ImVec2(200.0f, 100.0f), "Launch")) {
                if (!persistence) {
                    ImGui::OpenPopup("No persistence enabled");
                } else {
                    launchAvailable = false;

                    addOnlineFixDll(gameFolderPath);

                    if (!CreateDllFromMemory(gameFolderPath, L"OnlineFix.ini", g_OnlineFixIni, sizeof(g_OnlineFixIni))) {
                        launchAvailable = true;
                        return;
                    }


                    if (!CreateDllFromMemory(gameFolderPath, L"OnlineFix64.dll", g_OnlineFix, sizeof(g_OnlineFix))) {
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
            basedUi.RenderLoadingCircle(ImVec2(windowSize.x / 2.0f + launchButtonOffset, 210.0f), 3.0f, 20.0f);

            ImGui::SetCursorPos(ImVec2(200.0f, 300.0f));
            ImGui::Text("Do not close Marika");
        }

        ImVec2 installModEngineSize = ImGui::CalcTextSize("Install Mod Engine 2");
        installModEngineSize = ImVec2(installModEngineSize.x + 15.0f, installModEngineSize.y + 15.0f);
        if (RenderLaunchButton(ImVec2(settingsPositionX - 230.0f, 180.0f), installModEngineSize, "Install Mod Engine 2")) {
            // unzipTestCase();
        }

        ImVec2 pasButtonSize = ImGui::CalcTextSize("Set ERSC Password");
        pasButtonSize = ImVec2(pasButtonSize.x + 15.0f, pasButtonSize.y + 15.0f);
        if (RenderLaunchButton(ImVec2(settingsPositionX - 230.0f, 220), pasButtonSize, "Set ERSC Password")) {
            std::wstring configPath = gameFolderPath + L"\\SeamlessCoop\\ersc_settings.ini";
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            std::string configPathString = converter.to_bytes(configPath);

            //std::string configPathString = "D:\\Games\\ELDEN RING Shadow of the Erdtree\\Game\\SeamlessCoop\\ersc_settings.ini";
            mINI::INIFile file(configPathString);

            mINI::INIStructure ini;

            file.read(ini);


            std::string &password = ini["PASSWORD"]["cooppassword"];
            std::cout << "Current password: " << password;

            if (password != "misfitsthugshake") {
                ini["PASSWORD"]["cooppassword"] = "misfitsthugshake";
                file.write(ini);
            }
        }


        ImVec2 saveButtonSize = ImGui::CalcTextSize("Save Settings");
        saveButtonSize = ImVec2(saveButtonSize.x + 15.0f, saveButtonSize.y + 15.0f);
        if (RenderLaunchButton(ImVec2(settingsPositionX - 230.0f, 370.0f), saveButtonSize, "Save Settings")) {
            saveConfig(path, persistence, steamInstall);
        }


        RenderConfirmPopup(windowSize, [&]() {
            DeleteDllFile(gameFolderPath, L"steam_api64.dll"); // we can really just ignore the res of this

            if (!CreateDllFromMemory(gameFolderPath, L"OnlineFix64.dll", g_OnlineFix, sizeof(g_OnlineFix))) {
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
                DeleteDllFile(gameFolderPath, L"OnlineFix64.dll");
                launchAvailable = true;
                showSafeClose = true;
            }

            stop = true;
        }


        ImGui::SetCursorPos(ImVec2(settingsPositionX - 230.0f, 140.0f));
        ImGui::Checkbox("Steam Install", &steamInstall);


        if (!steamInstall) {
            RenderPathSelectBox(ImVec2(settingsPositionX - 290.0f, 300.0f), ImVec2(280.0f, 40.0f), path);
        }


        ImVec2 configButtonSize = ImGui::CalcTextSize("Remove Configuration");
        configButtonSize = ImVec2(configButtonSize.x + 15.0f, configButtonSize.y + 15.0f);
        if (RenderLaunchButton(ImVec2(settingsPositionX - configButtonSize.x - 50.0f, 260.0f), configButtonSize, "Remove Configuration")) {
            if (!gameExited) {
                DeleteDllFile(gameFolderPath, L"OnlineFix64.dll");
            }
        }


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
    return Marika::Main(__argc, __argv);
}

#else

int main(int argc, char **argv) {
    return Marika::Main(argc, argv);
}

#endif

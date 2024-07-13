//
// Created by Taco on 7/3/2024.
//

#include "BaseUI.h"


#include <imgui_internal.h>
#include <unordered_map>

#include "engine/GUI/ApplicationGUI.h"

const std::unordered_map<std::string, ImVec2> defaultCirclePoisitons = {
        {"Circle1", ImVec2(100, 100)},
        {"Circle2", ImVec2(200, 200)},
        {"Circle3", ImVec2(300, 300)},
        {"Circle4", ImVec2(400, 400)},
        {"Circle5", ImVec2(200, 50)}
};

BaseUI::BaseUI(ImVec2 windowSize, ImVec2 windowPos, ImDrawList *drawList) : m_drawList(drawList),
                                                                            m_windowSize(windowSize),
                                                                            m_windowPos(windowPos) {


    if (s_CirclePos.empty()) {
        s_CirclePos.resize(5);
    }
}

void BaseUI::RenderBackground() {
    RenderBackgroundBaseLayer();
    RenderBackgroundGradientLayer();
    RenderBackgroundDotsLayer();
}

void BaseUI::RenderBackgroundBaseLayer() {
    m_drawList->AddRectFilled(m_windowPos, ImVec2(m_windowPos.x + m_windowSize.x, m_windowPos.y + m_windowSize.y), ImColor(0.0f, 0.0f, 0.0f, 1.0f));

    m_drawList->AddRectFilledMultiColor(m_windowPos, ImVec2(m_windowPos.x + m_windowSize.x, m_windowPos.y + m_windowSize.y), ImColor(0.0f, 0.0f / 255.0f, 0.0f / 255.0f, 0.2f),
                                        ImColor(0.0f, 0.0f / 255.0f, 0.0f / 255.0f, 0.2f), ImColor(237.0f / 255.0f, 183.0f / 255.0f, 59.0f / 255.0f, 0.2f),
                                        ImColor(237.0f / 255.0f, 183.0f / 255.0f, 59.0f / 255.0f, 0.2f));
}

void BaseUI::RenderGradientCircle(ImVec2 center, float radius, float maxOpacity, ImU32 color) {
    const int segments = 100;
    const int layers = 60;

    ImVec4 colorVec = ImGui::ColorConvertU32ToFloat4(color);

    for (int i = 0; i < layers; i++) {
        float layerRadius = radius * (layers - i) / layers;
        float alpha = colorVec.w * (i + 1) / layers;

        if (alpha > maxOpacity) {
            alpha = maxOpacity;
        }

        ImU32 layerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colorVec.x, colorVec.y, colorVec.z, alpha));

        m_drawList->AddCircleFilled(center, layerRadius, layerColor, segments);
    }
}

void BaseUI::TrySetDefaultPositions() {
    if (s_CirclePos.size() == 5) {
        int i = 0;
        for (const auto &pair: defaultCirclePoisitons) {
            InitializeCirclePosition(i++, pair.second);
            if (i >= 5) break;
        }
    }
}

void BaseUI::RenderBackgroundDotsLayer() {
    const ImVec2 offsetPosition = ImVec2(m_windowPos.x + 10, m_windowPos.y + 10);
    const ImVec2 dotCount2D = ImVec2(400, 400);
    const int radius = 1;
    const ImColor color = ImColor(1.0f, 1.0f, 1.0f, 0.3f);
    const float spacing = 20.0f;

    for (int y = 0; y < dotCount2D.y; y++) {
        for (int x = 0; x < dotCount2D.x; x++) {
            ImVec2 pos = ImVec2(offsetPosition.x + x * spacing, offsetPosition.y + y * spacing);
            m_drawList->AddCircleFilled(pos, radius, color);
        }
    }
}

void BaseUI::RenderBackgroundGradientLayer() {
    static float circle1angle = 0.0f;
    static float circle2angle = 0.0f;
    static float circle3angle = 0.0f;
    static float circle4angle = 0.0f;
    static float circle5angle = 0.0f;


    TrySetDefaultPositions();

    RenderGradientCircle(ImVec2(m_windowPos.x + s_CirclePos[0].x, m_windowPos.y + s_CirclePos[0].y), 300.0f, 0.01f, ImColor(175.0f / 255.0f, 127.0f / 255.f, 13.0f / 255.0f, 0.01f));
    RenderGradientCircle(ImVec2(m_windowPos.x + s_CirclePos[1].x, m_windowPos.y + s_CirclePos[1].y), 200.0f, 0.01f, ImColor(175.0f / 255.0f, 127.0f / 255.f, 13.0f / 255.0f, 0.01f));
    RenderGradientCircle(ImVec2(m_windowPos.x + s_CirclePos[2].x, m_windowPos.y + s_CirclePos[2].y), 300.0f, 0.01f, ImColor(175.0f / 255.0f, 127.0f / 255.f, 13.0f / 255.0f, 0.01f));
    RenderGradientCircle(ImVec2(m_windowPos.x + s_CirclePos[3].x, m_windowPos.y + s_CirclePos[3].y), 200.0f, 0.01f, ImColor(175.0f / 255.0f, 127.0f / 255.f, 13.0f / 255.0f, 0.01f));
    RenderGradientCircle(ImVec2(m_windowPos.x + s_CirclePos[4].x, m_windowPos.y + s_CirclePos[4].y), 300.0f, 0.01f, ImColor(175.0f / 255.0f, 127.0f / 255.f, 13.0f / 255.0f, 0.01f));

    if (circle1angle - 0.1f < 0.0f) {
        circle1angle = 360.0f;
    } else {
        circle1angle -= 0.1f;
    }
    if (circle2angle + 0.1f > 360) {
        circle2angle = 0;
    } else {
        circle2angle += 0.1f;
    }
    if (circle3angle - 0.1f < 0.0f) {
        circle3angle = 360.0f;
    } else {
        circle3angle -= 0.1f;
    }
    if (circle4angle + 0.1f > 360) {
        circle4angle = 0;
    } else {
        circle4angle += 0.1f;
    }
    if (circle5angle - 0.1f < 360) {
        circle5angle = 0;
    } else {
        circle5angle -= 0.1f;
    }

    s_CirclePos[0] = GetCircleCoords(200.0f, circle1angle, ImVec2(m_windowSize.x / 2 + 100.0f, m_windowSize.y / 2 + 200.0f));
    s_CirclePos[1] = GetCircleCoords(100.0f, circle2angle, ImVec2(m_windowSize.x / 2 + 100.0f, m_windowSize.y / 2 + 20.0f));
    s_CirclePos[2] = GetCircleCoords(45.0f, circle3angle, ImVec2(m_windowSize.x / 4 + 200.0f, m_windowSize.y / 4 + 100.0f));
    s_CirclePos[3] = GetCircleCoords(209.0f, circle4angle, ImVec2(m_windowSize.x / 4 + 0.0f, m_windowSize.y / 4 + 200.0f));
    s_CirclePos[4] = GetCircleCoords(381.0f, circle5angle, ImVec2(m_windowSize.x / 4 - 100.0f, m_windowSize.y / 4 + 00.0f));
}

bool RenderLaunchButton(ImVec2 pos, ImVec2 size, const char *label) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return false;
    }
    ImGuiContext &ctx = *GImGui;
    const ImGuiStyle &style = ctx.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, NULL);

    ImVec2 innerSize = size;
    if (innerSize.x <= 0.0f) {
        innerSize.x = labelSize.x + style.FramePadding.x * 2.0f;
    }
    if (innerSize.y <= 0.0f) {
        innerSize.y = labelSize.y + style.FramePadding.y * 2.0f;
    }

    const ImRect rect(pos, ImVec2(pos.x + innerSize.x, pos.y + innerSize.y));
    ImGui::ItemSize(rect, style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id)) {
        return false;
    }

    bool hovered, held;
    const bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held);

    if (held || hovered) {
        window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(0.3f, 0.3f, 0.3f, 0.5f), 8, NULL);
        window->DrawList->AddRect(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 8, NULL, 2.0f);
    } else {
        window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, 0.5f), 8, NULL);
        window->DrawList->AddRect(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 8, NULL, 2.0f);
    }


    ImGui::RenderTextClipped(ImVec2(rect.Min.x + style.FramePadding.x, rect.Min.y + style.FramePadding.y), ImVec2(rect.Max.x - style.FramePadding.x, rect.Max.y - style.FramePadding.y), label, NULL,
                             &labelSize, style.ButtonTextAlign, &rect);


    return pressed;
}

void RenderConfirmPopup(ImVec2 windowSize, const std::function<void()> &continueCallback) {
    ImVec2 zeroPoint = ImVec2(windowSize.x / 2.0f - (windowSize.x - 100.0f) / 2.0f, windowSize.y / 2.0f - 100.0f);
    ImGui::SetNextWindowPos(zeroPoint);
    ImVec2 popupSize = ImVec2(windowSize.x - 100.0f, windowSize.y - 200.0f);
    ImGui::SetNextWindowSize(popupSize);
    ImGui::PushTextWrapPos(windowSize.x - 20.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 2.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    if (ImGui::BeginPopup("No persistence enabled", ImGuiWindowFlags_NoMove)) {
        ImGui::TextWrapped(
                "Launching the game with persistence disabled will cause the launcher to not revert back to the official steam configuration automatically upon closing the game. In order to play on Steam, you will have to use the 'Remove Configuration' button");

        ImGui::SetCursorPos(ImVec2(zeroPoint.x, zeroPoint.y + 50.0f));
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetCursorPos(ImVec2(zeroPoint.x + 320.0f, zeroPoint.y + 50.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 0.8f));
        if (ImGui::Button("Continue")) {
            if (continueCallback) {
                continueCallback();
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor();

        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(2);
    ImGui::PopTextWrapPos();
    ImGui::PopStyleVar(2);
}


void BaseUI::RenderLoadingCircle(ImVec2 pos, float speed, float radius) {
    float time = ImGui::GetTime();
    float angle = speed * time;

    ImVec2 dotPos = ImVec2(pos.x + radius * std::cos(angle), pos.y + radius * std::sin(angle));

    float dotRadius = 5.0f;
    ImU32 dotColor = IM_COL32(255, 255, 255, 255); // White color, full opacity
    ImGui::GetWindowDrawList()->AddCircle(pos, radius, ImColor(0.0f, 0.0f, 0.0f, 0.4f), 0, dotRadius * 2.0f);
    ImGui::GetWindowDrawList()->AddCircleFilled(dotPos, dotRadius, dotColor);
}

bool BaseUI::RenderSettingsIcon(ImVec2 pos, bool state, bool resetAnimation) {
    auto settingsIconAnimationParams = SettingsIconAnimationParams{
            .Top = LineParams{.Start = ImVec2(pos.x + 10.0f, pos.y + 10.0f), .End = ImVec2(pos.x + 40.0f, pos.y + 10.0f)},
            .Middle = LineParams{.Start = ImVec2(pos.x + 10.0f, pos.y + 22.0f), .End = ImVec2(pos.x + 40.0f, pos.y + 22.0f)},
            .Bottom =  LineParams{.Start = ImVec2(pos.x + 10.0f, pos.y + 34.0f), .End = ImVec2(pos.x + 40.0f, pos.y + 34.0f)},
    };


    if (state) {
        settingsIconAnimationParams.Top.Start = ImVec2(pos.x + 10.0f, pos.y + 34.0f);
        settingsIconAnimationParams.Bottom.Start = ImVec2(pos.x + 10.0f, pos.y + 10.0f);
    }


    ImGui::GetWindowDrawList()->AddLine(settingsIconAnimationParams.Top.Start, settingsIconAnimationParams.Top.End, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
    if (!state) {
        ImGui::GetWindowDrawList()->AddLine(settingsIconAnimationParams.Middle.Start, settingsIconAnimationParams.Middle.End, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);

    }
    ImGui::GetWindowDrawList()->AddLine(settingsIconAnimationParams.Bottom.Start, settingsIconAnimationParams.Bottom.End, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
    ImGui::SetCursorPos(ImVec2(pos.x, pos.y));
    bool clicked;
    clicked = ImGui::InvisibleButton("SettingsMenu", ImVec2(50.0f, 50.0f));


    return clicked;

}

static float settingsMenuX = -10.0f;
static const float animationSpeed = 5.0f;


float BaseUI::RenderSettingsMenu(bool show, bool &persistence, bool gameExited, std::wstring gameFolderPath) {

    static bool previousShow = false;


    float targetX = show ? 300.0f : -10.0f;

    if (settingsMenuX != targetX) {
        float delta = (targetX - settingsMenuX) * animationSpeed * ImGui::GetIO().DeltaTime;
        if (std::abs(delta) < 0.1f) {
            settingsMenuX = targetX;
        } else {
            settingsMenuX += delta;
        }
    }

    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(-10.0f, 40.0f), ImVec2(settingsMenuX, 420.0f), ImColor(0.0f, 0.0f, 0.0f, 0.3f), 10.0f);

    ImGui::SetCursorPos(ImVec2(settingsMenuX - 190.0f, 60.0f));
    ImGui::Text("Settings");
    ImGui::GetWindowDrawList()->AddLine(ImVec2(settingsMenuX - 290.0f, 100.0f), ImVec2(settingsMenuX - 10.0f, 100.0f), ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
    ImGui::SetCursorPos(ImVec2(settingsMenuX - 230.0f, 110.0f));
    ImGui::Checkbox("Persistence", &persistence);


    previousShow = show;

    return settingsMenuX;
}

#include <Windows.h>
#include <ShlObj.h>

std::wstring OpenFolderDialog(const std::wstring &defaultPath) {
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog *pfd = NULL;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

        if (SUCCEEDED(hr)) {
            DWORD dwOptions;
            pfd->GetOptions(&dwOptions);
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

            IShellItem *psiFolder = NULL;
            hr = SHCreateItemFromParsingName(defaultPath.c_str(), NULL, IID_PPV_ARGS(&psiFolder));
            if (SUCCEEDED(hr)) {
                pfd->SetFolder(psiFolder);
                psiFolder->Release();
            }

            hr = pfd->Show(NULL);

            if (SUCCEEDED(hr)) {
                IShellItem *psiResult;
                hr = pfd->GetResult(&psiResult);

                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath = NULL;
                    hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr)) {
                        std::wstring ws(pszFilePath);
                        CoTaskMemFree(pszFilePath);
                        psiResult->Release();
                        pfd->Release();
                        CoUninitialize();

                        return ws;
                    }
                    psiResult->Release();
                }
            }
            pfd->Release();
        }
        CoUninitialize();
    }
    return std::wstring(L"");
}


std::string WStringToUTF8(const std::wstring &wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}


void RenderPathSelectBox(ImVec2 pos, ImVec2 size, std::wstring &path) {

    ImDrawList *drawList = ImGui::GetWindowDrawList();

    const ImVec2 textBoxSize = ImVec2(size.x - 50.0f, size.y);
    const ImRect rct(pos, ImVec2(textBoxSize.x + pos.x, textBoxSize.y + pos.y));

    drawList->AddRect(rct.Min, rct.Max, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 8, NULL, 2.0f);

    std::string appdataPathString = WStringToUTF8(path);

    if (RenderLaunchButton(ImVec2(pos.x - 45.0f + size.x, pos.y), ImVec2(40.0f, size.y), "0")) {
        std::wstring selectedPath = OpenFolderDialog(path);

        if (!selectedPath.empty()) {
            path = selectedPath;
        }
    }

    const ImVec2 labelSize = ImGui::CalcTextSize(appdataPathString.c_str(), NULL, NULL);

    ImGui::RenderTextClipped(ImVec2(pos.x + 8.0f, pos.y + 11.0f), ImVec2(pos.x - 16.0f + textBoxSize.x, pos.y + 11.0f + textBoxSize.y), appdataPathString.c_str(), NULL, &labelSize);
}
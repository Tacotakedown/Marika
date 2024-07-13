//
// Created by Taco on 7/3/2024.
//

#ifndef BASEUI_H
#define BASEUI_H

#include "imgui/imgui.h"
#include <vector>
#include <string>
#include <functional>

static std::vector<ImVec2> s_CirclePos;

bool RenderLaunchButton(ImVec2 pos, ImVec2 size, const char *label);

void RenderConfirmPopup(ImVec2 windowSize, const std::function<void()> &continueCallback);

void RenderPathSelectBox(ImVec2 pos, ImVec2 size, std::wstring &path);

struct LineParams {
    ImVec2 Start;
    ImVec2 End;
};
struct SettingsIconAnimationParams {
    LineParams Top;
    LineParams Middle;
    LineParams Bottom;
};


class BaseUI {
public:
    BaseUI(ImVec2 windowSize, ImVec2 windowPos, ImDrawList *drawList);

    void RenderBackground();

    void RenderBackgroundDotsLayer();

    void RenderBackgroundGradientLayer();

    void RenderBackgroundBaseLayer();

    void RenderGradientCircle(ImVec2 center, float radius, float maxOpacity, ImU32 color);

    void RenderLoadingCircle(ImVec2 pos, float speed, float radius);

    bool RenderSettingsIcon(ImVec2 pos, bool state, bool resetAnimation);

    float RenderSettingsMenu(bool show, bool &persistence, bool gameExited, std::wstring gameFolderPath); // return the x position of the window for animation of the children

private:
    ImDrawList *m_drawList;
    ImVec2 m_windowPos;
    ImVec2 m_windowSize;
    // 5 circles


    static void InitializeCirclePosition(int index, ImVec2 position) {
        if (index >= 0 && index < s_CirclePos.size() && s_CirclePos[index].x == 0 && s_CirclePos[index].y == 0) {
            s_CirclePos[index] = position;
        }
    }

    void TrySetDefaultPositions();


    ImVec2 GetCircleCoords(float radius, float theta, ImVec2 center) {
        float radians = (3.141592 / 180) * theta;
        float x = center.x + radius * cos(radians);
        float y = center.y + radius * sin(radians);

        return ImVec2(x, y);
    }
};


#endif //BASEUI_H

//
// Created by Taco on 7/2/2024.
//

#ifndef APPLICATIONGUI_H
#define APPLICATIONGUI_H

#include "engine/Layer.h"
#include "engine/GUI/Image.h"

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <functional>
#include <filesystem>

#include "imgui.h"
#include "vulkan/vulkan.h"

void check_vk_result(VkResult err);

struct GLFWwindow;

namespace ModLauncher {
    struct ApplicationSpecification {
        std::string Name = "Elden Ring Mod Launcher";
        uint32_t Width = 800;
        uint32_t Height = 800;

        std::filesystem::path IconPath;

        bool WindowResizable = true;
        bool CustomTitlebar = true;
        bool CenterWindow = true;
    };

    class Application {
    public:
        Application(const ApplicationSpecification &applicationSpecification = ApplicationSpecification());

        ~Application();

        static Application &Get();

        void Run();

        void SetMenubarCallback(const std::function<void()> &menubarCallback) { m_MenubarCallback = menubarCallback; }

        template<typename T>
        void PushLayer() {
            static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
            m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
        }

        void PushLayer(const std::shared_ptr<Layer> &layer) {
            m_LayerStack.emplace_back(layer);
            layer->OnAttach();
        }

        void Close();

        bool IsMaximized() const;

        std::shared_ptr<Image> GetApplicationIcon() const { return m_AppHeaderIcon; }

        float GetTime();

        GLFWwindow *GetWindowHandle() const { return m_WindowHandle; }
        bool IsTitleBarHovered() const { return m_TitleBarHovered; }

        static VkInstance GetInstance();

        static VkPhysicalDevice GetPhysicalDevice();

        static VkDevice GetDevice();

        static VkCommandBuffer GetCommandBuffer(bool begin);

        static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

        static void SubmitResourceFree(std::function<void()> &&func);

        static ImFont *GetFont(const std::string &name);

        template<typename Func>
        void QueueEvent(Func &&func) {
            m_EventQueue.push(func);
        }

    private:
        void Init();

        void Shutdown();

        void SetWindowIcon(GLFWwindow *window, const unsigned char *data, int size);

        void UI_DrawTitlebar(float &outTitlebarHeight);

        void UI_DrawMenubar();

    private:
        ApplicationSpecification m_Specification;
        GLFWwindow *m_WindowHandle = nullptr;
        bool m_Running = false;

        float m_TimeStep = 0.0f;
        float m_FrameTime = 0.0f;
        float m_LastFrameTime = 0.0f;

        bool m_TitleBarHovered = false;

        std::vector<std::shared_ptr<Layer> > m_LayerStack;
        std::function<void()> m_MenubarCallback;

        std::mutex m_EventQueueMutex;
        std::queue<std::function<void()> > m_EventQueue;


        std::shared_ptr<ModLauncher::Image> m_AppHeaderIcon;
        std::shared_ptr<ModLauncher::Image> m_IconClose;
        std::shared_ptr<ModLauncher::Image> m_IconMinimize;
        std::shared_ptr<ModLauncher::Image> m_IconMaximize;
        std::shared_ptr<ModLauncher::Image> m_IconRestore;
    };

    Application *CreateApplication(int argc, char **argv);
}

#endif //APPLICATIONGUI_H

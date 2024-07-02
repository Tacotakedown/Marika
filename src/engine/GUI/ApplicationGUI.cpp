//
// Created by Taco on 7/2/2024.
//

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32

#include "ApplicationGUI.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

#include <stdio.h>
#include <stdlib.h>
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "stb_image/stb_image.h"

#include <iostream>


extern bool g_ApplicationRunning;

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

static VkAllocationCallbacks *g_Allocator = nullptr;
static VkInstance g_Instance = VK_NULL_HANDLE;
static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice g_Device = VK_NULL_HANDLE;
static uint32_t g_QueueFamily = static_cast<uint32_t>(-1);
static VkQueue g_Queue = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;

static ImGui_ImplVulkanH_Window g_MainWindowData;
static int g_MinImageCount = 2;
static bool g_SwapChainRebuild = false;

static std::vector<std::vector<VkCommandBuffer> > s_AllocatedCommandBuffers;
static std::vector<std::vector<std::function<void()> > > s_ResourceFreeQueue;

static uint32_t s_CurrentFrameIndex = 0;

static std::unordered_map<std::string, ImFont *> s_Fonts;

static ModLauncher::Application *s_Instance = nullptr;

constexpr int FPS_CAP = 144;
constexpr double FRAME_DUATION = 1.0 / FPS_CAP;

void check_vk_result(const VkResult err) {
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}


#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
                                                   uint64_t object, size_t location, int32_t messageCode,
                                                   const char *pLayerPrefix, const char *pMessage, void *pUserData) {
    (void) flags;
    (void) object;
    (void) location;
    (void) messageCode;
    (void) pUserData;
    (void) pLayerPrefix;
    return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT



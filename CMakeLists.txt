cmake_minimum_required(VERSION 3.0)
project(Marika)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SRC_FILES
        "src/*.c"
        "src/*.cpp"
        "src/*.h"
        "src/*.hpp"
        "dependencies/glm/glm/*.c"
        "dependencies/glm/glm/*.cpp"
        "dependencies/glm/glm/*.h"
        "dependencies/glm/glm/*.hpp"
        "dependencies/stb_image/*.h"
)

set(IMGUI_SOURCES
        dependencies/imgui/imgui.cpp
        dependencies/imgui/imgui_draw.cpp
        dependencies/imgui/imgui_widgets.cpp
        dependencies/imgui/imgui_tables.cpp
        dependencies/imgui/misc/cpp/imgui_stdlib.cpp
        dependencies/imgui/backends/imgui_impl_glfw.cpp
        dependencies/imgui/backends/imgui_impl_vulkan.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb_image
        "C:/VulkanSDK/1.3.283.0/Include"
        ${IncludeDir.glm}
)

add_executable(Marika ${SRC_FILES}
        src/engine/Buffer/Buffer.h
        src/Utils/DllWriter.h
        src/engine/Serialization/BufferStream.cpp
        src/engine/Serialization/BufferStream.h
        src/engine/Serialization/FileStream.cpp
        src/engine/Serialization/FileStream.h
        src/engine/Serialization/StreamReader.cpp
        src/engine/Serialization/StreamReader.h
        src/engine/Serialization/StreamWriter.cpp
        src/engine/Serialization/StreamWriter.h
        src/Utils/StringUtils.cpp
        src/Utils/StringUtils.h
        src/engine/GUI/Image.cpp
        src/engine/GUI/Image.h
        src/engine/GUI/ApplicationGUI.cpp
        src/engine/GUI/ApplicationGUI.h
        src/engine/Layer.h
        src/engine/GUI/Fonts/Roboto-Bold.h
        src/engine/GUI/Fonts/Roboto-Italic.h
        src/engine/GUI/Fonts/Roboto-Regular.h
        src/engine/GUI/Images/windowIcons.h
        src/engine/GUI/UI/UI.cpp
        src/engine/GUI/UI/UI.h
        src/engine/GUI/ImGuiTheme/ImGuiTheme.h
        src/engine/GUI/ImGuiTheme/ImGuiTheme.cpp
        src/engine/GUI/Images/logo.h
        src/BaseUI/BaseUI.cpp
        src/BaseUI/BaseUI.h
        src/Utils/SteamUtils.h
        src/Utils/Launcher.h
        src/Utils/Console.cpp
        src/Utils/Console.h

)

target_sources(Marika PRIVATE ${IMGUI_SOURCES})

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/yaml-cpp)

add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/include)
target_link_libraries(GLFW INTERFACE glfw)


target_link_libraries(
        Marika PRIVATE
        yaml-cpp
        glfw
        ${Vulkan_LIBRARIES}
)

target_compile_definitions(Marika PRIVATE
        $<$<CONFIG:Release>:RELEASE_DIST>
)

target_link_options(Marika PRIVATE
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
        $<$<NOT:$<CONFIG:Release>>:/SUBSYSTEM:CONSOLE>
)
//
// Created by Taco on 7/2/2024.
//

#ifndef LAYER_H
#define LAYER_H

#include <imgui.h>

namespace Marika {
    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void OnAttach() {
        }

        virtual void OnDetach() {
        }

        virtual void OnUpdate(float ts) {
        }

        virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) {
        }
    };
}

#endif //LAYER_H

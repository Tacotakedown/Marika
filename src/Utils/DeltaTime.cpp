//
// Created by Taco on 7/9/2024.
//

#include "DeltaTime.h"

DeltaTime::DeltaTime() : m_lastTime(highResClock::now()) {}

float DeltaTime::getDeltaTime() {
    auto currentTime = highResClock::now();
    std::chrono::duration<float> deltaTime = currentTime - m_lastTime;
    m_lastTime = currentTime;
    return deltaTime.count();
}
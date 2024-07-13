//
// Created by Taco on 7/9/2024.
//

#ifndef MARIKA_DELTATIME_H
#define MARIKA_DELTATIME_H

#include <chrono>

typedef std::chrono::high_resolution_clock highResClock;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> timePointHighResClock;


class DeltaTime {


public:
    DeltaTime();

    float getDeltaTime();

private:
    timePointHighResClock m_lastTime;
};


#endif //MARIKA_DELTATIME_H

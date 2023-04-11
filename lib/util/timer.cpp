#include "timer.h"

#include <chrono>

void Timer::tic() {
    begin = std::chrono::steady_clock::now();
}

void Timer::toc() {
    end = std::chrono::steady_clock::now();
}

unsigned int Timer::elapsedMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}
#include "timer.h"

#include <chrono>

void Timer::tic() {
    begin = std::chrono::steady_clock::now();
}

void Timer::toc() {
    end = std::chrono::steady_clock::now();
}

double Timer::elapsedMilliseconds() const {
    return ((double) std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count()) / 10e6;
}
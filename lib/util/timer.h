#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
private:
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
public:
    void tic();
    void toc();
    unsigned int elapsedMilliseconds() const;
};

#endif
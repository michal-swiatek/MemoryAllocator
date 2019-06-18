#ifndef __TIMER__
#define __TIMER__

#include <chrono>

//  From https://stackoverflow.com/questions/728068/how-to-calculate-a-time-difference-in-c

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<std::chrono::microseconds>
            (clock_::now() - beg_).count(); }

private:
    typedef std::chrono::high_resolution_clock clock_;
    std::chrono::time_point<clock_> beg_;
};

#endif // __TIMER__

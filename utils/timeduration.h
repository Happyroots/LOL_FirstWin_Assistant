#ifndef TIMEDURATION_H
#define TIMEDURATION_H

#include <iostream>
#include <chrono>

class TimeDuration {
public:
    TimeDuration();

private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    bool running;

public:

    void start() ;
    void stop();
    double get_duration();
};


#endif // TIMEDURATION_H

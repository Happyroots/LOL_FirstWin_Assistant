#include "timeduration.h"

TimeDuration::TimeDuration()
    :running(false)
{
//    start();
}

void TimeDuration::start()
{
    start_time = std::chrono::steady_clock::now();
    running = true;
}

void TimeDuration::stop()
{
   end_time = std::chrono::steady_clock::now();
   running = false;
}

double TimeDuration::get_duration()
{
   if (running) {
       std::cerr << "计时尚未停止\n";
       return -1; // 返回负数表示错误
   }

   std::chrono::duration<double> elapsed_seconds = end_time - start_time;

   return elapsed_seconds.count();
}

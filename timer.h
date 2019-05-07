#ifndef MONKEYROOMS_TIMER_H
#define MONKEYROOMS_TIMER_H

#include <chrono>
#include <iomanip>
#include <sstream>
#include <vector>

using namespace std;
using namespace chrono;

class Timer {
private:
    steady_clock::time_point start_;
    static constexpr double ratio_ = (double)steady_clock::period::num / steady_clock::period::den;
public:
    Timer() : start_(steady_clock::now()) {};
    double getSeconds() { return (double)(steady_clock::now() - start_).count() * ratio_; }
    string getSecondsString() { return secondsToString(getSeconds()); }
    string secondsToString(double ss) { stringstream s; s << fixed << setprecision(1); s << ss; return s.str(); }
};

#endif //MONKEYROOMS_TIMER_H

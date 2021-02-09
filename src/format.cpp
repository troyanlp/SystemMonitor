#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long minutes = seconds / 60;
    long hours = minutes / 60;
    long secondsElapsed = int(seconds%60);
    minutes = int(minutes%60);
    string result = ParseTime(hours) + ":" + ParseTime(minutes) + ":" + ParseTime(secondsElapsed);
    return result;
}

string Format::ParseTime(long time){
    return time < 10 ? "0" + std::to_string(time) : std::to_string(time);
}
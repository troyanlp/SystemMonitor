#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long minutes = seconds / 60;
    long hours = minutes / 60;
    long secondsElapsed = int(seconds%60);
    minutes = int(minutes%60);
    string result = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(secondsElapsed);
    return result;
}
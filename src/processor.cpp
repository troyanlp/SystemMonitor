#include <thread>
#include <chrono>
#include "processor.h"
#include "linux_parser.h"


//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
float Processor::Utilization() {
    std::vector<std::string> systemState1 = LinuxParser::CpuUtilization();
    float const activeTime1 = LinuxParser::ActiveJiffies(systemState1);
    float const idleTime1 = LinuxParser::IdleJiffies(systemState1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   
    std::vector<std::string> systemState2 = LinuxParser::CpuUtilization();
    float const activeTime2 = LinuxParser::ActiveJiffies(systemState2);
    float const idleTime2 = LinuxParser::IdleJiffies(systemState2);

    float const activeTime  = activeTime2 - activeTime1;
    float const idleTime  = idleTime2 - idleTime1;
    float const totalTime = activeTime + idleTime;
    float const result = (activeTime / totalTime);

    return result;
}
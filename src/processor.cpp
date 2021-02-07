#include <thread>
#include <chrono>
#include "processor.h"
#include "linux_parser.h"


// TODO: Return the aggregate CPU utilization
//https://stackoverflow.com/questions/4184468/sleep-for-milliseconds
float Processor::Utilization() {
    std::vector<std::string> systemState1 = LinuxParser::CpuUtilization();
    float activeTime1 = LinuxParser::ActiveJiffies(systemState1);
    float idleTime1 = LinuxParser::IdleJiffies(systemState1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   
    std::vector<std::string> systemState2 = LinuxParser::CpuUtilization();
    float activeTime2 = LinuxParser::ActiveJiffies(systemState2);
    float idleTime2 = LinuxParser::IdleJiffies(systemState2);

    float  activeTime  = activeTime2 - activeTime1;
    float  idleTime  = idleTime2 - idleTime1;
    float  totalTime = activeTime + idleTime;
    float  result = (activeTime / totalTime);

    return result;
}
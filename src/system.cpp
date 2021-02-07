#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

#include <algorithm>

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    return cpu_; 
}

vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();
    int pids_size= pids.size();
    vector<int> aux_sort;
    for (auto i = 0; i<pids_size; i++){
        Process process = Process(pids[i]);
        processes_.push_back(process);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_;
}

std::string System::Kernel() {
    std::string os_kernel;
    os_kernel = LinuxParser::Kernel();
    return os_kernel;
}

float System::MemoryUtilization() {
    float mem_utilization;
    mem_utilization = LinuxParser::MemoryUtilization();
    return mem_utilization;
}

std::string System::OperatingSystem() {
    std::string os_name;  
    os_name = LinuxParser::OperatingSystem();
    return os_name;
}

int System::RunningProcesses() {
    int number_of_running_proc;
    number_of_running_proc = LinuxParser::RunningProcesses();
    return number_of_running_proc;
}

int System::TotalProcesses() {
    int total_number_of_proc;
    total_number_of_proc = LinuxParser::TotalProcesses();
    return total_number_of_proc;
}

long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}
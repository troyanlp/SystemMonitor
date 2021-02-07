#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int pid) : pid(pid) {}

int Process::Pid() {
    return pid;
}

float Process::CpuUtilization() {
    return LinuxParser::ActiveJiffies(pid);
}

string Process::Command() {
    return LinuxParser::Command(pid);
}

string Process::Ram() {
    return LinuxParser::Ram(pid);
}

string Process::User() {
    return LinuxParser::User(pid);
}

long int Process::UpTime() {
    return LinuxParser::UpTime(pid);
}

bool Process::operator<(Process const& a[[maybe_unused]]) const {
    return (this->pid > a.pid)? true : false;
}
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string line2;
  string key;
  float memTotal;
  float memFree;
  float memValue;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> memValue) {
        if (key == "MemTotal:") {
          memTotal = memValue;
        }
      }
      while (std::getline(filestream, line2)) {
        std::istringstream linestream2(line2);
        while (linestream2 >> key >> memValue) {
          if (key == "MemFree:") {
            memFree = memValue;
          }
        }
      }
    }
  }
  return (memTotal - memFree)/memTotal;
}

long LinuxParser::UpTime() {
  string line;
  float timer1, timer2;
  int timer12;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> timer1 >> timer2) {
        timer12 = int(timer1);
        return long(timer12);
      }
    }
  }
  return long(timer12);
}

long LinuxParser::Jiffies() {
  std::vector<std::string> values = LinuxParser::CpuUtilization();
  float activeTime = LinuxParser::ActiveJiffies(values);
  float idleTime = LinuxParser::IdleJiffies(values);  
  return activeTime+idleTime; 
}

long LinuxParser::ActiveJiffies(int pid) {
  string pidString;
  string line;
  string nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10,
  nr11, nr12, nr13, nr14, nr15,nr16, nr17, nr18, nr19, nr20,nr21, nr22;
  double totalTime, utime, stime, cutime, cstime, uptime, seconds,startTime, hertz, pidCpuUsage;
  pidString = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + "/" + pidString + kStatFilename);
  if(filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream_proc(line);
      while (linestream_proc >> nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10>>
      nr11>> nr12>> nr13>> nr14>> nr15>> nr16>> nr17>> nr18>> nr19>> nr20>>nr21>> nr22) {
        utime = std::stof(nr14);
        stime = std::stof(nr15);
        cutime = std::stof(nr16);
        cstime = std::stof(nr17);
        startTime = std::stof(nr22);
        uptime = LinuxParser::UpTime();
        hertz = sysconf(_SC_CLK_TCK);

        totalTime = utime + stime;
        totalTime = totalTime + cutime + cstime;
        seconds = uptime - (startTime/hertz);
        pidCpuUsage = (totalTime/hertz)/seconds; 
        
        return pidCpuUsage;      
        }
      }
   }
   return 0;
}

long LinuxParser::ActiveJiffies(std::vector<std::string> vector_jiffies) { 
  long active_jiffies_value = std::stof(vector_jiffies[0]) + std::stof(vector_jiffies[1]) + std::stof(vector_jiffies[2]) +
                            std::stof(vector_jiffies[5]) + std::stof(vector_jiffies[6]) + std::stof(vector_jiffies[7]) +
                            std::stof(vector_jiffies[8]) + std::stof(vector_jiffies[9]);
  return active_jiffies_value;
}

long LinuxParser::IdleJiffies(std::vector<std::string> vector_jiffies) {
  return (std::stof(vector_jiffies[3]) + std::stof(vector_jiffies[4]));
}

vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key, nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >>nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10;  
        if (key == "cpu") {
          vector <string> cpuVector;
          cpuVector.push_back(nr1);
          cpuVector.push_back(nr2);
          cpuVector.push_back(nr3);
          cpuVector.push_back(nr4);
          cpuVector.push_back(nr5);
          cpuVector.push_back(nr6);
          cpuVector.push_back(nr7);
          cpuVector.push_back(nr8);
          cpuVector.push_back(nr9);
          cpuVector.push_back(nr10);
         
          return cpuVector;
        }
    }
  }
  return {};
}

int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int totalProcesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> totalProcesses) {
        if (key == "processes") {
          return totalProcesses;
        }
      }
    }
  }
  return totalProcesses;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int runningProcesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> runningProcesses) {
        if (key == "procs_running") {
          return runningProcesses;
        }
      }
    }
  }
  return runningProcesses;
}

string LinuxParser::Command(int pid) {
  string line;
  string key;
  std::string pidString = std::to_string(pid); 
  std::ifstream filestream_proc(kProcDirectory + "/" + pidString + kCmdlineFilename);
  if (filestream_proc.is_open()) {
      while (std::getline(filestream_proc, line)) {
        std::istringstream linestream_proc(line);
        while (linestream_proc >> key ) {
            return key;            
        }
      }
  } 
 return string(); 
}

string LinuxParser::Ram(int pid) {
  string ram_value;
  string line;
  string key;
  float ram_value_f;
  std::string pidString = std::to_string(pid);
  
  std::ifstream filestream(kProcDirectory + "/" + pidString + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream>> key >> ram_value) {
        if (key == "VmSize:") {
          ram_value_f = std::stof(ram_value)*0.001024;
        }
      }
    }
    return std::to_string(ram_value_f).substr(0,7);
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string uid, nr2, nr3, nr4, nr5, nr6;
  string line;
  string key;
  string sPid = std::to_string(pid);

  std::ifstream filestream_proc(kPasswordPath);
  while (std::getline(filestream_proc, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream_proc(line);
    linestream_proc >> uid>>nr2>> nr3>>nr4>> nr5>>nr6;
    if(nr3 == sPid){  
      return uid;
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string nr1, nr2, nr3, nr4, nr1_return;
  string line;
  string key;
  std::string pidString = std::to_string(pid);
  std::ifstream filestream_proc(kProcDirectory + "/" + pidString + kStatusFilename);

  while (std::getline(filestream_proc, line)) {
    std::istringstream linestream_proc(line);
    while (linestream_proc >> key >> nr1 >>nr2 >>nr3 >>nr4 ) {
      if (key == "Uid:") {
        nr1_return = nr1;
      }
    }
  }
  nr1_return = LinuxParser::Uid(std::stoi(nr1_return));
  return nr1_return;
}

long LinuxParser::UpTime(int pid) {
  string nr1, nr2, nr3, nr4, nr5, nr6, nr7, nr8, nr9, nr10,nr11, nr12, nr13, nr14, nr15,nr16, nr17, nr18, nr19, nr20,nr21, nr22;
  string line;
  long proc_uptime;
  std::string pidString = std::to_string(pid);
  std::ifstream filestream_proc(kProcDirectory + "/" + pidString + kStatFilename);
  while (std::getline(filestream_proc, line)) {
    std::istringstream linestream_proc(line);
    while (linestream_proc >> nr1 >>nr2 >>nr3 >>nr4>> nr5>> nr6>> nr7>> nr8>> nr9>> nr10>>nr11>> nr12>> nr13>> nr14>> nr15>> nr16>> nr17>> nr18>> nr19>> nr20>>nr21>> nr22) {
      proc_uptime = long(std::stoi(nr22)/sysconf(_SC_CLK_TCK));
      return UpTime() - proc_uptime;
    }
  }
  return 0;
}
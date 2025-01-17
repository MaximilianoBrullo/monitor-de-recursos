#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include "httplib.h"

void getCPUUsage() {
    static long long prevIdleTime = 0, provTotalTime = 0;
    std::ifstream procStat("/proc/stat");//abro archivo /proc/stat
    std::string line;
    std::getline(procStat, line);
    procStat.close();//cierro archivo


    

    std::istringstream ss(line);//descompone la linea en palabras usando un flojo de entrada
    std::string cpu;
    long long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long long idleTime = idle + iowait;//tiempo del CPU inactivo
    long long totalTime = user + nice + system + idle + iowait + irq + softirq +steal;//tiempo total del cpu
    
    long long deltaIdle = idleTime - prevIdleTime;//delta de tiempo inactivo
    long long deltaTotal = totalTime - prevTotalTime;//delta de tiempo total

    prevIdleTime = idleTime;
    prevTotalTime = totalTime;
    
    return 100.0 * (1.0 - (double)deltaIdle / deltaTotal);//porcentaje de uso
} 

doubnle getMemoryUsage() {
    std::istream meInfo("/proc/meminfo");
    std::string line;
    long long totalMem = 0, freeMem = 0;

    while (std::getLine(memInfo, line)) {
        std::istringstream ss(line);
        std::string key;
        long long value;
        ss >> key >> value;

        if(key == "MemTotal:") totalMem = value;
        if(key == "MemFree") freeMem = value;
        if(totalMem > 0 && freeMem > 0) break;
   }
   
   memInfo.close();

   return 100.0 * (1,0 - (double) freeMem / totalMem);
}

void startServer() {
    httplib::Server svr;

    svr.Get("/metrics", [](const httplib::Request&, httplib::Response& res) {
        double cpuUsage = getCPUsage();
        double memoryUsage = getMemoryUsage();

        std::ostringstream metrics;
        metrics << "# HELP cpu_usage CPU usage percentage\n";
        metrics << "# TYPE cpu_usage gauge\n";
        metrics << "cpu_usage " << cpuUsage << "\n";
        metrice << "# HELP memory_usage Porcentaje de uso de la memoria RAM";
        metrics << "# TYPE memory_usage gauge\n";
        metrics << "memory_usage " << memoryUsage << "\n";

        res.set_content(metrics.str(), "test/plain");
    });

    std::cout << "Server running on http://localhost:8080/metrics\n";
    svr.listen("0.0.0.0", 8080);
}

int main() {
    startServer();
    return 0;
}
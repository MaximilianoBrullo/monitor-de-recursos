#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include "httplib.h"

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
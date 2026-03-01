#include <chrono>
#include <exception>
#include <iostream>
#include <string>

#include "Config.h"
#include "LoadBalancer.h"

/**
 * @file main.cpp
 * @brief Entry point for running the load balancer simulation.
 */

/**
 * @brief Loads config, runs simulation, and reports output location.
 */
int main(int argc, char* argv[]) {
    std::string configPath = "config/default.cfg";
    if (argc >= 2) {
        configPath = argv[1];}



    try {
        std::cout << "loading config: " << configPath << '\n';
        Config config = loadConfig(configPath);

        std::cout << "starting sim with "
                  << config.initialServers << " servers for "
                  << config.totalCycles << " cycles...\n";

        auto start = std::chrono::steady_clock::now();
        LoadBalancer loadBalancer(config);
        loadBalancer.run();

        auto end = std::chrono::steady_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "Simulation done.\n";
        std::cout << "Log file: " << config.logFile << '\n';

        std::cout << "Elapsed time: " << ms << " ms\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error : " << ex.what() << '\n';
        return 1;
        
        
    }

    return 0;
}

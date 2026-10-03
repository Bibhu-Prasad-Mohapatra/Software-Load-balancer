#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/common/config.hpp"
#include "../include/routing/router.hpp"
#include "../include/health/health_manager.hpp"

// Reusing parser from M4
LoadBalancerConfig parse_config(const std::string& config_path) {
    LoadBalancerConfig config;
    std::ifstream file(config_path);
    if (!file.is_open()) return config;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto delimiter_pos = line.find('=');
        if (delimiter_pos == std::string::npos) continue;
        std::string key = line.substr(0, delimiter_pos);
        std::string value = line.substr(delimiter_pos + 1);
        if (key == "listen_port") config.listen_port = std::stoi(value);
        else if (key == "health_check_interval") config.health_check_interval_ms = std::stoi(value);
        else if (key == "health_check_timeout") config.health_check_timeout_ms = std::stoi(value);
        else if (key == "failure_threshold") config.failure_threshold = std::stoi(value);
        else if (key == "recovery_threshold") config.recovery_threshold = std::stoi(value);
        else if (key == "algorithm") config.algorithm = value;
        else if (key == "backend") {
            std::stringstream ss(value);
            std::string id, ip, port_str;
            std::getline(ss, id, ',');
            std::getline(ss, ip, ',');
            std::getline(ss, port_str, ',');
            if (!id.empty() && !ip.empty() && !port_str.empty()) {
                config.backends.emplace_back(id, ip, std::stoi(port_str));
            }
        }
    }
    return config;
}

int main(int argc, char* argv[]) {
    std::string config_file = "config/load_balancer.conf";
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--config" && i + 1 < argc) {
            config_file = argv[++i];
        }
    }

    LoadBalancerConfig config = parse_config(config_file);
    if (config.backends.empty()) {
        std::cerr << "[ERROR] No backends configured. Exiting.\n";
        return 1;
    }

    std::cout << "[INFO] Configuration Loaded. Starting Health Manager...\n";
    
    // Milestone 6: Start Health Manager in background
    HealthManager health_manager(config);
    health_manager.start();

    // Milestone 5: Routing Test (Proxy/Epoll loop logic will be wired up in the next immediate step. 
    // Right now, this proves our threads run safely without colliding.)
    Router router;
    
    std::cout << "[INFO] Health Manager is pinging backends. Press Ctrl+C to terminate.\n";
    
    // Temporarily keeping the main thread alive to demonstrate health-checks firing.
    // In the next step, we replace this sleep loop with the EpollServer run loop.
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        BackendServer* selected = router.get_next_backend(config);
        if (selected) {
            std::cout << "[DEBUG] Router selected backend: " << selected->id << "\n";
        } else {
            std::cout << "[WARN] Router found NO healthy backends!\n";
        }
    }

    health_manager.stop();
    return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <csignal>
#include <thread>
#include "../include/common/config.hpp"
#include "../include/routing/router.hpp"
#include "../include/health/health_manager.hpp"
#include "../include/networking/epoll_server.hpp"

EpollServer* global_server = nullptr;
HealthManager* global_health = nullptr;

void signal_handler(int signum) {
    std::cout << "\n[INFO] SIGINT received. Shutting down gracefully...\n";
    if (global_server) global_server->stop();
    if (global_health) global_health->stop();
    std::cout << "[INFO] Cleanup complete. Exiting.\n";
    exit(0);
}

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
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGPIPE, SIG_IGN); // Prevent crashing on broken pipes

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

    std::cout << "[INFO] Starting Software-Defined Load Balancer\n";
    std::cout << "[INFO] Routing Algorithm: " << config.algorithm << "\n";
    
    Router router;
    
    HealthManager health_manager(config);
    global_health = &health_manager;
    health_manager.start();

    EpollServer epoll_server(config, router);
    global_server = &epoll_server;
    
    // This blocks and runs the epoll event loop until signal_handler triggers
    epoll_server.start();

    return 0;
}

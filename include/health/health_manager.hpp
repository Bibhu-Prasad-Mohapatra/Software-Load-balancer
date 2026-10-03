#pragma once
#include "../common/config.hpp"
#include <thread>
#include <atomic>

class HealthManager {
public:
    HealthManager(LoadBalancerConfig& config);
    ~HealthManager();
    
    void start();
    void stop();

private:
    void monitor_loop();
    bool check_server(const std::string& host, uint16_t port);

    LoadBalancerConfig& config;
    std::atomic<bool> running{false};
    std::thread worker_thread;
};

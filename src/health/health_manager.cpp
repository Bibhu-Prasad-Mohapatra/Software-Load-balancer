#include "../../include/health/health_manager.hpp"
#include <iostream>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

HealthManager::HealthManager(LoadBalancerConfig& cfg) : config(cfg) {}

HealthManager::~HealthManager() { stop(); }

void HealthManager::start() {
    running = true;
    worker_thread = std::thread(&HealthManager::monitor_loop, this);
    std::cout << "[INFO] Health Manager started.\n";
}

void HealthManager::stop() {
    running = false;
    if (worker_thread.joinable()) {
        worker_thread.join();
    }
}

bool HealthManager::check_server(const std::string& host, uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    // Set non-blocking for timeout handling
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    bool is_healthy = false;
    int res = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (res < 0) {
        if (errno == EINPROGRESS) {
            fd_set set;
            FD_ZERO(&set);
            FD_SET(sock, &set);
            struct timeval timeout;
            timeout.tv_sec = config.health_check_timeout_ms / 1000;
            timeout.tv_usec = (config.health_check_timeout_ms % 1000) * 1000;

            if (select(sock + 1, nullptr, &set, nullptr, &timeout) > 0) {
                int so_error;
                socklen_t len = sizeof(so_error);
                getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
                if (so_error == 0) is_healthy = true;
            }
        }
    } else {
        is_healthy = true;
    }

    close(sock);
    return is_healthy;
}

void HealthManager::monitor_loop() {
    while (running.load()) {
        for (auto& backend : config.backends) {
            if (!backend.enabled) continue;

            bool is_up = check_server(backend.host, backend.port);
            bool currently_healthy = backend.healthy.load();

            if (is_up) {
                backend.consecutiveFailures = 0;
                backend.consecutiveSuccesses++;
                if (!currently_healthy && backend.consecutiveSuccesses >= config.recovery_threshold) {
                    backend.healthy.store(true);
                    std::cout << "[INFO] Backend " << backend.id << " recovered and is now HEALTHY.\n";
                }
            } else {
                backend.consecutiveSuccesses = 0;
                backend.consecutiveFailures++;
                if (currently_healthy && backend.consecutiveFailures >= config.failure_threshold) {
                    backend.healthy.store(false);
                    std::cerr << "[ERROR] Backend " << backend.id << " marked UNHEALTHY.\n";
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(config.health_check_interval_ms));
    }
}

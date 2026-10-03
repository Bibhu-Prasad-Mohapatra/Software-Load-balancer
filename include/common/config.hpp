#pragma once
#include <string>
#include <vector>
#include <atomic>

struct BackendServer {
    std::string id;
    std::string host;
    uint16_t port;

    // We use atomic so the health check thread can update this safely
    // while the routing thread reads it.
    std::atomic<bool> healthy{true};
    bool enabled{true};

    std::atomic<uint32_t> activeConnections{0};
    std::atomic<uint64_t> totalRequests{0};
    std::atomic<uint64_t> failedRequests{0};

    int consecutiveFailures{0};
    int consecutiveSuccesses{0};

    // Need a copy constructor because atomics disable default copy
    BackendServer(const std::string& id, const std::string& host, uint16_t port)
        : id(id), host(host), port(port) {}
    
    BackendServer(const BackendServer& other)
        : id(other.id), host(other.host), port(other.port),
          healthy(other.healthy.load()), enabled(other.enabled),
          activeConnections(other.activeConnections.load()),
          totalRequests(other.totalRequests.load()),
          failedRequests(other.failedRequests.load()),
          consecutiveFailures(other.consecutiveFailures),
          consecutiveSuccesses(other.consecutiveSuccesses) {}
};

struct LoadBalancerConfig {
    uint16_t listen_port = 9000;
    int health_check_interval_ms = 2000;
    int health_check_timeout_ms = 500;
    int failure_threshold = 3;
    int recovery_threshold = 2;
    std::string algorithm = "round_robin";
    std::vector<BackendServer> backends;
};

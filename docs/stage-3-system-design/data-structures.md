# Data Structures & Core Classes

## 1. Backend Server Representation

```cpp
struct BackendServer {
    std::string id;             // e.g., "server1"
    std::string host;           // e.g., "127.0.0.1"
    uint16_t port;              // e.g., 8001

    std::atomic<bool> healthy;  // Atomic for thread-safe read/write by Health Manager
    bool enabled;               // Admin toggle

    std::atomic<uint32_t> activeConnections; 
    std::atomic<uint64_t> totalRequests;
    std::atomic<uint64_t> failedRequests;

    int consecutiveFailures;    // Counter for UNHEALTHY transition
    int consecutiveSuccesses;   // Counter for HEALTHY transition
};
```

## 2. Connection Tracking Context
When `epoll` signals a socket is ready, we need context to know if it's a client or backend, and who they are talking to.

```cpp
enum class SocketType {
    CLIENT,
    BACKEND
};

struct ConnectionContext {
    int client_fd;
    int backend_fd;
    SocketType type;
    BackendServer* assigned_backend;
    std::chrono::time_point<std::chrono::steady_clock> connected_at;
};
```

## 3. Configuration Map
```cpp
struct LoadBalancerConfig {
    uint16_t listen_port = 9000;
    int health_check_interval_ms = 2000;
    int health_check_timeout_ms = 500;
    int failure_threshold = 3;
    int recovery_threshold = 2;
    std::string algorithm = "round_robin";
    std::vector<BackendServer> backends;
};
```

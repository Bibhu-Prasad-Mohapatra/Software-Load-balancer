#pragma once
#include "../common/config.hpp"
#include "../routing/router.hpp"
#include <unordered_map>
#include <atomic>

class EpollServer {
public:
    EpollServer(LoadBalancerConfig& config, Router& router);
    ~EpollServer();
    
    void start();
    void stop();

private:
    void accept_clients();
    void proxy_data(int source_fd);
    void close_connection(int fd);

    LoadBalancerConfig& config;
    Router& router;
    int listen_fd;
    int epoll_fd;
    std::atomic<bool> running{false};

    // Maps client <-> backend sockets
    std::unordered_map<int, int> fd_map;
    std::unordered_map<int, BackendServer*> backend_map;
};

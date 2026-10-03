#include "../../include/networking/epoll_server.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

#define MAX_EVENTS 1024

EpollServer::EpollServer(LoadBalancerConfig& cfg, Router& rtr) 
    : config(cfg), router(rtr), listen_fd(-1), epoll_fd(-1) {}

EpollServer::~EpollServer() { stop(); }

void set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void EpollServer::start() {
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        std::cerr << "[ERROR] Could not create listen socket.\n";
        return;
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(config.listen_port);

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[ERROR] Bind failed on port " << config.listen_port << ".\n";
        return;
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
        std::cerr << "[ERROR] Listen failed.\n";
        return;
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "[ERROR] epoll_create1 failed.\n";
        return;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
        std::cerr << "[ERROR] epoll_ctl failed for listen_fd.\n";
        return;
    }

    running = true;
    std::cout << "[INFO] Load Balancer Core running on port " << config.listen_port << "\n";

    struct epoll_event events[MAX_EVENTS];
    while (running.load()) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000); // 1 sec timeout for clean shutdown check
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == listen_fd) {
                accept_clients();
            } else {
                proxy_data(events[i].data.fd);
            }
        }
    }
}

void EpollServer::accept_clients() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) return;

    BackendServer* backend = router.get_next_backend(config);
    if (!backend) {
        std::cerr << "[WARN] Dropping connection: No healthy backends available.\n";
        close(client_fd);
        return;
    }

    int backend_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in b_addr;
    b_addr.sin_family = AF_INET;
    b_addr.sin_port = htons(backend->port);
    inet_pton(AF_INET, backend->host.c_str(), &b_addr.sin_addr);

    // Simple blocking connect for proxy reliability
    if (connect(backend_fd, (struct sockaddr*)&b_addr, sizeof(b_addr)) < 0) {
        close(client_fd);
        close(backend_fd);
        return;
    }

    set_non_blocking(client_fd);
    set_non_blocking(backend_fd);

    fd_map[client_fd] = backend_fd;
    fd_map[backend_fd] = client_fd;
    backend_map[client_fd] = backend;
    backend_map[backend_fd] = backend;

    backend->activeConnections++;
    backend->totalRequests++;

    struct epoll_event ev_client;
    ev_client.events = EPOLLIN | EPOLLET;
    ev_client.data.fd = client_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev_client);

    struct epoll_event ev_backend;
    ev_backend.events = EPOLLIN | EPOLLET;
    ev_backend.data.fd = backend_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, backend_fd, &ev_backend);
}

void EpollServer::proxy_data(int source_fd) {
    if (fd_map.find(source_fd) == fd_map.end()) return;
    int target_fd = fd_map[source_fd];

    char buffer[4096];
    ssize_t bytes_read = read(source_fd, buffer, sizeof(buffer));

    if (bytes_read > 0) {
        send(target_fd, buffer, bytes_read, 0);
    } else if (bytes_read == 0 || (bytes_read < 0 && errno != EAGAIN)) {
        close_connection(source_fd);
    }
}

void EpollServer::close_connection(int fd) {
    if (fd_map.find(fd) == fd_map.end()) return;
    
    int target_fd = fd_map[fd];
    
    if (backend_map.find(fd) != backend_map.end()) {
        BackendServer* backend = backend_map[fd];
        backend->activeConnections--;
        backend_map.erase(fd);
        backend_map.erase(target_fd);
    }

    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, target_fd, nullptr);
    
    close(fd);
    close(target_fd);
    
    fd_map.erase(fd);
    fd_map.erase(target_fd);
}

void EpollServer::stop() {
    running = false;
    if (listen_fd != -1) close(listen_fd);
    if (epoll_fd != -1) close(epoll_fd);
}

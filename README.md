# Software-Defined Load Balancer & Health-Check Router

**A Linux-Based C++ Software Load Balancer with Automated Health Monitoring, Fault Detection, Dynamic Routing, and Failover**

## Academic Project - Final Release
This project is an end-to-end implementation of a software-based load balancer designed for Linux user-space. It demonstrates advanced concepts in C++ system programming, TCP/IP networking, socket management (`epoll`), thread synchronization, and fault-tolerant system design.

## Features
- **epoll I/O Multiplexing:** Highly scalable non-blocking event loop.
- **Routing Algorithms:** Round Robin & Least Connections.
- **Active Health Checks:** Background thread constantly polling backend state.
- **Automatic Failover & Recovery:** Instantly bypasses dead nodes and reintroduces recovered nodes.
- **Graceful Shutdown:** `SIGINT` interception for clean resource teardown.

## Build Requirements
- Linux (Ubuntu / WSL2 recommended)
- GCC / G++ (C++17)
- CMake

## Compilation
```bash
mkdir build && cd build
cmake ..
make
```

## Running the Demonstration
1. Start the backends: `./scripts/start_servers.sh`
2. Run the load balancer: `./build/load_balancer --config config/load_balancer.conf`
3. Generate traffic: `./scripts/run_test.sh`
4. Test failure detection: `./scripts/kill_server.sh server2`
5. Cleanup: `./scripts/stop_servers.sh`

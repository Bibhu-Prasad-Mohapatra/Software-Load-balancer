# Final Project Report: Software-Defined Load Balancer

## 1. Abstract
This report details the design, implementation, and testing of a software-defined load balancer written in C++ for Linux environments. The project successfully demonstrates socket programming, I/O multiplexing via `epoll`, multithreading, and dynamic fault tolerance.

## 2. Introduction & Problem Statement
Modern architectures require a robust mechanism to distribute incoming traffic across multiple backend servers. Relying on a single server leads to performance bottlenecks and single points of failure. The objective of this project was to build a transparent proxy that intelligently routes TCP traffic while automatically detecting and bypassing failed backend instances.

## 3. Architecture & Design
The system is bifurcated into two major asynchronous components:
- **Health Manager:** A detached thread that pings all configured backend servers at 2-second intervals, updating an atomic `healthy` flag.
- **Epoll Core:** A non-blocking event loop using Linux `epoll` that accepts client connections, queries the Router for a healthy backend, and proxies the raw byte streams.

## 4. Algorithms Implemented
1. **Round Robin:** Evenly distributes sequential requests across all active servers.
2. **Least Connections:** Dynamically evaluates the `activeConnections` atomic counter on each backend to send new clients to the least-burdened server.

## 5. Linux & C++ Concepts Utilized
- **POSIX Sockets:** `socket()`, `bind()`, `listen()`, `accept()`, `connect()`.
- **I/O Multiplexing:** `epoll_create1()`, `epoll_ctl()`, `epoll_wait()`.
- **C++17 Features:** `std::atomic` for lock-free state sharing, `std::thread`, RAII for resource management.
- **Signals:** Graceful shutdown using `SIGINT` interception.

## 6. Testing & Results
- **Failure Detection:** Using `kill_server.sh`, we proved the LB isolates a dead server within 6 seconds (3 failures x 2s). 
- **Traffic Redistribution:** Remaining requests seamlessly mapped to surviving nodes.
- **Recovery:** Restarting the killed server resulted in the Health Manager restoring its `healthy` status and resuming traffic delivery.

## 7. Limitations & Future Scope
- **Limitations:** The current proxy is Layer 4 (TCP) only. It does not parse HTTP headers or terminate SSL/TLS.
- **Future Scope:** Adding eBPF integration for ultra-low latency kernel routing, and full Layer 7 HTTP parsing.

## 8. Conclusion
The project successfully meets all functional and non-functional requirements. It stands as a practical demonstration of advanced Linux system programming and fault-tolerant network engineering.

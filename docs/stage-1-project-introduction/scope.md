# Scope

## Included Scope
- **Core Load Balancer:** A user-space C++ Linux application that manages incoming TCP client connections and routes them to backends.
- **Algorithms:** Implementation of Round Robin and Least Connections routing strategies.
- **Health-Check Manager:** An asynchronous subsystem that probes backend availability and manages state transitions.
- **Configuration Management:** A mechanism to read port bindings, backend pools, and health thresholds from a local configuration file.
- **Custom Backend Simulator:** A lightweight C++ server to emulate backend processes with configurable artificial latency.
- **Custom Client Simulator:** A lightweight C++ client to generate concurrent load.
- **Concurrency:** Usage of `epoll` and multithreading to manage multiple simultaneous socket connections.
- **Graceful Lifecycle Management:** Catching Linux signals (SIGINT/TERM) for proper resource cleanup.

## Excluded Scope (Limitations)
- **Layer 7 Application Routing:** The load balancer will operate primarily at the Transport Layer (Layer 4 TCP). It will not parse HTTP headers, manage SSL/TLS termination, or do URL-based routing.
- **Kernel-Space Drivers:** While Linux network concepts are utilized, a custom Linux kernel module or device driver is excluded to keep the project focused on user-space systems programming.
- **Graphical Web Dashboard:** All monitoring and configuration will be handled via CLI and log outputs. No web UI will be developed.
- **Physical Hardware Infrastructure:** The project assumes a virtualized or single-host Linux environment; no hardware switches or external physical nodes are required.

## Future Scope
- Support for Layer 7 load balancing (HTTP parsing).
- Implementation of Weighted Round Robin or Dynamic Latency-based routing.
- SSL/TLS termination support using OpenSSL.
- A REST API for dynamic configuration without restarting the load balancer.
- EBPF (Extended Berkeley Packet Filter) integration for ultra-low latency packet manipulation closer to the kernel space.

# Software-Defined Load Balancer & Health-Check Router

**A Linux-Based C++ Software Load Balancer with Automated Health Monitoring, Fault Detection, Dynamic Routing, and Failover**

## Academic Project
This project is an end-to-end implementation of a software-based load balancer designed for Linux user-space. It demonstrates advanced concepts in C++ system programming, TCP/IP networking, socket management (`epoll`), thread synchronization, and fault-tolerant system design.

*(Note: Currently in Stage 1 of Development)*

## Directory Structure Overview
- `docs/` - Academic reporting, architecture, and diagrams across 6 stages.
- `src/` - Core Load Balancer source files.
- `include/` - C++ header files.
- `backend/` - Configurable backend server simulator.
- `client/` - Configurable client load simulator.
- `config/` - System configuration files.
- `scripts/` - Test automation and orchestration scripts.
- `tests/` - Unit, integration, and system tests.

## Build Requirements
- Linux (Ubuntu / WSL2 recommended)
- GCC / G++ (C++17 or higher)
- CMake
- Git

# Project Requirements Document (PRD)

## 1. Functional Requirements (FR)
These define the specific behaviors and functions the load balancer must perform.

- **FR-01 (Client Connection):** The system must accept incoming TCP connections from multiple concurrent clients.
- **FR-02 (Backend Pool Management):** The system must maintain a configurable pool of backend server addresses (IP and Port) loaded at startup.
- **FR-03 (Health Checks):** The system must asynchronously poll configured backend servers at a defined interval to determine availability.
- **FR-04 (Failure Detection):** The system must mark a backend as `UNHEALTHY` after it fails a configurable number of consecutive health checks.
- **FR-05 (Backend Selection):** The system must select a `HEALTHY` backend for each incoming client request using a specified algorithm (Round Robin or Least Connections).
- **FR-06 (Request Forwarding):** The system must seamlessly relay data from the client to the chosen backend, and relay the backend's response back to the client.
- **FR-07 (Backend Recovery):** The system must continue health-checking `UNHEALTHY` servers and mark them as `HEALTHY` (reintroducing them to the routing pool) once they pass a configurable number of consecutive checks.
- **FR-08 (Logging):** The system must write structured logs to the console and/or a log file, categorizing events by severity (INFO, WARN, ERROR, DEBUG).
- **FR-09 (Statistics & Monitoring):** The system must track and report real-time statistics (active connections, total requests, failed requests) via a console status command or log output.
- **FR-10 (Graceful Shutdown):** The system must intercept `SIGINT` and `SIGTERM` signals to cleanly close all open file descriptors, terminate threads safely, and dump final statistics before exiting.

## 2. Non-Functional Requirements (NFR)
These define system attributes such as performance, reliability, and maintainability.

- **NFR-01 (Performance & Scalability):** The system should utilize Linux `epoll` to multiplex socket I/O efficiently, allowing it to handle a high volume of concurrent clients (e.g., 100+) without the overhead of a thread-per-connection model.
- **NFR-02 (Reliability):** The system must not crash if a client abruptly disconnects, sends malformed data, or if a backend drops the connection mid-transfer.
- **NFR-03 (Maintainability):** The C++ codebase must adhere to Object-Oriented principles, utilizing RAII (Resource Acquisition Is Initialization) for socket and memory management to prevent leaks.
- **NFR-04 (Portability):** The application must compile and run on standard Linux distributions (e.g., Ubuntu) and WSL2 without requiring kernel patches or proprietary libraries.
- **NFR-05 (Usability):** Configuration must be managed via a clear, human-readable text file (e.g., `.conf`), rather than requiring recompilation to change routing rules or backend lists.

## 3. Hardware & Software Requirements

### Hardware Requirements
- **No dedicated physical hardware required.** The project is a software-only implementation.
- Standard x86_64 or ARM64 architecture (PC or Virtual Machine) capable of running Linux.

### Software Requirements
- **Operating System:** Linux (Ubuntu 20.04/22.04 LTS or WSL2 recommended).
- **Programming Language:** C++17 or C++20.
- **Compiler:** GCC (g++) or Clang.
- **Build System:** CMake (version 3.10 or higher) and Make.
- **Version Control:** Git.

## 4. Development Plan & Milestones
Development is broken down into 14 incremental milestones to ensure structured progress and continuous testing.

| Milestone | Description | Goal |
| :--- | :--- | :--- |
| **M1** | Basic Backend Server | Create a lightweight C++ TCP server to act as a dummy backend. |
| **M2** | Basic TCP Client | Create a load-generating C++ client simulator. |
| **M3** | Basic TCP Load Balancer | Implement socket binding, listening, and basic connection acceptance. |
| **M4** | Backend Pool Logic | Implement parsing and memory storage of backend server configurations. |
| **M5** | Round Robin Routing | Route incoming connections to backends sequentially. |
| **M6** | Health Checking | Implement an asynchronous thread/timer to ping backends. |
| **M7** | Failure Detection | Add logic to remove failing backends from the active routing pool. |
| **M8** | Recovery Handling | Add logic to restore recovered backends to the active routing pool. |
| **M9** | Concurrent Clients | Refactor network I/O to use `epoll` for handling multiple simultaneous connections. |
| **M10** | Least-Connections Algo | Implement the secondary routing algorithm. |
| **M11** | Logging System | Integrate structured, level-based logging. |
| **M12** | Monitoring & Stats | Build the internal metrics tracker and status display. |
| **M13** | Graceful Shutdown | Implement Linux signal handling (SIGINT/SIGTERM) for clean exits. |
| **M14** | Testing & Optimization | Run unit/integration tests, benchmark throughput, and finalize code. |

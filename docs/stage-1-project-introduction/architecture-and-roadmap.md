# Initial High-Level Architecture & Roadmap

## Technology Stack
- **OS:** Linux (Targeting Ubuntu/WSL2 environments)
- **Language:** C++17/C++20
- **Build System:** CMake, GCC/G++
- **Version Control:** Git / GitHub
- **Key System APIs:** 
  - `socket`, `bind`, `listen`, `accept`, `connect`
  - `epoll` (for scalable I/O multiplexing)
  - `pthread` / `<thread>` / `<mutex>` (concurrency)
  - `signal` (graceful shutdown)

## Initial High-Level Architecture

```text
+-------------------------------------------------------------+
|                 Software-Defined Load Balancer              |
|                                                             |
|  +-------------+    +---------------+    +---------------+  |
|  | Config      |    | Health-Check  |    | Routing /     |  |
|  | Parser      |    | Manager       |    | Algo Engine   |  |
|  +-------------+    +---------------+    +---------------+  |
|                                                             |
|  +-------------------------------------------------------+  |
|  |                   epoll Event Loop                    |  |
|  +-------------------------------------------------------+  |
+-------------------------------------------------------------+
          ^                                   |
          | (TCP traffic)                     | (Forwarded requests)
          |                                   v
+-------------------+               +-------------------+
|                   |               |                   |
| Client Simulator  |               | Backend Pool      |
|                   |               | (Server 1, 2, 3)  |
+-------------------+               +-------------------+
```

## Development Roadmap (Mapped to Academic Stages)

- **Stage 1: Project Introduction (Completed)**
  - Definition, Problem Statement, Scope, Objectives, Architecture Preview.
  
- **Stage 2: Project Requirements & Development Plan**
  - PRD creation (Functional & Non-Functional Requirements).
  - Environment specification and detailed timeline formulation.

- **Stage 3: System Design & Architecture**
  - UML generation (Component, Class, Sequence, State Machine).
  - Internal data structure definition and thread modeling.
  - Establishment of Git branching strategy.

- **Stage 4: Initial Implementation & Prototype**
  - Incremental execution (Milestone 1 to 14).
  - Implementation of simulators, basic TCP proxying, epoll loops.
  - Introduction of Round Robin, Least Connections, and Health-Check manager.

- **Stage 5: Testing, Integration & Improvement**
  - Execution of test matrices.
  - Validation of failover and recovery states.
  - Load/Stress testing with performance telemetry capture.

- **Stage 6: Final Implementation & Presentation**
  - Code cleanup and repository finalization.
  - README, final report, and presentation slides.
  - 50+ Viva questions preparation.
  - Complete end-to-end demonstration.
